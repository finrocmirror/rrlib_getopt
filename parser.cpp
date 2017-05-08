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
/*!\file    rrlib/getopt/parser.cpp
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

bool &ProcessCommandLineCalled()
{
  static bool value = false;
  return value;
}

//----------------------------------------------------------------------
// PrintHelp
//----------------------------------------------------------------------
void PrintHelp(const std::string &program_description, const std::string &command_line_arguments, const std::string &additional_help_text)
{
  unsigned int padding = 0;
  for (tHandlerToNameToOptionMapMap::iterator it = HandlerToNameToOptionMapMap().begin(); it != HandlerToNameToOptionMapMap().end(); ++it)
  {
    for (tNameToOptionMap::iterator kt = it->second.begin(); kt != it->second.end(); ++kt)
    {
      unsigned int candidate = 0;
      if (kt->second->GetShortName())
      {
        candidate += 2 + (kt->second->ExpectsValue() ? 8 : 0);
        if (kt->second->GetLongName())
        {
          candidate += 2;
        }
      }
      if (kt->second->GetLongName())
      {
        candidate += 2 + strlen(kt->second->GetLongName()) + (kt->second->ExpectsValue() ? 8 : 0);
      }
      padding = std::max(padding, candidate);
    }
  }
  padding += 5;

  if (!program_description.empty())
  {
    RRLIB_LOG_PRINT(USER, program_description, "\n\n");
  }

  std::stringstream output;
  output << "usage: " << ProgramName();
  if (!HandlerToNameToOptionMapMap().empty())
  {
    output << " [options]";
  }
  if (!command_line_arguments.empty())
  {
    output << " " << command_line_arguments;
  }
  RRLIB_LOG_PRINT(USER, output.str());
  if (!HandlerToNameToOptionMapMap().empty())
  {
    RRLIB_LOG_PRINT(USER, "options:");
  }
  for (tHandlerToNameToOptionMapMap::iterator it = HandlerToNameToOptionMapMap().begin(); it != HandlerToNameToOptionMapMap().end(); ++it)
  {
    for (tNameToOptionMap::iterator kt = it->second.begin(); kt != it->second.end(); ++kt)
    {
      std::stringstream option;

      option << "  ";
      if (kt->second->GetShortName())
      {
        option << "-" << kt->second->GetShortName() << (kt->second->ExpectsValue() ? " <value>" : "");
        if (kt->second->GetLongName())
        {
          option << ", ";
        }
      }
      if (kt->second->GetLongName())
      {
        option << "--" << kt->second->GetLongName() << (kt->second->ExpectsValue() ? "=<value>" : "");
      }

      std::stringstream option_line;
      option_line << std::left << std::setw(padding) << option.str();

      const char *help = kt->second->GetHelp();
      size_t help_length = strlen(help);
      for (size_t i = 0; i < help_length; ++i)
      {
        if (help[i] == '\n')
        {
          option_line << std::endl << std::setw(padding) << "";
          continue;
        }
        option_line << help[i];
      }
      RRLIB_LOG_PRINT(USER, option_line.str());
    }
  }
  if (!additional_help_text.empty())
  {
    RRLIB_LOG_PRINT(USER, "\n", additional_help_text);
  }
}

//----------------------------------------------------------------------
// ProcessOption
//----------------------------------------------------------------------
size_t ProcessOption(const tOptionBase &option, std::string &value, size_t argc, char **argv, size_t i)
{
  if (option.ExpectsValue() && value.empty())
  {
    i++;
    RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "   Expecting value which was not seen, yet.");
    if (i < argc && std::string(argv[i]) != "--")
    {
      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "   Looking at argv[", i, "]: ", argv[i]);
      value = argv[i];
    }
  }

  if (!value.empty())
  {
    RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "   Found value: ", value);
  }

  if (!const_cast<tOptionBase &>(option).SetValueFromString(value))
  {
    exit(EXIT_FAILURE);
  }

  return i;
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
const tOption AddValue(const char *long_name, const char short_name, const  char *help, tHandler handler, bool restrict_to_single_occurrence)
{
  return AddOption(tOption(new tValue(long_name, short_name, help, restrict_to_single_occurrence)), handler);
}

//----------------------------------------------------------------------
// ProcessCommandLine
//----------------------------------------------------------------------
std::vector<std::string> ProcessCommandLine(size_t argc, char **argv,
    const std::string &program_description, const std::string &command_line_arguments, const std::string &additional_help_text)
{
  assert(!ProcessCommandLineCalled() && "This method may be called only once!");
  ProcessCommandLineCalled() = true;

  ProgramName() = basename(argv[0]);

  RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, util::Join(argv + 1, argv + argc));

  std::vector<std::string> remaining_arguments;
  for (size_t i = 1; i < argc; ++i)
  {
    RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Looking at argv[", i, "]: ", argv[i]);
    if (std::string(argv[i]) == "--")
    {
      while (++i < argc)
      {
        remaining_arguments.push_back(argv[i]);
      }
      break;
    }

    if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h")
    {
      PrintHelp(program_description, command_line_arguments, additional_help_text);
      exit(EXIT_SUCCESS);
    }

    if (strncmp(argv[i], "--", 2) == 0)
    {
      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Long option processing for '", argv[i] + 2, "'");
      size_t name_length = 2;
      while (name_length < strlen(argv[i]) && argv[i][name_length] != '=')
      {
        name_length++;
      }
      std::string name(argv[i] + 2, name_length - 2);
      name_length = std::min(name_length + 1, strlen(argv[i]));
      std::string value(argv[i] + name_length);

      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Found long option: ", name);

      if (!IsInMap(LongNameToOptionMap(), name))
      {
        RRLIB_LOG_PRINT(ERROR, "Unknown long option with name '", name, "'!");
        exit(EXIT_FAILURE);
      }

      i = ProcessOption(*LongNameToOptionMap().at(name), value, argc, argv, i);
      continue;
    }

    if (argv[i][0] == '-')
    {
      RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Short option processing for '", argv[i] + 1, "'");
      size_t number_of_short_options = strlen(argv[i]) - 1;
      for (size_t k = 0; k < number_of_short_options; ++k)
      {
        char name = argv[i][k + 1];
        std::string value;

        RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Found short option: ", name);

        if (!IsInMap(ShortNameToOptionMap(), name))
        {
          RRLIB_LOG_PRINT(ERROR, "Unknown short option with name '", name, "'!");
          exit(EXIT_FAILURE);
        }

        const tOptionBase &option = *ShortNameToOptionMap().at(name);
        i = ProcessOption(option, value, argc, argv, i);

        if (option.ExpectsValue())
        {
          if (k < number_of_short_options - 1)
          {
            RRLIB_LOG_PRINT(ERROR, "Short option '", name, "' which expects a value can only be used as last element in option group!");
            exit(EXIT_FAILURE);
          }
          break;
        }
      }
      continue;
    }
    remaining_arguments.push_back(argv[i]);
  }

  for (tHandlerToNameToOptionMapMap::iterator it = HandlerToNameToOptionMapMap().begin(); it != HandlerToNameToOptionMapMap().end(); ++it)
  {
    if (!it->first(it->second))
    {
      RRLIB_LOG_PRINT(ERROR, "Option processing failed!");
      exit(EXIT_FAILURE);
    }
  }

  RRLIB_LOG_PRINT(DEBUG_VERBOSE_1, "Remaining command line arguments: ", util::Join(remaining_arguments));
  return remaining_arguments;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
