/* Copyright Vital Audio, LLC
 *
 * visage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * visage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dsp_tools.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdarg>

namespace visage {
  class String;
  void debugLog(const char* file, unsigned int line, const String& log_message, ...);
  void debugLog(const char* file, unsigned int line, const char* format, va_list arg_list);
  void debugLog(const char* file, unsigned int line, const char* format, ...);
  void debugLog(const char* file, unsigned int line, long long log_message, ...);
  void debugLog(const char* file, unsigned int line, unsigned long long log_message, ...);
  void debugLog(const char* file, unsigned int line, int log_message, ...);
  void debugLog(const char* file, unsigned int line, unsigned int log_message, ...);
  void debugLog(const char* file, unsigned int line, float log_message, ...);
  void debugLog(const char* file, unsigned int line, double log_message, ...);
  void debugLog(const char* file, unsigned int line, char log_message, ...);
  void debugAssert(bool condition);
  void forceCrash();
}

#define VISAGE_FORCE_CRASH() visage::forceCrash()

#ifndef NDEBUG

#define VISAGE_LOG(log, ...) visage::debugLog(__FILE__, int(__LINE__), log, ##__VA_ARGS__)

#define VISAGE_ASSERT(condition) visage::debugAssert((condition))
#define no_except

namespace visage {
  template<class T>
  class InstanceCounter {
  public:
    static InstanceCounter<T>& getInstance() {
      static InstanceCounter<T> instance;
      return instance;
    }

    ~InstanceCounter() { VISAGE_ASSERT(count_ == 0); }

    void add() { count_++; }
    void remove() { count_--; }

  private:
    int count_ = 0;
  };

  template<class T>
  class LeakChecker {
  public:
    LeakChecker() { InstanceCounter<T>::getInstance().add(); }

    LeakChecker(const LeakChecker& other) { InstanceCounter<T>::getInstance().add(); }

    ~LeakChecker() { InstanceCounter<T>::getInstance().remove(); }
  };
}

#define VISAGE_LEAK_CHECKER(className)             \
  friend class visage::InstanceCounter<className>; \
  static const char* vaLeakCheckerName() {         \
    return #className;                             \
  }                                                \
  visage::LeakChecker<className> leakChecker;

#else
#define VISAGE_ASSERT(x) ((void)0)
#define VISAGE_LOG(x) ((void)0)
#define VISAGE_LEAK_CHECKER(className)
#define no_except noexcept
#endif

#if VISAGE_WINDOWS
#define VISAGE_STDCALL __stdcall
#else
#define VISAGE_STDCALL
#endif