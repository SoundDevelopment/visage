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
 * along with visage.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "visage_utils/time_utils.h"

namespace visage {
  template<class T>
  class Animation {
  public:
    enum EasingFunction {
      kLinear,
      kEaseIn,
      kEaseOut,
      kEaseInOut,
    };

    static constexpr int kSlowTime = 240;
    static constexpr int kRegularTime = 80;
    static constexpr int kFastTime = 50;

    static inline T interpolate(const T& from, const T& to, float t) {
      return from + (to - from) * t;
    }

    static inline float sin1(float phase) {
      phase = 0.5f - phase;
      const float phase2 = phase * phase;
      const float phase4 = phase2 * phase2;
      const float coefficient4 = phase4 * 12.228473185021549602f - phase2 * 38.12119956657129365f +
                                 67.04364396354298358f;
      const float stage = coefficient4 * phase4 - phase2 * 64.834670562974805234f + 25.13273028802431777f;
      return stage * phase * (0.25f - phase2);
    }

    static T ease(const T& from, const T& to, float t, EasingFunction easing) {
      switch (easing) {
      case kEaseIn: return interpolate(from, to, 1.0f - sin1(0.25f * (1.0f - t)));
      case kEaseOut: return interpolate(from, to, sin1(0.25f * t));
      case kEaseInOut: return interpolate(from, to, sin1(0.5f * t - 0.25f) * 0.5f + 0.5f);
      case kLinear:
      default: return interpolate(from, to, t);
      }
    }

    Animation() : Animation(kRegularTime, kEaseIn, kEaseOut) { }

    explicit Animation(int milliseconds, EasingFunction forward_easing = kLinear,
                       EasingFunction backward_easing = kLinear) :
        source_(), target_(), time_(milliseconds), forward_easing_(forward_easing),
        backward_easing_(backward_easing) { }

    Animation(T* value, int milliseconds, EasingFunction forward_easing = kLinear,
              EasingFunction backward_easing = kLinear) :
        value_(value), source_(), target_(), time_(milliseconds), forward_easing_(forward_easing),
        backward_easing_(backward_easing) { }

    Animation(T* value, T source, T target, int milliseconds,
              EasingFunction forward_easing = kLinear, EasingFunction backward_easing = kLinear) :
        value_(value), source_(source), target_(target), time_(milliseconds),
        forward_easing_(forward_easing), backward_easing_(backward_easing) { }

    void target(bool target, bool jump = false) {
      last_ms_ = time::getMilliseconds();

      targeting_ = target;
      if (jump)
        t_ = target ? 1.0f : 0.0f;
    }
    bool isTargeting() const { return targeting_; }
    bool isAnimating() const { return targeting_ ? t_ < 1.0f : t_ > 0.0f; }
    void setSourceValue(T value) { source_ = value; }
    void setTargetValue(T value) { target_ = value; }
    T setSourceValue() const { return source_; }
    T setTargetValue() const { return target_; }
    void setAnimationTime(int milliseconds) { time_ = milliseconds; }

    T value() const {
      float t = t_;
      EasingFunction easing = forward_easing_;
      const T* from = &source_;
      const T* to = &target_;

      if (!targeting_) {
        easing = backward_easing_;
        from = &target_;
        to = &source_;
        t = 1.0f - t_;
      }

      if (value_)
        return *value_ = ease(*from, *to, t, easing);
      return ease(*from, *to, t, easing);
    }

    T update() {
      long long ms = time::getMilliseconds();
      float delta = (ms - last_ms_) / time_;
      last_ms_ = ms;

      if (targeting_)
        t_ = std::min(t_ + delta, 1.0f);
      else
        t_ = std::max(t_ - delta, 0.0f);

      return value();
    }

  private:
    T* value_ = nullptr;
    T source_;
    T target_;
    float time_ = kRegularTime;
    long long last_ms_ = 0;

    EasingFunction forward_easing_ = kLinear;
    EasingFunction backward_easing_ = kLinear;

    bool targeting_ = false;
    float t_ = 0.0f;
  };
}