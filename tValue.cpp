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
/*!\file    tValue.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define __rrlib__getopt__include_guard__
#include "rrlib/getopt/tValue.h"

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
// tValue constructors
//----------------------------------------------------------------------
tValue::tValue(const char *long_name, const char short_name, const char *help)
  : tOptionBase(long_name, short_name, help),
    value(0)
{}

//----------------------------------------------------------------------
// tValue HasParameter
//----------------------------------------------------------------------
const bool tValue::HasParameter() const
{
  return true;
}

//----------------------------------------------------------------------
// tValue GetValue
//----------------------------------------------------------------------
const boost::any tValue::GetValue() const
{
  return this->value;
}

//----------------------------------------------------------------------
// tValue SetValueFromParameter
//----------------------------------------------------------------------
const bool tValue::SetValueFromParameter(const char *parameter)
{
  if (this->IsActive())
  {
    RRLIB_LOG_PRINT(ERROR, "Double occurrence of option '", this->GetName(), "'!");
    return false;
  }
  if (!parameter)
  {
    RRLIB_LOG_PRINT(ERROR, "Missing value for option '", this->GetName(), "'!");
    return false;
  }
  this->value = parameter;
  return tOptionBase::SetValueFromParameter(parameter);
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
