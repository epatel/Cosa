/**
 * @file Cosa/PinChangeInterrupt.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012-2014, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/PinChangeInterrupt.hh"

// Define symbols for enable/disable pin change interrupts
#if defined(GIMSK)
#define PCICR GIMSK
#endif
#if defined(PCIE3)
#define PCIE (_BV(PCIE3) | _BV(PCIE2) | _BV(PCIE1) | _BV(PCIE0))
#elif defined(PCIE2)
#define PCIE (_BV(PCIE2) | _BV(PCIE1) | _BV(PCIE0))
#elif defined(PCIE1)
#define PCIE (_BV(PCIE1) | _BV(PCIE0))
#elif defined(PCIE0)
#define PCIE (_BV(PCIE0))
#endif

PinChangeInterrupt* PinChangeInterrupt::s_pin[Board::PCINT_MAX] = { NULL };
uint8_t PinChangeInterrupt::s_state[Board::PCMSK_MAX] = { 0 };

void 
PinChangeInterrupt::enable() 
{ 
  synchronized {
    *PCIMR() |= m_mask;
#if defined(__ARDUINO_MEGA__)
    uint8_t ix = m_pin - (m_pin < 24 ? 16 : 48);
    s_pin[ix] = this;
#else
    s_pin[m_pin] = this;
#endif
  }
}

void 
PinChangeInterrupt::disable() 
{ 
  synchronized {
    *PCIMR() &= ~m_mask;
#if defined(__ARDUINO_MEGA__)
    uint8_t ix = m_pin - (m_pin < 24 ? 16 : 48);
    s_pin[ix] = NULL;
#else
    s_pin[m_pin] = NULL;
#endif
  }
}

void 
PinChangeInterrupt::begin()
{
#if defined(__ARDUINO_MEGA__)
  s_state[0] = *Pin::PIN(16);
  s_state[1] = 0;
  s_state[2] = *Pin::PIN(64);
#else
  for (uint8_t i = 0; i < Board::PCMSK_MAX; i++)
    s_state[i] = *Pin::PIN(i << 3);
#endif

  synchronized {
    bit_mask_set(PCICR, PCIE);
  }
}

void 
PinChangeInterrupt::end()
{
  synchronized {
    bit_mask_clear(PCICR, PCIE);
  }
}

void
PinChangeInterrupt::on_interrupt(uint8_t pcint, uint8_t mask, uint8_t base)
{
  uint8_t new_state = *Pin::PIN(base);
  uint8_t changed = (new_state ^ s_state[pcint]) & mask;
  base = (pcint << 3);
  
  for (uint8_t i = 0; changed && (i < CHARBITS); i++) {
    if ((changed & 1) && (s_pin[base + i] != NULL)) {
      s_pin[base + i]->on_interrupt();
    }
    changed >>= 1;
  }

  s_state[pcint] = new_state;
}

#define PCINT_ISR(vec,pcint,base)				\
ISR(PCINT ## vec ## _vect)					\
{								\
  PinChangeInterrupt::on_interrupt(pcint, PCMSK ## vec, base);	\
}

#if defined(__ARDUINO_TINYX61__)

ISR(PCINT0_vect)
{
  uint8_t mask;
  uint8_t ix;

  if (GIFR & _BV(INTF0)) {
    mask = PCMSK0;
    ix = 0;
  } else {
    mask = PCMSK1;
    ix = 1;
  }
  PinChangeInterrupt::on_interrupt(ix, mask, (ix << 3));
}

#elif defined(__ARDUINO_TINYX5__)

PCINT_ISR(0, 0, 0);

#elif defined(__ARDUINO_TINYX4__)

PCINT_ISR(0, 0, 0);
PCINT_ISR(1, 1, 8);

#elif defined(__ARDUINO_STANDARD__)

PCINT_ISR(0, 1, 8);
PCINT_ISR(1, 2, 14);
PCINT_ISR(2, 0, 0);

#elif defined(__ARDUINO_STANDARD_USB__)

PCINT_ISR(0, 0, 0);

#elif defined(__ARDUINO_MEGA__)

PCINT_ISR(0, 0, 16);
ISR(PCINT1_vect) {}
PCINT_ISR(2, 2, 64);

#elif defined(__ARDUINO_MIGHTY__)

PCINT_ISR(0, 0, 0);
PCINT_ISR(1, 1, 8);
PCINT_ISR(2, 2, 16);
PCINT_ISR(3, 3, 24);

#elif defined(__PINOCCIO_SCOUT__)

PCINT_ISR(0, 0, 0);
PCINT_ISR(1, 1, 8);
PCINT_ISR(2, 2, 16);

#endif
