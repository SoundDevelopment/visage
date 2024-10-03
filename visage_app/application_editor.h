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

#include "visage_ui/drawable_component.h"

namespace visage {
  class Canvas;
  class Window;
  class WindowEventHandler;

  class ApplicationEditor : public DrawableComponent {
  public:
    ApplicationEditor();
    ~ApplicationEditor() override;

    void resized() final;
    virtual void editorResized() { }

    virtual bool loadPresetFile(const std::string& preset) { return false; }
    virtual void loadInitPreset() { }
    virtual void setAsDefaultPreset() { }
    virtual void saveTheme() { }
    virtual void showThemeEditor(int editor) { }
    virtual bool loadTheme(const std::string& theme_path) { return false; }
    virtual std::string browseForTheme() { return ""; }
    virtual void loadDefaultTheme() { }
    void addToWindow(Window* handle);
    void removeFromWindow();

    void drawWindow();

    virtual int getDefaultWidth() const { return 800; }
    virtual int getDefaultHeight() const { return 600; }
    float getDefaultAspectRatio() const { return getDefaultWidth() * 1.0f / getDefaultHeight(); }
    float getWidthScale() override { return getWidth() * 1.0f / getDefaultWidth(); }
    float getHeightScale() override { return getHeight() * 1.0f / getDefaultHeight(); }
    float getDpiScale() override;

    void requestKeyboardFocus(UiFrame* frame) override;
    void setCursorStyle(MouseCursor style) override;
    void setCursorVisible(bool visible) override;
    std::string getClipboardText() override;
    void setClipboardText(const std::string& text) override;
    void setMouseRelativeMode(bool relative) override;

    bool requestRedraw(DrawableComponent* component) override;

    bool isFixedAspectRatio() const { return fixed_aspect_ratio_; }
    void setFixedAspectRatio(bool fixed) { fixed_aspect_ratio_ = fixed; }

    virtual void applicationStateLoaded() { }
    Window* window() const { return window_; }

  private:
    std::set<DrawableComponent*> stale_children_;
    std::set<DrawableComponent*> drawing_children_;

    Window* window_ = nullptr;
    std::unique_ptr<Canvas> canvas_;
    std::unique_ptr<WindowEventHandler> window_event_handler_;
    bool fixed_aspect_ratio_ = false;

    VISAGE_LEAK_CHECKER(ApplicationEditor)
  };

  class WindowedEditor : public ApplicationEditor {
  public:
    ~WindowedEditor() override;

    void setTitle(std::string title) { title_ = std::move(title); }

    void show(float window_scale);
    void show(int x, int y, int width, int height);
    void show(int width, int height);

  private:
    void showWindow();

    std::string title_;
    std::unique_ptr<Window> window_;
  };
}
