#ifndef OWLED_H
#define OWLED_H

#include <stdint.h>
#include "Arduino.h"

#define OWLED_CHANNEL_RED 0x4
#define OWLED_CHANNEL_GREEN 0x8
#define OWLED_CHANNEL_BLUE 0xc

// Output channel brightness is calculated as
//  output = (input * mul) >> shift;
// where
//  1 <= mul <= 16
//  0 <= shift <= 3
#define OWLED_BRIGHT_MUL(n) (((n) - 1) << 4)
#define OWLED_BRIGHT_SHIFT(n) (n)

enum owled_common_mode {
  OWLED_COMMON_CATHODE_PASSIVE,
  OWLED_COMMON_CATHODE,
  OWLED_COMMON_ANODE
};

// One-wire LED bus controller
class OWLed
{
public:
  OWLed(int pin, bool invert = false, int delay = 100);

  // Set LED color.  r, g, b values are 0-255
  void SetColor(int address, uint8_t r, uint8_t g, uint8_t b);

  // Following two functions write to the LED module's EEPROM, and are not
  // needed during normal operation.

  // Set the LED address.  Will change all connected LEDs
  void SetAddress(int address);
  // Set LED configuration.  Will change the config of all connected LEDs
  // Each channel config is combination of OWLED_CHANNEL_*,
  // OWLED_BRIGHT_MUL and OWLED_BRIGHT_SHIFT
  void WriteConfig(enum owled_common_mode, uint8_t ch0, uint8_t ch1, uint8_t ch2);
private:
  int bit_delay;
  uint8_t pinmask;
  volatile uint8_t *pinreg;
  void pulse(int len);
  void out_value(uint16_t val);
  void out_cmd(uint8_t op, uint8_t val);
};

#endif
