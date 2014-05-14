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
/*!\file    rrlib/getopt/examples/getopt.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdlib>
#include <libgen.h>

#include "rrlib/getopt/parser.h"

#include "rrlib/logging/messages.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

bool OptionsHandler(const rrlib::getopt::tNameToOptionMap &name_to_option_map)
{
  if (name_to_option_map.at("port")->IsActive())
  {
    int port = atoi(rrlib::getopt::EvaluateValue(name_to_option_map.at("port")).c_str());
    RRLIB_LOG_PRINT(USER, "Port was set to ", port);
  }

  if (name_to_option_map.at("foo")->IsActive())
  {
    RRLIB_LOG_PRINT(USER, "Foo was enabled (set to ", rrlib::getopt::EvaluateFlag(name_to_option_map.at("foo")), ")");
  }

  if (name_to_option_map.at("l")->IsActive())
  {
    RRLIB_LOG_PRINT(USER, "l was set to ", rrlib::getopt::EvaluateValue(name_to_option_map.at("l")));
  }

  if (name_to_option_map.at("r")->IsActive())
  {
    RRLIB_LOG_PRINT(USER, "r was enabled");
  }

  if (name_to_option_map.at("v")->IsActive())
  {
    int verbosity = rrlib::getopt::EvaluateCounter(name_to_option_map.at("v"));
    RRLIB_LOG_PRINT(USER, "v was enabled and set to ", verbosity);
  }

  return true;
}

int main(int argc, char **argv)
{
  rrlib::logging::default_log_description = basename(argv[0]);

  rrlib::getopt::AddValue("port", 'p', "A litte help", &OptionsHandler);
  rrlib::getopt::AddFlag("foo", 'f', "from my friends", &OptionsHandler);
  rrlib::getopt::AddValue(0, 'l', "lalala", &OptionsHandler);
  rrlib::getopt::AddFlag(0, 'r', "lululu", &OptionsHandler);
  rrlib::getopt::AddCounter(0, 'v', "blubb", &OptionsHandler);
  rrlib::getopt::AddValue("long_option", 0, "Another long option with a\nreally long help text", &OptionsHandler);

  rrlib::getopt::ProcessCommandLine(argc, argv, "This is an example for the RRLib getopt implementation.", "<files>", "Some text that should be printed, too.");

  return EXIT_SUCCESS;
}
