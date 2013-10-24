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
/*!\file    rrlib/getopt/tFlag.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define __rrlib__getopt__include_guard__
#include "rrlib/getopt/tFlag.h"

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

///----------------------------------------------------------------------
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
// tFlag constructors
//----------------------------------------------------------------------
tFlag::tFlag(const char *long_name, const char short_name, const char *help)
  : tOptionBase(long_name, short_name, help)
{}

//----------------------------------------------------------------------
// tFlag SetValueFromString
//----------------------------------------------------------------------
bool tFlag::SetValueFromString(const std::string &value)
{
  if (this->IsActive())
  {
    RRLIB_LOG_PRINT(ERROR, "Double occurrence of flag option '", this->GetName(), "!");
    return false;
  }
  if (!value.empty())
  {
    RRLIB_LOG_PRINT(ERROR, "Illegal value for flag option '", this->GetName(), "': ", value, "!");
    return false;
  }
  return tOptionBase::SetValueFromString(value);
}

//----------------------------------------------------------------------
// EvaluateFlag
//----------------------------------------------------------------------
bool EvaluateFlag(const std::shared_ptr<const tOptionBase> option)
{
  const tFlag *flag = dynamic_cast<const tFlag *>(option.get());
  if (!flag)
  {
    throw std::logic_error("Trying to treat an option as flag which was not created as such!");
  }
  return flag->IsActive();
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
