/// This file is part of flutter_native_view
/// (https://github.com/alexmercerind/flutter_native_view).
///
/// Copyright (C) 2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
///
/// flutter_native_view is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by the
/// Free Software Foundation, either version 3 of the License, or (at your
/// option) any later version.
///
/// flutter_native_view is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
/// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///
/// You should have received a copy of the GNU General Public License along with
/// flutter_native_view. If not, see <https://www.gnu.org/licenses/>.
///

#include "native_view_core.h"

namespace flutternativeview {

NativeViewCore* NativeViewCore::GetInstance() { return instance_.get(); }

void NativeViewCore::SetInstance(std::unique_ptr<NativeViewCore> instance) {
  instance_ = std::move(instance);
}

std::optional<int32_t> NativeViewCore::GetProcId() { return proc_id_; }

void NativeViewCore::SetProcId(int32_t proc_id) { proc_id_ = proc_id; }

NativeViewCore::NativeViewCore(HWND window, HWND child_window)
    : window_(window), child_window_(child_window) {}

void NativeViewCore::EnsureInitialized(COLORREF layered_color) {
  SetWindowComposition(window_, 6, 0);
  TITLEBARINFOEX title_bar_info;
  title_bar_info.cbSize = sizeof(TITLEBARINFOEX);
  ::SendMessage(window_, WM_GETTITLEBARINFOEX, 0, (LPARAM)&title_bar_info);
  title_bar_height_ =
      title_bar_info.rcTitleBar.bottom - title_bar_info.rcTitleBar.top;
  native_view_container_ = flutternativeview::GetNativeViewContainer(window_);
}

void NativeViewCore::UpdateLayeredColor(COLORREF layered_color) {
  ::SetLayeredWindowAttributes(window_, layered_color, 0, LWA_COLORKEY);
}

void NativeViewCore::CreateNativeView(HWND native_view, RECT rect,
                                      double device_pixel_ratio) {
  ::SetParent(native_view, native_view_container_);
  auto style = ::GetWindowLongPtr(native_view, GWL_STYLE);
  style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
             WS_EX_APPWINDOW);
  ::SetWindowLongPtr(native_view, GWL_STYLE, style);
  device_pixel_ratio_ = device_pixel_ratio;
  native_views_[native_view] = rect;
  // Position the |native_view| at the correct position behind the |window_|
  // (parent).
  auto global_rect =
      GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom);
  ::SetWindowPos(native_view, window_, global_rect.left, global_rect.top,
                 global_rect.right - global_rect.left,
                 global_rect.bottom - global_rect.top, SWP_NOACTIVATE);
}

void NativeViewCore::DisposeNativeView(HWND native_view) {
  ::SendMessage(native_view, WM_CLOSE, NULL, NULL);
  native_views_.erase(native_view);
}

void NativeViewCore::ResizeNativeView(HWND native_view, RECT rect) {
  native_views_[native_view] = rect;
  auto global_rect =
      GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom);
  // Move the |native_view|, since this happens when the size of the viewport
  // is likely changed, thus redraw is required.
  ::MoveWindow(native_view, global_rect.left, global_rect.top,
               global_rect.right - global_rect.left,
               global_rect.bottom - global_rect.top, TRUE);
}

std::optional<HRESULT> NativeViewCore::WindowProc(HWND hwnd, UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam) {
  switch (message) {
    case WM_ACTIVATE: {
      for (const auto& [native_view, rect] : native_views_) {
        RECT window_rect;
        ::GetWindowRect(window_, &window_rect);
        // Position |native_view| such that it's z order is behind |window_|.
        ::SetWindowPos(native_view_container_, window_, window_rect.left,
                       window_rect.top, window_rect.right - window_rect.left,
                       window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
      }
      break;
    }
    case WM_SIZE:
    case WM_MOVE:
    case WM_MOVING:
    case WM_SYSCOMMAND: {
      RECT window_rect;
      ::GetWindowRect(window_, &window_rect);
      ::SetWindowPos(native_view_container_, window_, window_rect.left,
                     window_rect.top, window_rect.right - window_rect.left,
                     window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
      for (const auto& [native_view, rect] : native_views_) {
        auto global_rect =
            GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom);
        // Position the |native_view| at new position if the window was moved.
        // No redraw is required.
        ::MoveWindow(native_view, global_rect.left, global_rect.top,
                     global_rect.right - global_rect.left,
                     global_rect.bottom - global_rect.top, TRUE);
      }
      break;
    }
    case WM_CLOSE: {
      for (const auto& [native_view, rect] : native_views_) {
        ::SendMessage(native_view, WM_CLOSE, NULL, NULL);
      }
      native_views_.clear();
      break;
    }
    default:
      break;
  }
  return std::nullopt;
}

RECT NativeViewCore::GetGlobalRect(int32_t left, int32_t top, int32_t right,
                                   int32_t bottom) {
  // Expanding the client area so that window boundaries don't leave empty
  // transparent airspace.
  left -= static_cast<int32_t>(ceil(device_pixel_ratio_));
  top -= static_cast<int32_t>(ceil(device_pixel_ratio_));
  right += static_cast<int32_t>(ceil(device_pixel_ratio_));
  bottom += static_cast<int32_t>(ceil(device_pixel_ratio_));
  RECT window_rect;
  ::GetClientRect(window_, &window_rect);
  RECT rect;
  rect.left = window_rect.left + left;
  rect.top = window_rect.top + top;
  rect.right = window_rect.left + right;
  rect.bottom = window_rect.top + bottom;
  return rect;
}

NativeViewCore::~NativeViewCore() {
  // Destroy existing |native_views_|.
  for (const auto& [native_view, rect] : native_views_) {
    ::SendMessage(native_view, WM_CLOSE, NULL, NULL);
  }
  native_views_.clear();
}

std::unique_ptr<NativeViewCore> NativeViewCore::instance_ = nullptr;

std::optional<int32_t> NativeViewCore::proc_id_ = std::nullopt;

}  // namespace flutternativeview
