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
/*!\file    parser.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 * \brief
 *
 * \b
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_getopt_parser_h_
#define _rrlib_getopt_parser_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define _rrlib_getopt_include_guard_

#include "rrlib/getopt/tOptionBase.h"
#include "rrlib/getopt/tFlag.h"
#include "rrlib/getopt/tCounter.h"
#include "rrlib/getopt/tValue.h"

#undef _rrlib_getopt_include_guard_

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
typedef std::map<std::string, tOptionBase *> tNameToOptionMap;
typedef bool (*tHandler)(const tNameToOptionMap &name_to_option_map);

//----------------------------------------------------------------------
// Function declaration
//----------------------------------------------------------------------

void AddOption(const tFlag &option, tHandler handler);

void AddOption(const tCounter &option, tHandler handler);

void AddOption(const tValue &option, tHandler handler);

void SetProgramVersion(const char *version);

void SetProgramDescription(const char *description);

std::vector<char *> ProcessCommandLine(int argc, char **argv);

std::vector<char *> ProcessCommandLine(int argc, char **argv, const char *program_version, const char *program_description);



//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
