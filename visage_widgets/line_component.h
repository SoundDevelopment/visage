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

#include "visage_graphics/line.h"
#include "visage_graphics/theme.h"
#include "visage_ui/drawable_component.h"

namespace visage {
  class LineComponent : public DrawableComponent {
  public:
    class BoostBuffer {
    public:
      explicit BoostBuffer(float* values, int num_points) :
          values_(values), num_points_(num_points) { }

      void boostRange(float start, float end, float min);
      void enableBackwardBoost(bool enable) { enable_backward_boost_ = enable; }
      void decayBoosts(float decay);
      bool anyBoostValue() const { return any_boost_value_; }
      const float* values() const { return values_; }
      void setValue(int index, float value) const { values_[index] = value; }

    private:
      float* values_ = nullptr;
      int num_points_ = 0;

      bool enable_backward_boost_ = true;
      bool last_negative_boost_ = false;
      bool any_boost_value_ = false;
    };

    static constexpr int kLineVerticesPerPoint = 6;
    static constexpr int kFillVerticesPerPoint = 2;

    THEME_DEFINE_COLOR(LineColor);
    THEME_DEFINE_COLOR(LineFillColor);
    THEME_DEFINE_COLOR(LineFillColor2);
    THEME_DEFINE_COLOR(LineDisabledColor);
    THEME_DEFINE_COLOR(LineDisabledFillColor);
    THEME_DEFINE_COLOR(CenterPoint);
    THEME_DEFINE_COLOR(GridColor);
    THEME_DEFINE_COLOR(HoverColor);
    THEME_DEFINE_COLOR(DragColor);

    THEME_DEFINE_VALUE(LineWidth);
    THEME_DEFINE_VALUE(LineSizeBoost);
    THEME_DEFINE_VALUE(LineColorBoost);
    THEME_DEFINE_VALUE(LineFillBoost);

    enum FillCenter {
      kCenter,
      kBottom,
      kTop,
      kCustom
    };

    explicit LineComponent(int num_points, bool loop = false);
    ~LineComponent() override;

    void init() override;
    void draw(Canvas& canvas) override;
    void drawLine(Canvas& canvas, unsigned int color_id);
    void drawFill(Canvas& canvas, unsigned int color_id);
    void drawPosition(Canvas& canvas, float x, float y);
    void destroy() override;
    void resized() override;

    float boostAt(int index) const { return boost_.values()[index]; }
    float yAt(int index) const { return line_.y[index]; }
    float xAt(int index) const { return line_.x[index]; }

    void setBoost(int index, float val) {
      VISAGE_ASSERT(index < line_.num_points && index >= 0);
      boost_.setValue(index, val);
      redraw();
    }
    void setYAt(int index, float val) {
      VISAGE_ASSERT(index < line_.num_points && index >= 0);
      line_.y[index] = val;
      redraw();
    }
    void setXAt(int index, float val) {
      VISAGE_ASSERT(index < line_.num_points && index >= 0);
      line_.x[index] = val;
      redraw();
    }

    bool fill() const { return fill_; }

    void setFill(bool fill) { fill_ = fill; }
    void setFillCenter(FillCenter fill_center) { fill_center_ = fill_center; }
    void setFillCenter(float center) {
      custom_fill_center_ = center;
      fill_center_ = kCustom;
    }
    int getFillLocation() const;

    int numPoints() const { return line_.num_points; }
    BoostBuffer& boost() { return boost_; }

    bool active() const { return active_; }
    void setActive(bool active) { active_ = active; }
    void setFillAlphaMult(float mult) { fill_alpha_mult_ = mult; }

  private:
    Line line_;
    BoostBuffer boost_;
    float line_width_ = 1.0f;

    bool fill_ = false;
    FillCenter fill_center_ = kCenter;
    float custom_fill_center_ = 0.0f;
    float fill_alpha_mult_ = 1.0f;

    bool active_ = true;
    bool loop_ = false;

    VISAGE_LEAK_CHECKER(LineComponent)
  };
}