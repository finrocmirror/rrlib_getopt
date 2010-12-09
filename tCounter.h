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
/*!\file    tCounter.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 * \brief Contains tCounter
 *
 * \b tCounter
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_getopt_include_guard_
#error Invalid include directive. Try #include "rrlib/getopt/parser.h" instead.
#endif

#ifndef _rrlib_getopt_tCounter_h_
#define _rrlib_getopt_tCounter_h_

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
class tCounter : public tOptionBase
{
  unsigned int value;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  tCounter(const char *long_name, const char short_name, const char *help);

  virtual inline const char *GetDescription() const
  {
    return "tCounter";
  }

  virtual const boost::any GetValue() const;

  virtual const bool SetValueFromParameter(const char *parameter);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
