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
#ifndef __rrlib__getopt__include_guard__
#error Invalid include directive. Try #include "rrlib/getopt/parser.h" instead.
#endif

#ifndef __rrlib__getopt__tOptionBase_h__
#define __rrlib__getopt__tOptionBase_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <boost/any.hpp>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  virtual inline const char *GetDescription() const
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

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  inline const char *GetLogDescription() const
  {
    return this->GetDescription();
  }

  tOptionBase(const char *long_name, const char short_name, const char *help);

  virtual ~tOptionBase() = 0;

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  const char *long_name;
  char short_name;
  const char *help;
  bool active;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
