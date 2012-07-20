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

#include "rrlib/logging/messages.h"

#include "rrlib/util/join.h"

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
typedef std::map<std::string, tOption> tLongNameToOptionMap;
typedef std::map<char, tOption> tShortNameToOptionMap;
typedef std::map<tHandler, tNameToOptionMap> tHandlerToNameToOptionMapMap;

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace
{

//----------------------------------------------------------------------
// GetOptions
//----------------------------------------------------------------------
tHandlerToNameToOptionMapMap &HandlerToNameToOptionMapMap()
{
  static tHandlerToNameToOptionMapMap map;
  return map;
}

//----------------------------------------------------------------------
// GetLongNameToOptionMap
//----------------------------------------------------------------------
tLongNameToOptionMap &LongNameToOptionMap()
{
  static tLongNameToOptionMap map;
  return map;
}

//----------------------------------------------------------------------
// GetShortNameToOptionMap
//----------------------------------------------------------------------
tShortNameToOptionMap &ShortNameToOptionMap()
{
  static tShortNameToOptionMap map;
  return map;
}

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
// AddOption
//----------------------------------------------------------------------
static const tOption AddOption(const tOption &option, const tHandler handler)
{
  RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "long_name = ", option->GetLongName(), ", short_name = ", option->GetShortName(), ", help = ", option->GetHelp());
  assert(handler);

  if (option->GetLongName())
  {
    if (IsInMap(LongNameToOptionMap(), option->GetLongName()))
    {
      RRLIB_LOG_PRINT(ERROR, "Option '", option->GetLongName(), "' already exists with description '", LongNameToOptionMap().at(option->GetLongName())->GetHelp(), "'");
      return tOption();
    }
    LongNameToOptionMap()[option->GetLongName()] = option;
  }
  if (option->GetShortName())
  {
    if (IsInMap(ShortNameToOptionMap(), option->GetShortName()))
    {
      RRLIB_LOG_PRINT(ERROR, "Option '", option->GetShortName(), "' already exists with description '", ShortNameToOptionMap().at(option->GetShortName())->GetHelp(), "'");
      return tOption();
    }
    ShortNameToOptionMap()[option->GetShortName()] = option;
  }

  HandlerToNameToOptionMapMap()[handler][option->GetName()] = option;
  return option;
}

const char *&ProgramName()
{
  static const char *value = 0;
  return value;
}

const char *&ProgramVersion()
{
  static const char *value = "<Program version not defined>";
  return value;
}

const char *&ProgramDescription()
{
  static const char *value = "<Program description not defined>";
  return value;
}

bool &ProcessCommandLineCalled()
{
  static bool value = false;
  return value;
}

//----------------------------------------------------------------------
// PrintHelp
//----------------------------------------------------------------------
void PrintHelp(int return_code)
{
  unsigned int max_long_name_length = 0;
  for (tLongNameToOptionMap::iterator it = LongNameToOptionMap().begin(); it != LongNameToOptionMap().end(); ++it)
  {
    unsigned int long_name_length = it->first.length() + (it->second->HasParameter() ? 8 : 0);
    max_long_name_length = std::max(max_long_name_length, long_name_length);
  }
  unsigned int max_short_name_length = 1;
  for (tShortNameToOptionMap::iterator it = ShortNameToOptionMap().begin(); it != ShortNameToOptionMap().end(); ++it)
  {
    if (it->second->HasParameter())
    {
      max_short_name_length = 9;
      break;
    }
  }

  if (return_code == EXIT_SUCCESS && ProgramVersion())
  {
    RRLIB_LOG_PRINT(USER, ProgramName(), " ", ProgramVersion(), "\n\n");
  }
  if (return_code == EXIT_SUCCESS && ProgramDescription())
  {
    RRLIB_LOG_PRINT(USER, ProgramDescription(), "\n\n");
  }
  if (HandlerToNameToOptionMapMap().empty())
  {
    RRLIB_LOG_PRINT(USER, "Usage: ", ProgramName(), "\n\n");
  }
  else
  {
    RRLIB_LOG_PRINT(USER, "Usage: ", ProgramName(), " <OPTIONS> ", "\n\n", "Possible options are:\n\n");
  }
  for (tHandlerToNameToOptionMapMap::iterator it = HandlerToNameToOptionMapMap().begin(); it != HandlerToNameToOptionMapMap().end(); ++it)
  {
    for (tNameToOptionMap::iterator kt = it->second.begin(); kt != it->second.end(); ++kt)
    {
      std::string long_name = "";
      if (kt->second->GetLongName())
      {
        long_name += std::string("--") + kt->second->GetLongName() + (kt->second->HasParameter() ? "=<value>" : "") + (kt->second->GetShortName() ? "," : "");
      }
      std::string short_name = "";
      if (kt->second->GetShortName())
      {
        short_name += std::string("-") + kt->second->GetShortName() + (kt->second->HasParameter() ? " <value>" : "");
      }
      std::stringstream option_line;
      option_line << " " << std::left << std::setw(max_long_name_length + 5) << long_name << std::setw(max_short_name_length + 1) << short_name << "    ";

      const char *help = kt->second->GetHelp();
      size_t help_length = strlen(help);
      for (size_t i = 0; i < help_length; ++i)
      {
        if (help[i] == '\n')
        {
          option_line << "\n" << std::setw(max_long_name_length + max_short_name_length + 11) << "";
          continue;
        }
        option_line << help[i];
      }
      RRLIB_LOG_PRINT(USER, option_line.str());
    }
  }

  exit(return_code);
}

}



//----------------------------------------------------------------------
// AddFlag
//----------------------------------------------------------------------
const tOption AddFlag(const char *long_name, const char short_name, const  char *help, tHandler handler)
{
  return AddOption(tOption(new tFlag(long_name, short_name, help)), handler);
}

//----------------------------------------------------------------------
// AddCounter
//----------------------------------------------------------------------
const tOption AddCounter(const char *long_name, const char short_name, const  char *help, tHandler handler)
{
  return AddOption(tOption(new tCounter(long_name, short_name, help)), handler);
}

//----------------------------------------------------------------------
// AddValue
//----------------------------------------------------------------------
const tOption AddValue(const char *long_name, const char short_name, const  char *help, tHandler handler)
{
  return AddOption(tOption(new tValue(long_name, short_name, help)), handler);
}

//----------------------------------------------------------------------
// SetProgramVersion
//----------------------------------------------------------------------
void SetProgramVersion(const char *version)
{
  ProgramVersion() = version;
}

//----------------------------------------------------------------------
// SetProgramDescription
//----------------------------------------------------------------------
void SetProgramDescription(const char *description)
{
  ProgramDescription() = description;
}

//----------------------------------------------------------------------
// ProcessCommandLine
//----------------------------------------------------------------------
std::vector<char *> ProcessCommandLine(int argc, char **argv)
{
  assert(!ProcessCommandLineCalled() && "This method may be called only once!");
  ProcessCommandLineCalled() = true;

  ProgramName() = basename(argv[0]);

  // create option list
  std::vector<char *> arguments(argv + 1, argv + argc);
  RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, util::Join(arguments));

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
    RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Looking at ", *arg);
    if (strncmp(*arg, "--help", 6) == 0)
    {
      PrintHelp(EXIT_SUCCESS);
    }
    if (strncmp(*arg, "--", 2) == 0)
    {
      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Long option processing for '", *arg + 2, "'");
      char *parameter = *arg + 2;
      char *name = strsep(&parameter, "=");

      RRLIB_LOG_PRINT(DEBUG, "Found long option: ", name);

      if (!IsInMap(LongNameToOptionMap(), name))
      {
        RRLIB_LOG_PRINT(ERROR, "Unknown long option with name '", name, "'!");
        PrintHelp(EXIT_FAILURE);
      }

      if (parameter)
      {
        RRLIB_LOG_PRINT(DEBUG, "   with parameter: ", parameter);
      }

      if (!const_cast<tOptionBase *>(LongNameToOptionMap().at(name).get())->SetValueFromParameter(parameter))
      {
        exit(EXIT_FAILURE);
      }
      continue;
    }
    if ((*arg)[0] == '-')
    {
      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Short option processing for '", *arg + 1, "'");
      for (size_t i = 1; i < strlen(*arg); ++i)
      {
        char name = (*arg)[i];
        char *parameter = 0;

        RRLIB_LOG_PRINT(DEBUG, "Found short option: ", name);

        if (!IsInMap(ShortNameToOptionMap(), name))
        {
          RRLIB_LOG_PRINT(ERROR, "Unknown short option with name '", name, "'!");
          PrintHelp(EXIT_FAILURE);
        }

        if (i < strlen(*arg) - 1)
        {
          if (ShortNameToOptionMap().at(name)->HasParameter())
          {
            RRLIB_LOG_PRINT(ERROR, "Short option '", name, "' cannot be used within option group '", *arg + 1, "' because it needs parameter!");
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          if (ShortNameToOptionMap().at(name)->HasParameter())
          {
            if (arg + 1 != arguments.end())
            {
              parameter = *(++arg);
              i = strlen(*arg);
              RRLIB_LOG_PRINT(DEBUG, "    with parameter: ", parameter);
            }
          }
        }

        if (!const_cast<tOptionBase *>(ShortNameToOptionMap().at(name).get())->SetValueFromParameter(parameter))
        {
          exit(EXIT_FAILURE);
        }
      }
      continue;
    }

    RRLIB_LOG_PRINT(ERROR, "Unhandled argument '", *arg, "' while expecting an option!");
    exit(EXIT_FAILURE);
  }

  for (tHandlerToNameToOptionMapMap::iterator it = HandlerToNameToOptionMapMap().begin(); it != HandlerToNameToOptionMapMap().end(); ++it)
  {
    if (!it->first(it->second))
    {
      RRLIB_LOG_PRINT(DEBUG, "Option processing failed!");
      exit(EXIT_FAILURE);
    }
  }

  RRLIB_LOG_PRINT(DEBUG, "Remaining command line data: ", util::Join(remaining_data));
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
