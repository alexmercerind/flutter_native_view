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

// Global variables... Thanks to Win32.
HWND g_window = nullptr;
static std::unordered_map<HWND, std::unique_ptr<RECT>> g_native_views = {};

NativeViewCore::NativeViewCore(HWND window) {
  g_window = window;
  SetWindowSubclass(g_window, NativeViewCore::SubclassWndProc, 1, NULL);
}

int NativeViewCore::EnsureInitialized(COLORREF layered_color) {
  // Use |SetWindowCompositionAttribute| API on Windows versions higher or equal
  // to 1903.
  if (GetWindowsVersion().dwBuildNumber > 18362) {
    SetWindowComposition(g_window, 2, 0);
    return 0;
  } else {
    // Use |WS_EX_LAYERED| with layered color defined by |layered_color| on
    // older Windows versions.
    auto style = ::GetWindowLongPtr(g_window, GWL_EXSTYLE);
    ::SetWindowLongPtr(g_window, GWL_EXSTYLE, style | WS_EX_LAYERED);
    ::SetLayeredWindowAttributes(g_window, layered_color, 0, LWA_COLORKEY);
    return 1;
  }
}

void NativeViewCore::UpdateLayeredColor(COLORREF layered_color) {
  ::SetLayeredWindowAttributes(g_window, layered_color, 0, LWA_COLORKEY);
}

void NativeViewCore::CreateNativeView(HWND window, int32_t x, int32_t y,
                                      int32_t cx, int32_t cy) {
  // Expanding the client area so that window boundaries don't leave empty
  // transparent airspace.
  x -= 1;
  y -= 1;
  // Since the left & right have been reduced by 1 pixels, increasing the widths
  // by 2 so as to cover the space correctly.
  cx += 2;
  cy += 2;
  auto style = ::GetWindowLongPtr(window, GWL_STYLE);
  style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
             WS_EX_APPWINDOW);
  ::SetWindowLongPtr(window, GWL_STYLE, style);
  RECT parent_window_rect;
  GetWindowRect(g_window, &parent_window_rect);
  const POINT border{
      ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
      ::GetSystemMetrics(SM_CYFRAME)};
  TITLEBARINFOEX title_bar_info;
  title_bar_info.cbSize = sizeof(TITLEBARINFOEX);
  ::SendMessage(g_window, WM_GETTITLEBARINFOEX, 0, (LPARAM)&title_bar_info);
  int32_t title_bar_height =
      title_bar_info.rcTitleBar.bottom - title_bar_info.rcTitleBar.top;
  auto rect = std::make_unique<RECT>();
  // Retrieved values are assumed to be pre-mutiplied with the scaling factor.
  // TODO: Store values in correct format as expected by |RECT|.
  rect->left = x;
  rect->top = y;
  rect->right = cx;
  rect->bottom = cy;
  g_native_views[window] = std::move(rect);
  // Position the |window| at the correct position behind the |window_|
  // (parent).
  ::SetWindowPos(
      window, g_window, (int32_t)(parent_window_rect.left + x + border.x),
      (int32_t)(parent_window_rect.top + y + title_bar_height + border.y),
      (int32_t)cx, (int32_t)cy, SWP_NOACTIVATE);
}

LRESULT CALLBACK NativeViewCore::SubclassWndProc(HWND window, UINT message,
                                                 WPARAM wparam, LPARAM lparam,
                                                 UINT_PTR subclassID,
                                                 DWORD_PTR refData) noexcept {
  switch (message) {
    case WM_WINDOWPOSCHANGED: {
      for (const auto& [native_view, rect] : g_native_views) {
        RECT parent_window_rect;
        GetWindowRect(g_window, &parent_window_rect);
        const POINT border{::GetSystemMetrics(SM_CXFRAME) +
                               ::GetSystemMetrics(SM_CXPADDEDBORDER),
                           ::GetSystemMetrics(SM_CYFRAME)};
        TITLEBARINFOEX title_bar_info;
        title_bar_info.cbSize = sizeof(TITLEBARINFOEX);
        ::SendMessage(g_window, WM_GETTITLEBARINFOEX, 0,
                      (LPARAM)&title_bar_info);
        int32_t title_bar_height =
            title_bar_info.rcTitleBar.bottom - title_bar_info.rcTitleBar.top;
        // Position the |native_view| at the correct position behind the
        // |window_| (parent).
        ::SetWindowPos(
            native_view, g_window,
            (int32_t)(parent_window_rect.left + rect->left + border.x),
            (int32_t)(parent_window_rect.top + rect->top + title_bar_height +
                      border.y),
            (int32_t)rect->right, (int32_t)rect->bottom, SWP_NOACTIVATE);
      }
    }
    default:
      break;
  }
  return DefSubclassProc(window, message, wparam, lparam);
}

NativeViewCore::~NativeViewCore() {}
