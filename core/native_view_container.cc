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

#include "native_view_container.h"

namespace flutternativeview {

constexpr const auto kClassName = L"FLUTTER_NATIVE_VIEW";
constexpr const auto kWindowName = L"flutter_native_view";

LRESULT CALLBACK NativeViewContainerProc(HWND const window, UINT const message,
                                         WPARAM const wparam,
                                         LPARAM const lparam) noexcept {
  switch (message) {
    case WM_DESTROY: {
      ::PostQuitMessage(0);
      return 0;
    }
    default:
      break;
  }
  return ::DefWindowProc(window, message, wparam, lparam);
}

HWND CreateNativeViewContainer() {
  auto window_class = WNDCLASSEX{};
  ::SecureZeroMemory(&window_class, sizeof(window_class));
  window_class.cbSize = sizeof(window_class);
  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = NativeViewContainerProc;
  window_class.hInstance = 0;
  window_class.lpszClassName = kClassName;
  window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
  window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  ::RegisterClassExW(&window_class);
  auto window = ::CreateWindowEx(
      WS_EX_TOOLWINDOW, kClassName, kWindowName, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
      nullptr, GetModuleHandle(nullptr), nullptr);

  return window;
}

HWND GetNativeViewContainer(HWND window) {
  RECT window_rect;
  ::GetWindowRect(window, &window_rect);
  auto native_view_container = ::FindWindow(kClassName, kWindowName);
  ::SetWindowPos(native_view_container, window, window_rect.left,
                 window_rect.top, window_rect.right - window_rect.left,
                 window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
  ::ShowWindow(native_view_container, SW_SHOWNOACTIVATE);
  ::SetFocus(window);
  return native_view_container;
}

}  // namespace flutternativeview