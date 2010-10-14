//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) AG Robotersysteme TU Kaiserslautern
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//----------------------------------------------------------------------
/*!\file    tCounter.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define _rrlib_getopt_include_guard_
#include "rrlib/getopt/tCounter.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/definitions.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::getopt;
using namespace rrlib::logging;

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tCounter constructors
//----------------------------------------------------------------------
tCounter::tCounter(const char *long_name, const char short_name, const char *help)
    : tOptionBase(long_name, short_name, help),
    value(0)
{}

//----------------------------------------------------------------------
// tCounter GetValue
//----------------------------------------------------------------------
const boost::any tCounter::GetValue() const
{
  return this->value;
}

//----------------------------------------------------------------------
// tCounter SetValueFromParameter
//----------------------------------------------------------------------
const bool tCounter::SetValueFromParameter(const char *parameter)
{
  if (parameter)
  {
    RRLIB_LOG_STREAM(eLL_ERROR) << "Illegal value for counter option '" << this->GetName() << "': " << parameter << "!";
    return false;
  }
  this->value++;
  return tOptionBase::SetValueFromParameter(parameter);
}
