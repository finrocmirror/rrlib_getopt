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
/*!\file    tFlag.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define _rrlib_getopt_include_guard_
#include "getopt/tFlag.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/definitions.h"

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
// tFlag constructors
//----------------------------------------------------------------------
tFlag::tFlag(const char *long_name, const char short_name, const char *help)
    : tOptionBase(long_name, short_name, help)
{}

//----------------------------------------------------------------------
// tFlag GetValue
//----------------------------------------------------------------------
const boost::any tFlag::GetValue() const
{
  return this->IsActive();
}

//----------------------------------------------------------------------
// tFlag SetValueFromParameter
//----------------------------------------------------------------------
const bool tFlag::SetValueFromParameter(const char *parameter)
{
  if (this->IsActive())
  {
    RRLIB_LOG_STREAM(eLL_ERROR) << "Double occurrence of flag option '" << this->GetName() << "': " << parameter << "!";
    return false;
  }
  if (parameter)
  {
    RRLIB_LOG_STREAM(eLL_ERROR) << "Illegal value for flag option '" << this->GetName() << "': " << parameter << "!";
    return false;
  }
  return tOptionBase::SetValueFromParameter(parameter);
}
