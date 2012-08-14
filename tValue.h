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
/*!\file    tValue.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 * \brief Contains tValue
 *
 * \b tValue
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__getopt__include_guard__
#error Invalid include directive. Try #include "rrlib/getopt/parser.h" instead.
#endif

#ifndef __rrlib__getopt__tValue_h__
#define __rrlib__getopt__tValue_h__

#include "rrlib/getopt/tOptionBase.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

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
class tValue : public tOptionBase
{

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tValue(const char *long_name, const char short_name, const char *help);

  virtual inline const char *GetDescription() const
  {
    return "tValue";
  }

  virtual const bool HasParameter() const;

  virtual const boost::any GetValue() const;

  virtual const bool SetValueFromParameter(const char *parameter);

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  const char *value;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
