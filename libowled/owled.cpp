/*
  One-wire RGB led driver module
 */
 
#include "owled.h"

#define DELAY_ZERO  1
#define DELAY_ONE   4
#define DELAY_BREAK 7

OWLed::OWLed(int pin, bool invert, int delay)
{
  uint8_t port = digitalPinToPort(pin);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, !invert);
#ifndef __AVR__
#error Direct port access almost certainly broken
#endif
  pinmask = digitalPinToBitMask(pin);
  // Writing 1 to input register toggles the output
  pinreg = portInputRegister(port);
  bit_delay = delay;
}

void
OWLed::pulse(int len)
{
  noInterrupts();
  *pinreg = pinmask;
  delayMicroseconds(len);
  *pinreg = pinmask;
  interrupts();
}

void
OWLed::out_value(uint16_t value)
{
  int i;
  int len;

  pulse(DELAY_BREAK);
  delayMicroseconds(bit_delay);
  for (i = 0; i < 16; i++)
    {
      if (value & 1)
	len = DELAY_ONE;
      else
	len = DELAY_ZERO;
      value >>= 1;
      pulse(len);
      delayMicroseconds(bit_delay);
    }
}

void
OWLed::out_cmd(uint8_t op, uint8_t val)
{
  out_value(0xf | (op << 4) | ((uint16_t)val << 8) | ((uint16_t)(~op ^ val) << 12));
  // Give the module time to process the command.
  delay(10);
}

void
OWLed::SetAddress(int address)
{
  out_cmd(0, address);
}

void
OWLed::WriteConfig(enum owled_common_mode com, uint8_t ch0, uint8_t ch1, uint8_t ch2)
{
  out_cmd(1, com);
  out_cmd(2, ch0 & 0xf);
  out_cmd(3, ch1 & 0xf);
  out_cmd(4, ch2 & 0xf);
  out_cmd(5, ch0 >> 4);
  out_cmd(6, ch1 >> 4);
  out_cmd(7, ch2 >> 4);
}

void
OWLed::SetColor(int address, uint8_t r, uint8_t g, uint8_t b)
{
  uint16_t val;

  if (address < 0 || address > 0xf)
    return;

  val = address;
  val |= (uint16_t)r & 0x00f0;
  val |= ((uint16_t)g << 4) & 0x0f00;
  val |= ((uint16_t)b << 8) & 0xf000;
  out_value(val);
}
