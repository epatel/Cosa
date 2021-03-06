/**
 * @file Cosa/IOPin.hh
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

#ifndef __COSA_IO_PIN_HH__
#define __COSA_IO_PIN_HH__

#include "Cosa/OutputPin.hh"

/**
 * Abstract IO-pin that may switch between input and output pin.
 */
class IOPin : public OutputPin {
public:
  enum Mode {
    OUTPUT_MODE = 0,
    INPUT_MODE = 1
  } __attribute__((packed));

  /**
   * Construct abstract in/output pin given Arduino pin number.
   * @param[in] pin number.
   * @param[in] mode pin mode (normal or pullup).
   */
  IOPin(Board::DigitalPin pin, Mode mode = INPUT_MODE, bool pullup = false) : 
    OutputPin(pin),
    m_mode(mode)
  {
    if (pullup)
      *PORT() |= m_mask; 
    set_mode(mode);
  }

  /**
   * Change IO-pin to given mode.
   * @param[in] mode new operation mode.
   */
  void set_mode(Mode mode) __attribute__((always_inline))
  {
    synchronized {
      if (mode == OUTPUT_MODE)
	*DDR() |= m_mask; 
      else
	*DDR() &= ~m_mask; 
    }
    m_mode = mode;
  }
  
  /**
   * Get current IO-pin mode.
   * @return mode.
   */
  Mode get_mode() const
  {
    return (m_mode);
  }
  
  /**
   * Change IO-pin to given mode.
   * @param[in] pin number.
   * @param[in] mode new operation mode.
   */
  static void set_mode(uint8_t pin, Mode mode) __attribute__((always_inline)) 
  { 
    volatile uint8_t* ddr = DDR(pin);
    const uint8_t mask = MASK(pin);
    synchronized {
      if (mode == OUTPUT_MODE)
	*ddr |= mask; 
      else
	*ddr &= ~mask; 
    }
  }

private:
  Mode m_mode;
};

#endif
