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

#include <cmath>

NativeViewCore::NativeViewCore(HWND window) : window_(window) {}

int NativeViewCore::EnsureInitialized(COLORREF layered_color) {
  // Use |SetWindowCompositionAttribute| API on Windows versions higher or equal
  // to 1903.
  if (GetWindowsVersion().dwBuildNumber > 18362) {
    SetWindowComposition(window_, 2, 0);
    return 0;
  } else {
    // Use |WS_EX_LAYERED| with layered color defined by |layered_color| on
    // older Windows versions.
    auto style = ::GetWindowLongPtr(window_, GWL_EXSTYLE);
    ::SetWindowLongPtr(window_, GWL_EXSTYLE, style | WS_EX_LAYERED);
    ::SetLayeredWindowAttributes(window_, layered_color, 0, LWA_COLORKEY);
    return 1;
  }
}

void NativeViewCore::UpdateLayeredColor(COLORREF layered_color) {
  ::SetLayeredWindowAttributes(window_, layered_color, 0, LWA_COLORKEY);
}

void NativeViewCore::CreateNativeView(HWND window, int32_t left, int32_t top,
                                      int32_t right, int32_t bottom,
                                      double device_pixel_ratio) {
  auto style = ::GetWindowLongPtr(window, GWL_STYLE);
  // TODO: Remove taskbar entry of the |window|.
  style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
             WS_EX_APPWINDOW);
  ::SetWindowLongPtr(window, GWL_STYLE, style);
  device_pixel_ratio_ = device_pixel_ratio;
  RECT rect;
  rect.left = left;
  rect.top = top;
  rect.right = right;
  rect.bottom = bottom;
  native_views_[window] = rect;
  // Position the |window| at the correct position behind the |window_|
  // (parent).
  auto global_rect = GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom,
                                   device_pixel_ratio_);
  ::SetWindowPos(window, window_, global_rect.left, global_rect.top,
                 global_rect.right - global_rect.left,
                 global_rect.bottom - global_rect.top, SWP_NOACTIVATE);
}

std::optional<HRESULT> NativeViewCore::WindowProc(HWND hwnd, UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam) {
  switch (message) {
    case WM_ACTIVATE:
      for (const auto& [native_view, rect] : native_views_) {
        auto global_rect = GetGlobalRect(rect.left, rect.top, rect.right,
                                         rect.bottom, device_pixel_ratio_);
        // Position |native_view| such that it's z order is behind |window_|.
        ::SetWindowPos(native_view, window_, global_rect.left, global_rect.top,
                       global_rect.right - global_rect.left,
                       global_rect.bottom - global_rect.top, SWP_NOACTIVATE);
      }
      break;
    case WM_SIZE:
    case WM_MOVE:
    case WM_MOVING:
    case WM_WINDOWPOSCHANGED:
      for (const auto& [native_view, rect] : native_views_) {
        auto global_rect = GetGlobalRect(rect.left, rect.top, rect.right,
                                         rect.bottom, device_pixel_ratio_);
        // Position the |native_view| at new position if the window was moved.
        // No redraw is required.
        ::MoveWindow(native_view, global_rect.left, global_rect.top,
                     global_rect.right - global_rect.left,
                     global_rect.bottom - global_rect.top, FALSE);
      }
      break;
    default:
      break;
  }
  return std::nullopt;
}

RECT NativeViewCore::GetGlobalRect(int32_t left, int32_t top, int32_t right,
                                   int32_t bottom, double device_pixel_ratio) {
  // Expanding the client area so that window boundaries don't leave empty
  // transparent airspace.
  left -= static_cast<int32_t>(ceil(device_pixel_ratio));
  top -= static_cast<int32_t>(ceil(device_pixel_ratio));
  // Since the left & right have been reduced by 1 pixels, increasing the widths
  // by 2 so as to cover the space correctly.
  right += 2 * static_cast<int32_t>(ceil(device_pixel_ratio));
  bottom += 2 * static_cast<int32_t>(ceil(device_pixel_ratio));
  RECT window_rect;
  ::GetWindowRect(window_, &window_rect);
  const POINT border{
      ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
      ::GetSystemMetrics(SM_CYFRAME)};
  TITLEBARINFOEX title_bar_info;
  title_bar_info.cbSize = sizeof(TITLEBARINFOEX);
  ::SendMessage(window_, WM_GETTITLEBARINFOEX, 0, (LPARAM)&title_bar_info);
  int32_t title_bar_height =
      title_bar_info.rcTitleBar.bottom - title_bar_info.rcTitleBar.top;
  RECT rect;
  rect.left = (int32_t)(window_rect.left + left + border.x);
  rect.top = (int32_t)(window_rect.top + top + title_bar_height + border.y);
  rect.right = (int32_t)(window_rect.left + right + border.x);
  rect.bottom =
      (int32_t)(window_rect.top + bottom + title_bar_height + border.y);
  return rect;
}

NativeViewCore::~NativeViewCore() {}
