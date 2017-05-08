//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/getopt/tValue.cpp
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
tValue::tValue(const char *long_name, const char short_name, const char *help, bool restrict_to_single_occurrence)
  : tOptionBase(long_name, short_name, help),
    restrict_to_single_occurrence(restrict_to_single_occurrence)
{}

//----------------------------------------------------------------------
// tValue HasParameter
//----------------------------------------------------------------------
bool tValue::ExpectsValue() const
{
  return true;
}

//----------------------------------------------------------------------
// tValue SetValueFromString
//----------------------------------------------------------------------
bool tValue::SetValueFromString(const std::string &value)
{
  if (this->IsActive() && this->restrict_to_single_occurrence)
  {
    RRLIB_LOG_PRINT(ERROR, "Multiple occurrence of option '", this->GetName(), "'!");
    return false;
  }
  if (value.empty())
  {
    RRLIB_LOG_PRINT(ERROR, "Missing value for option '", this->GetName(), "'!");
    return false;
  }
  this->values.emplace_back(value);
  return tOptionBase::SetValueFromString(value);
}

//----------------------------------------------------------------------
// EvaluateValue
//----------------------------------------------------------------------
const std::string &EvaluateValue(const std::shared_ptr<const tOptionBase> option)
{
  const tValue *value = dynamic_cast<const tValue *>(option.get());
  if (!value)
  {
    throw std::logic_error("Trying to treat an option as value which was not created as such!");
  }
  assert(!value->values.empty());
  if (value->values.size() > 1)
  {
    RRLIB_LOG_PRINT(WARNING, "Evaluating only first value of a list of values (multiple occurrence of option ", option->GetName(), ")");
  }
  return value->values.front();
}

//----------------------------------------------------------------------
// EvaluateValueList
//----------------------------------------------------------------------
const std::vector<std::string> &EvaluateValueList(const std::shared_ptr<const tOptionBase> option)
{
  const tValue *value = dynamic_cast<const tValue *>(option.get());
  if (!value)
  {
    throw std::logic_error("Trying to treat an option as value which was not created as such!");
  }
  assert(!value->values.empty());
  return value->values;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
