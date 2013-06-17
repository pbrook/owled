/*
  One Wire RGB LED demo
 */
#include <owled.h> 

OWLed leds(6);

static const int button = 8;

static int next_address;

// the setup routine runs once when you press reset:
void setup()
{
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
}

static void do_config(void)
{
  leds.SetAddress(next_address);
  leds.WriteConfig(OWLED_COMMON_ANODE,
      OWLED_CHANNEL_GREEN | OWLED_BRIGHT_MUL(5) | OWLED_BRIGHT_SHIFT(1),
      OWLED_CHANNEL_BLUE  | OWLED_BRIGHT_MUL(8) | OWLED_BRIGHT_SHIFT(1),
      OWLED_CHANNEL_RED   | OWLED_BRIGHT_MUL(6) | OWLED_BRIGHT_SHIFT(3));
  Serial.print("Configured address ");
  Serial.println(next_address);
  next_address++;
}

#define NUM_LEDS 6

const struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb[7] = {
      {255,255,255},
      {255,0,0},
      {0,255,0},
      {0,0,255},
      {0,255,255},
      {255,0,255},
      {255,255,0},
};
// the loop routine runs over and over again forever:
void loop()
{
  int addr;
  unsigned long now;
  unsigned long last_tick;
  int count;
  int n;

  last_tick = millis();
  count = 0;

  while(1)
    {
      if (!digitalRead(button))
	{
	  do_config();
	  while (!digitalRead(button))
	    delay(1);
	  delay(100);
	  count = 0;
	}

      now = millis();
      if ((long)(now - last_tick) > 500)
	{
	  for (addr = 0; addr < 16; addr++)
	    {
	      Serial.println(addr);
	      n = (count + addr) % 7;
	      leds.SetColor(addr, rgb[n].r, rgb[n].g, rgb[n].b);
	    }
	  count++;
	  if (count == 7)
	    count = 0;
	  last_tick = now;
	}
    }
}

