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
/*!\file    parser.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-31
 *
 */
//----------------------------------------------------------------------
#include "rrlib/getopt/parser.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdlib>
#include <cstring>
#include <iomanip>

extern "C"
{
#include <libgen.h>
}
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/definitions.h"

#include "rrlib/util/stl_container/join.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

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
typedef tNameToOptionMap tLongNameToOptionMap;
typedef std::map<char, tOptionBase *> tShortNameToOptionMap;
typedef std::map<tHandler, tNameToOptionMap> tHandlerToNameToOptionMapMap;

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace
{


std::vector<std::pair<tFlag, tHandler> > &GetFlags()
{
  static std::vector<std::pair<tFlag, tHandler> > flags;
  return flags;
}
std::vector<std::pair<tCounter, tHandler> > &GetCounters()
{
  static std::vector<std::pair<tCounter, tHandler> > counters;
  return counters;
}
std::vector<std::pair<tValue, tHandler> > &GetValues()
{
  static std::vector<std::pair<tValue, tHandler> > values;
  return values;
}

//----------------------------------------------------------------------
// AddOptionToList
//----------------------------------------------------------------------
template <typename TOption>
static void AddOptionToList(const TOption &option, tHandler handler, std::vector<std::pair<TOption, tHandler> > &option_list)
{
  RRLIB_LOG_STREAM(eLL_DEBUG_VERBOSE_1) << "long_name = " << (option.GetLongName() ? option.GetLongName() : "<null>") << ", short_name = " << (option.GetShortName() ? std::string() + option.GetShortName() : "<null>") << ", help = " << (option.GetHelp() ? option.GetHelp() : "<null>");
  assert(handler);
  option_list.push_back(std::make_pair(option, handler));
}

}

//----------------------------------------------------------------------
// AddOption
//----------------------------------------------------------------------
void AddOption(const tFlag &option, tHandler handler)
{
  AddOptionToList(option, handler, GetFlags());
}

void AddOption(const tCounter &option, tHandler handler)
{
  AddOptionToList(option, handler, GetCounters());
}

void AddOption(const tValue &option, tHandler handler)
{
  AddOptionToList(option, handler, GetValues());
}


namespace
{

tLongNameToOptionMap long_name_to_option_map;
tShortNameToOptionMap short_name_to_option_map;
tHandlerToNameToOptionMapMap handler_to_name_to_option_map_map;

const char *program_name = 0;
const char *program_version = "<Program version not defined>";
const char *program_description = "<Program description not defined>";

//----------------------------------------------------------------------
// IsInMap
//----------------------------------------------------------------------
template <typename TMap>
static inline const bool IsInMap(const TMap &map, const typename TMap::key_type &key)
{
  typename TMap::const_iterator it = map.find(key);
  return it != map.end();
}

//----------------------------------------------------------------------
// PrepareMaps
//----------------------------------------------------------------------
template <typename TOption>
static const bool PrepareMaps(std::vector<std::pair<TOption, tHandler> > &option_list)
{
  for (typename std::vector<std::pair<TOption, tHandler> >::iterator it = option_list.begin(); it != option_list.end(); ++it)
  {
    handler_to_name_to_option_map_map[it->second][it->first.GetName()] = &(it->first);

    if (it->first.GetLongName())
    {
      if (IsInMap(long_name_to_option_map, it->first.GetLongName()))
      {
        RRLIB_LOG_STREAM(rrlib::logging::eLL_ERROR) << "Option '" << it->first.GetLongName() << "' already exists with description '" << long_name_to_option_map.at(it->first.GetLongName())->GetHelp() << "'";
        return false;
      }
      long_name_to_option_map[it->first.GetLongName()] = &(it->first);
    }
    if (it->first.GetShortName())
    {
      if (IsInMap(short_name_to_option_map, it->first.GetShortName()))
      {
        RRLIB_LOG_STREAM(rrlib::logging::eLL_ERROR) << "Option '" << it->first.GetShortName() << "' already exists with description '" << short_name_to_option_map.at(it->first.GetShortName())->GetHelp() << "'";
        return false;
      }
      short_name_to_option_map[it->first.GetShortName()] = &(it->first);
    }
  }
  return true;
}

//----------------------------------------------------------------------
// PrintHelp
//----------------------------------------------------------------------
void PrintHelp(int return_code)
{
  unsigned int max_long_name_length = 0;

  for (tLongNameToOptionMap::iterator it = long_name_to_option_map.begin(); it != long_name_to_option_map.end(); ++it)
  {
    max_long_name_length = std::max(max_long_name_length, it->first.length());
  }

  {
    tLogStream log = RRLIB_LOG_STREAM(eLL_USER);
    if (return_code == EXIT_SUCCESS && program_version)
    {
      log << program_name << " " << program_version << std::endl << std::endl;
    }
    if (return_code == EXIT_SUCCESS && program_description)
    {
      log << program_description << std::endl << std::endl;
    }
    if (handler_to_name_to_option_map_map.empty())
    {
      log << "Usage: " << program_name << std::endl;
    }
    else
    {
      log << "Usage: " << program_name << " <OPTIONS> " << std::endl << std::endl << "Possible options are:" << std::endl;
    }
    for (tHandlerToNameToOptionMapMap::iterator it = handler_to_name_to_option_map_map.begin(); it != handler_to_name_to_option_map_map.end(); ++it)
    {
      for (tNameToOptionMap::iterator kt = it->second.begin(); kt != it->second.end(); ++kt)
      {
        std::string long_name = kt->second->GetLongName() ? std::string("--") + kt->second->GetLongName() + (kt->second->GetShortName() ? "," : "") : "";
        std::string short_name = kt->second->GetShortName() ? std::string("-") + kt->second->GetShortName() : "";
        log << " " << std::left << std::setw(max_long_name_length + 5) << long_name << std::setw(2) << short_name << "    ";

        const char *help = kt->second->GetHelp();
        size_t help_length = strlen(help);
        for (size_t i = 0; i < help_length; ++i)
        {
          if (help[i] == '\n')
          {
            log << std::endl << std::setw(max_long_name_length + 12) << "";
            continue;
          }
          log << help[i];
        }
        log << std::endl;
      }
    }
  }

  exit(return_code);
}

}

//----------------------------------------------------------------------
// SetProgramVersion
//----------------------------------------------------------------------
void SetProgramVersion(const char *version)
{
  program_version = version;
}

//----------------------------------------------------------------------
// SetProgramDescription
//----------------------------------------------------------------------
void SetProgramDescription(const char *description)
{
  program_description = description;
}

//----------------------------------------------------------------------
// ProcessCommandLine
//----------------------------------------------------------------------
std::vector<char *> ProcessCommandLine(int argc, char **argv)
{
  program_name = basename(argv[0]);

  PrepareMaps(GetFlags());
  PrepareMaps(GetCounters());
  PrepareMaps(GetValues());

  // create option list
  std::vector<char *> arguments(argv + 1, argv + argc);
  rrlib::util::Join(arguments, RRLIB_LOG_STREAM(eLL_DEBUG_VERBOSE_1));

  // truncate option list
  std::vector<char *> remaining_data;
  std::vector<char *>::iterator end_marker;
  for (end_marker = arguments.begin(); end_marker != arguments.end(); ++end_marker)
  {
    if (std::string(*end_marker) == "--")
    {
      break;
    }
  }
  if (end_marker != arguments.end())
  {
    remaining_data.assign(end_marker + 1, arguments.end());
    arguments.erase(end_marker, arguments.end());
  }

  // parse option list
  for (std::vector<char *>::iterator arg = arguments.begin(); arg != arguments.end(); ++arg)
  {
    RRLIB_LOG_STREAM(eLL_DEBUG_VERBOSE_1) << "Looking at " << *arg;
    if (strncmp(*arg, "--help", 6) == 0)
    {
      PrintHelp(EXIT_SUCCESS);
    }
    if (strncmp(*arg, "--", 2) == 0)
    {
      RRLIB_LOG_STREAM(eLL_DEBUG_VERBOSE_1) << "Long option processing for '" << *arg + 2 << "'";
      char *parameter = *arg + 2;
      char *name = strsep(&parameter, "=");

      RRLIB_LOG_STREAM(eLL_DEBUG) << "Found long option: " << name;

      if (!IsInMap(long_name_to_option_map, name))
      {
        RRLIB_LOG_STREAM(eLL_ERROR) << "Unknown long option with name '" << name << "'!";
        PrintHelp(EXIT_FAILURE);
      }

      if (parameter)
      {
        RRLIB_LOG_STREAM(eLL_DEBUG) << "   with parameter: " << parameter;
      }

      if (!long_name_to_option_map.at(name)->SetValueFromParameter(parameter))
      {
        exit(EXIT_FAILURE);
      }
      continue;
    }
    if ((*arg)[0] == '-')
    {
      RRLIB_LOG_STREAM(eLL_DEBUG_VERBOSE_1) << "Short option processing for '" << *arg + 1 << "'";
      for (size_t i = 1; i < strlen(*arg); ++i)
      {
        char name = (*arg)[i];
        char *parameter = 0;

        RRLIB_LOG_STREAM(eLL_DEBUG) << "Found short option: " << name;

        if (!IsInMap(short_name_to_option_map, name))
        {
          RRLIB_LOG_STREAM(eLL_ERROR) << "Unknown short option with name '" << name << "'!";
          PrintHelp(EXIT_FAILURE);
        }

        if (i < strlen(*arg) - 1)
        {
          if (short_name_to_option_map.at(name)->HasParameter())
          {
            RRLIB_LOG_STREAM(eLL_ERROR) << "Short option '" << name << "' cannot be used within option group '" << *arg + 1 << "' because it needs parameter!";
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          if (short_name_to_option_map.at(name)->HasParameter())
          {
            if (arg + 1 != arguments.end())
            {
              parameter = *(++arg);
              i = strlen(*arg);
              RRLIB_LOG_STREAM(eLL_DEBUG) << "    with parameter: " << parameter;
            }
          }
        }

        if (!short_name_to_option_map.at(name)->SetValueFromParameter(parameter))
        {
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    RRLIB_LOG_STREAM(eLL_ERROR) << "Unhandled argument '" << *arg << "' while expecting an option!";
    exit(EXIT_FAILURE);
  }

  for (tHandlerToNameToOptionMapMap::iterator it = handler_to_name_to_option_map_map.begin(); it != handler_to_name_to_option_map_map.end(); ++it)
  {
    if (!it->first(it->second))
    {
      RRLIB_LOG_STREAM(eLL_DEBUG) << "Option processing failed!";
      exit(EXIT_FAILURE);
    }
  }

  RRLIB_LOG_STREAM(eLL_DEBUG) << "Remaining command line data: " << rrlib::util::Join(remaining_data);
  return remaining_data;
}

//----------------------------------------------------------------------
// ProcessCommandLine
//----------------------------------------------------------------------
std::vector<char *> ProcessCommandLine(int argc, char **argv, const char *version, const char *description)
{
  SetProgramVersion(version);
  SetProgramDescription(description);
  return ProcessCommandLine(argc, argv);
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
