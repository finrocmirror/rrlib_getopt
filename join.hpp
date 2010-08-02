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
/*!\file    join.hpp
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
#include <iterator>
#include <sstream>
#include <algorithm>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace
{
template <typename TBidirectionalIterator>
inline void __Join(TBidirectionalIterator first, TBidirectionalIterator last, std::ostream &destination, const char *delimiter, std::bidirectional_iterator_tag)
{
  assert(first != last);
  std::copy(first, --last, std::ostream_iterator<typename std::iterator_traits<TBidirectionalIterator>::value_type>(destination, delimiter));
  destination << *last;
}

template <class TInputIterator>
inline void __Join(TInputIterator first, TInputIterator last, std::ostream &destination, const char *delimiter, std::input_iterator_tag)
{
  assert(first != last);
  destination << *first++;
  for (TInputIterator it = first; it != last; ++it)
  {
    destination << delimiter << *it;
  }
}

template <class TInputIterator>
inline std::string __Join(TInputIterator first, TInputIterator last, const char *delimiter)
{
  assert(first != last);
  std::stringstream result;
  Join(first, last, result, delimiter);
  return result.str();
}
}

template <typename TIterator>
inline void Join(TIterator first, TIterator last, std::ostream &destination, const char *delimiter = ", ")
{
  if (first != last)
  {
    __Join(first, last, destination, delimiter, typename std::iterator_traits<TIterator>::iterator_category());
  }
}

template <typename TContainer>
inline void Join(TContainer container, std::ostream &destination, const char *delimiter = ", ")
{
  Join(container.begin(), container.end(), destination, delimiter);
}

template <typename TIterator>
inline const std::string Join(TIterator first, TIterator last, const char *delimiter = ", ")
{
  if (first != last)
  {
    return __Join(first, last, delimiter);
  }
  return "";
}

template <typename TContainer>
inline const std::string Join(const TContainer &container, const char *delimiter = ", ")
{
  return Join(container.begin(), container.end(), delimiter);
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
