//  SuperTux
//  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <SDL.h>

#include <config.h>
#include <memory>

#include "supertux/main.hpp"

#ifdef MIR_BUILD
//required
#include <QGuiApplication>
#endif
//static std::unique_ptr<Main> g_main;
#ifdef WAYLAND_UT
static Main* g_main;

#include <QGuiApplication>
#endif
int main(int argc, char** argv)
{

#ifdef MIR_BUILD
#define CUSTOM_BUILD
     qputenv("SDL_VIDEODRIVER","");
     qputenv("QT_QPA_PLATFORM","ubuntumirclient");
     QGuiApplication app(argc, argv);
#endif
#ifdef WAYLAND_UT
#define CUSTOM_BUILD
     qputenv("SDL_VIDEODRIVER","wayland");
     QGuiApplication app(argc, argv);
     g_main = new Main();
     g_main->run(argc, argv);
     return 0;
#endif

#ifndef CUSTOM_BUILD
  g_main = std::make_unique<Main>();

  int ret = g_main->run(argc, argv);

#if !defined(__EMSCRIPTEN__)
  // Manually destroy, as atexit() functions are called before global
  // destructors and thus would make the destruction crash.
  g_main.reset();
#endif
  return 0;

#endif
}

/* EOF */
