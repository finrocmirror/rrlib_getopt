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
/*!\file    test_getopt.cpp
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
#include <iostream>

extern "C"
{
#include <libgen.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "getopt/parser.h"
#include "logging/definitions.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
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

bool OptionsHandler(const rrlib::getopt::tNameToOptionMap &name_to_option_map)
{
  if (name_to_option_map.at("port")->IsActive())
  {
    int port = atoi(boost::any_cast<const char *>(name_to_option_map.at("port")->GetValue()));
    RRLIB_LOG_STREAM(eLL_USER) << "Port was set to " << port;
  }

  if (name_to_option_map.at("foo")->IsActive())
  {
    RRLIB_LOG_STREAM(eLL_USER) << "Port was enabled (set to " << boost::any_cast<bool>(name_to_option_map.at("foo")->GetValue()) << ")";
  }

  if (name_to_option_map.at("l")->IsActive())
  {
    RRLIB_LOG_STREAM(eLL_USER) << "l was set to " << boost::any_cast<const char *>(name_to_option_map.at("l")->GetValue());
  }

  if (name_to_option_map.at("r")->IsActive())
  {
    RRLIB_LOG_STREAM(eLL_USER) << "r was enabled";
  }

  if (name_to_option_map.at("v")->IsActive())
  {
    int verbosity = boost::any_cast<unsigned int>(name_to_option_map.at("v")->GetValue());
    RRLIB_LOG_STREAM(eLL_USER) << "v was enabled and set to " << verbosity;
  }

  return true;
}

int main(int argc, char **argv)
{
  default_log_description = basename(argv[0]);

//  tLogDomainRegistry::GetInstance()->SetDomainMaxMessageLevel(".getopt", eLL_DEBUG_VERBOSE_3);

  rrlib::getopt::AddOption(rrlib::getopt::tValue("port", 'p', "A litte help"), OptionsHandler);
  rrlib::getopt::AddOption(rrlib::getopt::tFlag("foo", 'f', "from my friends"), OptionsHandler);
  rrlib::getopt::AddOption(rrlib::getopt::tValue(0, 'l', "lalala"), OptionsHandler);
  rrlib::getopt::AddOption(rrlib::getopt::tFlag(0, 'r', "lululu"), OptionsHandler);
  rrlib::getopt::AddOption(rrlib::getopt::tCounter(0, 'v', "blubb"), OptionsHandler);

  rrlib::getopt::ProcessCommandLine(argc, argv);

  return EXIT_SUCCESS;
}
