/**
 * @file Cosa/Ciao/event_desc.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012, Mikael Patel
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
 * @section Description
 * The Cosa Ciao data stream digital pin value descriptor.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/Ciao.h"

static const char type_name[] PROGMEM = "type";
static const char target_name[] PROGMEM = "target";
static const char value_name[] PROGMEM = "value";
static const Ciao::desc_member_t members[] PROGMEM = {
  {
    Ciao::UINT8_TYPE,
    1,
    type_name,
    0
  },
  {
    Ciao::UINT16_TYPE,
    1,
    target_name,
    0
  },
  {
    Ciao::UINT16_TYPE,
    1,
    value_name,
    0
  }
};
static const char name[] PROGMEM = "Cosa::Event";
const Ciao::desc_user_t Ciao::event_desc PROGMEM = {
  Ciao::EVENT_ID,
  name,
  members,
  membersof(members)
};
