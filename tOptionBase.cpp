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
/*!\file    tOptionBase.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#define __rrlib__getopt__include_guard__
#include "rrlib/getopt/tOptionBase.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <cstring>

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
// tOptionBase constructors
//----------------------------------------------------------------------
tOptionBase::tOptionBase(const char *long_name, const char short_name, const char *help)
  : long_name(long_name),
    short_name(short_name),
    help(help),
    active(false)
{
  RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "long_name = ", long_name, ", short_name = ", short_name, ", help = ", help);
  assert((long_name || short_name) && "Specify at least a long or a short name for each option!");
  assert((!long_name || strlen(long_name)) && "long_name may not be an empty string!");
  assert((!short_name || (('A' <= short_name && short_name <= 'Z') || ('a' <= short_name && short_name <= 'z'))) && "Not a valid character for short_name!");
  assert(help && "Specify a help text for each option!");
}

//----------------------------------------------------------------------
// tOptionBase destructor
//----------------------------------------------------------------------
tOptionBase::~tOptionBase()
{}

//----------------------------------------------------------------------
// tOptionBase HasParameter
//----------------------------------------------------------------------
const bool tOptionBase::HasParameter() const
{
  return false;
}

//----------------------------------------------------------------------
// tOptionBase SetValueFromParameter
//----------------------------------------------------------------------
const bool tOptionBase::SetValueFromParameter(const char *)
{
  this->active = true;
  return true;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
