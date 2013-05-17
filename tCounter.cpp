//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
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
/*!\file    rrlib/getopt/tCounter.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define __rrlib__getopt__include_guard__
#include "rrlib/getopt/tCounter.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/logging/messages.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace getopt
{

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
// tCounter SetValueFromString
//----------------------------------------------------------------------
bool tCounter::SetValueFromString(const std::string &value)
{
  if (!value.empty())
  {
    RRLIB_LOG_PRINT(ERROR, "Illegal value for counter option '", this->GetName(), "': ", value, "!");
    return false;
  }
  this->value++;
  return tOptionBase::SetValueFromString(value);
}

//----------------------------------------------------------------------
// EvaluateCounter
//----------------------------------------------------------------------
unsigned int EvaluateCounter(const std::shared_ptr<const tOptionBase> option)
{
  const tCounter *counter = dynamic_cast<const tCounter *>(option.get());
  if (!counter)
  {
    throw std::logic_error("Trying to treat an option as counter which was not created as such!");
  }
  return counter->value;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
