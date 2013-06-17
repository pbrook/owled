#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

/* V1 hardware: COMMON_CATHODE
  PIN0: Green
  PIN1: Red
  PIN2: Blue */

#define PIN_LED0  _BV(0)
#define PIN_LED1  _BV(1)
#define PIN_LED2  _BV(4)
/* LED common pin.  */
#define PIN_COM	  _BV(3)
/* Power sense pin.  */
#define PIN_PWR	  _BV(2)


static const uint8_t pin_mask[3] = {
    PIN_LED0,
    PIN_LED1,
    PIN_LED2,
};
#define PIN_MASK_LED (PIN_LED0 | PIN_LED1 | PIN_LED2)

static volatile uint8_t * const pin_count[3] =
{
  &OCR0A,
  &OCR1A,
  &OCR1B
};

static uint8_t pin_config[3];

#define PORT_ON(mask) LED_PORT |= (mask)
#define PORT_OFF(mask) LED_PORT &= ~(mask)

struct {
  uint8_t addr;
  uint8_t common;
  uint8_t pins[3];
} eeprom EEMEM;

uint8_t my_addr;
enum {
    COMMON_CATHODE_PASSIVE,
    COMMON_CATHODE,
    COMMON_ANNODE,
};
uint8_t common;

static void
setup_clock(void)
{
  CLKPR = _BV(CLKPCE);
  CLKPR = 0;
}

static void
disable_outputs(void)
{
  DDRB &= ~(PIN_MASK_LED);
}

/* Select common annode and common cathode.  */
static void
select_common(void)
{
  disable_outputs();
  DDRB &= ~PIN_COM;
  if (common == COMMON_ANNODE)
    {
      TCCR0A |= _BV(COM0A0);
      TCCR1 |= _BV(COM1A0);
      GTCCR |= _BV(COM1B0);
      DDRB |= PIN_COM;
      PORTB |= PIN_COM;
    }
  else
    {
      TCCR0A &= ~_BV(COM0A0);
      TCCR1 &= ~_BV(COM1A0);
      GTCCR &= ~_BV(COM1B0);
      if (common == COMMON_CATHODE)
	{
	  DDRB |= PIN_COM;
	  PORTB &= ~PIN_COM;
	}
    }
}

static void
setup_pwm(void)
{
  /* Use OCR1A not OCR0B to avoid hardware bug on attiny45.  */
  /* Timer0 */
  /* Fast PWM, clear on match.  */
  TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS00);
  /* Timer1 */
  /* PWM channels A and B, clear on match.  */
  TCCR1 = _BV(CS10) | _BV(PWM1A) | _BV(COM1A1);
  GTCCR = _BV(PWM1B) | _BV(COM1B1);
  /* Zero all the counters.  */
  OCR0A = 0x00;
  OCR1A = 0x00;
  OCR1B = 0x00;
  select_common();
}

static void
read_eeprom(void)
{
  int i;

  my_addr = eeprom_read_byte(&eeprom.addr);
  if (my_addr >= 0xf)
    my_addr = 0xf;

  common = eeprom_read_byte(&eeprom.common);
  if (common == 0xff)
    common = COMMON_CATHODE_PASSIVE;

  for (i = 0; i < 3; i++)
    {
      pin_config[i] = eeprom_read_byte(&eeprom.pins[i]);
      if (pin_config[i] == 0xff)
	pin_config[i] = 0;
    }
}

static void
update_eeprom(void)
{
  int i;

  eeprom_update_byte(&eeprom.addr, my_addr);
  eeprom_update_byte(&eeprom.common, common);
  for (i = 0; i < 3; i++)
    eeprom_update_byte(&eeprom.pins[i], pin_config[i]);
}

static void
process_config(uint16_t cmd)
{
  uint8_t n;
  uint8_t op;

  op = (cmd >> 4) & 0xf;
  /* Parity check.  */
  n = (cmd >> 8) & 0xf;
  if ((op ^ n) != ((~cmd >> 12) & 0xf))
    return;
  switch (op)
    {
    case 0: /* Set address */
      if (n >= 0xf)
	return;
      my_addr = n;
      update_eeprom();
      break;
    case 1: /* Common config.  */
      if (n >= 3)
	return;
      common = n;
      select_common();
      update_eeprom();
      break;
    case 2: /* Pin config.  */
    case 3:
    case 4:
      disable_outputs();
      pin_config[op - 2] = (pin_config[op - 2] & 0xf0) | n;
      update_eeprom();
      break;
    case 5: /* Brightness control.  */
    case 6:
    case 7:
      disable_outputs();
      pin_config[op - 5] = (pin_config[op - 5] & 0x0f) | (n << 4);
      update_eeprom();
      break;
    }
  return;
}

static void process_cmd(uint16_t cmd)
{
  uint8_t n;
  int i;

  n = cmd & 0xf;
  if (n == 0xf)
    {
      process_config(cmd);
      return;
    }

  if (n != my_addr)
    return;

  for (i = 0; i < 3; i++)
    {
      if ((pin_config[i] & 0xc) == 0)
	continue;

      n = (cmd >> (pin_config[i] & 0xc)) & 0xf;
      if (n == 0)
	{
	  DDRB &= ~pin_mask[i];
	}
      else
	{
	  n *= (pin_config[i] >> 4) + 1;
	  *pin_count[i] = n >> (pin_config[i] & 3);
	  DDRB |= pin_mask[i];
	}
    }
}

static void
do_serial(void)
{
  uint16_t value;
  uint16_t mask;
  uint8_t count;

  mask = 0;
  value = 0;
  while (1)
    {
      while (PINB & PIN_PWR)
	/* no-op */;

      count = 0;
      while ((PINB & PIN_PWR) == 0)
	count++;

      if (count >= 8)
	{
	  /* Break.  */
	  mask = 1;
	  value = 0;
	}
      else if (mask)
	{

	  if (count >= 4)
	    value |= mask;
	  mask <<= 1;
	  if (mask == 0)
	    process_cmd(value);
	}
    }
}

int
main()
{
  setup_clock();

  read_eeprom();

  setup_pwm();

  DDRB &= ~PIN_PWR;
  PORTB &= ~PIN_PWR;

  do_serial();
  
  return 0;
}
