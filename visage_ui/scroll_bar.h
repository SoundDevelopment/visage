/* Copyright Vital Audio, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "frame.h"
#include "visage_graphics/animation.h"

namespace visage {
  class ScrollBar : public Frame {
  public:
    ScrollBar() :
        color_(Animation<float>::kRegularTime, Animation<float>::kEaseOut, Animation<float>::kEaseOut),
        width_(Animation<float>::kRegularTime, Animation<float>::kEaseOut, Animation<float>::kEaseOut) {
      color_.setTargetValue(1.0f);
    }

    void draw(Canvas& canvas) override;
    void resized() override {
      Frame::resized();
      width_.setSourceValue(width() / 2);
      width_.setTargetValue(width());
    }

    void mouseEnter(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

    void addScrollCallback(std::function<void(int)> callback) {
      callbacks_.push_back(std::move(callback));
    }

    void setRounding(float rounding) {
      rounding_ = rounding;
      redraw();
    }

    void setPosition(int position) {
      position_ = position;
      redraw();
    }

    void setViewPosition(int range, int view_height, int position) {
      range_ = range;
      view_height_ = view_height;
      position_ = position;

      active_ = view_height_ < range_;
      setIgnoresMouseEvents(!active_, true);
      redraw();
    }

    int viewRange() const { return range_; }
    int viewHeight() const { return view_height_; }

    void setLeftSide(bool left) { left_ = left; }

  private:
    std::vector<std::function<void(int)>> callbacks_;

    Animation<float> color_;
    Animation<float> width_;

    int last_drag_ = 0;

    bool active_ = false;
    bool left_ = false;
    int range_ = 0.0f;
    int view_height_ = 0.0f;
    float position_ = 0.0f;
    float rounding_ = 0.0f;

    VISAGE_LEAK_CHECKER(ScrollBar)
  };

  class ScrollableFrame : public Frame {
  public:
    explicit ScrollableFrame(const std::string& name = "") : Frame(name) {
      addChild(&container_);
      container_.setIgnoresMouseEvents(true, true);
      container_.setVisible(false);

      addChild(&scroll_bar_);
      scroll_bar_.addScrollCallback([this](int position) { scrollPositionChanged(position); });
      scroll_bar_.setOnTop(true);
    }

    void resized() override;

    void addScrolledChild(Frame* frame, bool make_visible = true) {
      container_.setVisible(true);
      container_.addChild(frame);
      frame->setVisible(make_visible);
    }

    void scrollPositionChanged(int position) {
      setYPosition(position);
      on_scroll_.callback(this);
    }

    bool scrollUp() {
      setYPosition(std::max(0, y_position_ - height() / 8));
      return true;
    }

    bool scrollDown() {
      setYPosition(y_position_ + height() / 8);
      return true;
    }

    void setScrollBarRounding(float rounding) { scroll_bar_.setRounding(rounding); }
    int scrollableHeight() const { return container_.height(); }
    void setScrollableHeight(int total_height, int view_height = 0) {
      if (view_height == 0)
        view_height = height();
      container_.setBounds(0, -y_position_, width(), total_height);
      setYPosition(std::max(0, std::min(y_position_, total_height - view_height)));
      scroll_bar_.setViewPosition(total_height, view_height, y_position_);
    }

    void setScrollBarBounds(int x, int y, int width, int height) {
      scroll_bar_.setBounds(x, y, width, height);
    }

    void setYPosition(float position) {
      float_position_ = position;
      y_position_ = position;
      container_.setTopLeft(container_.x(), -y_position_);
      scroll_bar_.setPosition(position);
      redraw();
      container_.redraw();
    }
    int yPosition() const { return y_position_; }

    bool mouseWheel(const MouseEvent& e) override {
      static constexpr float kScale = 30.0f;

      float max = scroll_bar_.viewRange() - scroll_bar_.viewHeight();
      if (max <= 0)
        return false;

      float y = float_position_ - kScale * e.precise_wheel_delta_y * heightScale();
      y = std::min(max, y);
      y = std::max(0.0f, y);
      scrollPositionChanged(y);
      scroll_bar_.setViewPosition(scroll_bar_.viewRange(), scroll_bar_.viewHeight(), y);
      return true;
    }

    void setScrollBarLeft(bool left) {
      scroll_bar_left_ = left;
      scroll_bar_.setLeftSide(left);
    }

    Layout& scrollableLayout() { return container_.layout(); }

    auto& onScroll() { return on_scroll_; }
    ScrollBar& scrollBar() { return scroll_bar_; }

  private:
    CallbackList<void(ScrollableFrame*)> on_scroll_;
    float float_position_ = 0.0f;
    int y_position_ = 0;
    bool scroll_bar_left_ = false;
    Frame container_;
    ScrollBar scroll_bar_;

    VISAGE_LEAK_CHECKER(ScrollableFrame)
  };
}