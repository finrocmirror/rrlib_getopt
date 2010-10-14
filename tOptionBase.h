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
/*!\file    tOptionBase.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 * \brief Contains tOptionBase
 *
 * \b tOptionBase
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_getopt_include_guard_
#error Invalid include directive. Try #include "rrlib/getopt/parser.h" instead.
#endif

#ifndef _rrlib_getopt_tOptionBase_h_
#define _rrlib_getopt_tOptionBase_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <boost/any.hpp>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/definitions.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace getopt
{
RRLIB_LOG_CREATE_DEFAULT_DOMAIN("getopt");

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//!
/*!
 *
 */
class tOptionBase
{
  const char *long_name;
  char short_name;
  const char *help;
  bool active;

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  inline const char *GetLogDescription()
  {
    return this->GetDescription();
  }

  tOptionBase(const char *long_name, const char short_name, const char *help);

  virtual ~tOptionBase() = 0;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  virtual inline const char *GetDescription()
  {
    return "tOptionBase";
  }

  inline const std::string GetName() const
  {
    return this->long_name ? this->long_name : std::string() + this->short_name;
  }

  inline const char *GetLongName() const
  {
    return this->long_name;
  }

  inline const char GetShortName() const
  {
    return this->short_name;
  }

  inline const char *GetHelp() const
  {
    return this->help;
  }

  inline bool IsActive() const
  {
    return this->active;
  }

  virtual const bool HasParameter() const;

  virtual const boost::any GetValue() const = 0;

  virtual const bool SetValueFromParameter(const char *parameter) = 0;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
