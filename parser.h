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
/*!\file    rrlib/getopt/parser.h
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
#ifndef __rrlib__getopt__parser_h__
#define __rrlib__getopt__parser_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <memory>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define __rrlib__getopt__include_guard__

#include "rrlib/getopt/tOptionBase.h"
#include "rrlib/getopt/tFlag.h"
#include "rrlib/getopt/tCounter.h"
#include "rrlib/getopt/tValue.h"

#undef __rrlib__getopt__include_guard__

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
typedef std::shared_ptr<const tOptionBase> tOption;
typedef std::map<std::string, tOption> tNameToOptionMap;
typedef bool (*tHandler)(const tNameToOptionMap &name_to_option_map);

//----------------------------------------------------------------------
// Function declaration
//----------------------------------------------------------------------

const tOption AddFlag(const char *long_name, const char short_name, const char *help, tHandler handler);

const tOption AddCounter(const char *long_name, const char short_name, const char *help, tHandler handler);

const tOption AddValue(const char *long_name, const char short_name, const char *help, tHandler handler, bool restrict_to_single_occurrence = false);

std::vector<std::string> ProcessCommandLine(size_t argc, char **argv,
    const std::string &description = "", const std::string &command_line_arguments = "", const std::string &additional_help_text = "");


//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
