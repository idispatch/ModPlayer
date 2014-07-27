/***************************************************************************
    copyright            : (C) 2002 - 2008 by Scott Wheeler
    email                : wheeler@kde.org
 ***************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License version   *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 *                                                                         *
 *   Alternatively, this file is available under the Mozilla Public        *
 *   License Version 1.1.  You may obtain a copy of the License at         *
 *   http://www.mozilla.org/MPL/                                           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
//#include <config.h>
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#ifndef TAGLIB_DEBUG_H
#include <tdebug.h>
#endif

#ifndef TAGLIB_STRING_H
#include <tstring.h>
#endif

#ifndef TAGLIB_DEBUGLISTENER_H
#include <tdebuglistener.h>
#endif

#include <bitset>
#include <cstdio>
#include <stdarg.h>

using namespace TagLib;

namespace
{
  String format(const char *fmt, ...)
  {
    va_list args;
    va_start(args, fmt);

    char buf[256];

#if defined(HAVE_SNPRINTF)

    vsnprintf(buf, sizeof(buf), fmt, args);

#elif defined(HAVE_SPRINTF_S)

    vsprintf_s(buf, fmt, args);

#else

    // Be careful. May cause a buffer overflow.
    std::vsprintf(buf, fmt, args);

#endif

    va_end(args);

    return String(buf);
  }
}

namespace TagLib
{
  // The instance is defined in tdebuglistener.cpp.
  extern DebugListener *debugListener;

  void debug(const String &s)
  {
#if !defined(NDEBUG) || defined(TRACE_IN_RELEASE)

    debugListener->printMessage("TagLib: " + s + "\n");
#endif
  }

  void debugData(const ByteVector &v)
  {
#if 0 && !defined(NDEBUG) || defined(TRACE_IN_RELEASE)

    for(size_t i = 0; i < v.size(); ++i)
    {
      std::string bits = std::bitset<8>(v[i]).to_string();
      String msg = format("*** [%d] - char '%c' - int %d, 0x%02x, 0b%s\n",
        i, v[i], v[i], v[i], bits.c_str());

      debugListener->printMessage(msg);
    }
#endif
  }
}
