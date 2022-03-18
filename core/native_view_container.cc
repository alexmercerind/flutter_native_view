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

#include <ShObjIdl.h>

#include "utils.h"

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
    case WM_MOUSEMOVE: {
      TRACKMOUSEEVENT event;
      event.cbSize = sizeof(event);
      event.hwndTrack = window;
      event.dwFlags = TME_HOVER;
      event.dwHoverTime = 200;
      NativeViewCore::GetInstance()->SetHitTestBehavior(0);
      break;
    }
    case WM_ERASEBKGND: {
      // Prevent erasing of |window| when it is unfocused and minimized or
      // moved out of screen etc.
      return 1;
    }
    case WM_SIZE:
    case WM_MOVE:
    case WM_MOVING:
    case WM_ACTIVATE:
    case WM_WINDOWPOSCHANGED: {
      NativeViewCore::GetInstance()->SetHitTestBehavior(0);
      auto user_data = ::GetWindowLongPtr(window, GWLP_USERDATA);
      if (user_data) {
        ::SetForegroundWindow(reinterpret_cast<HWND>(user_data));
      }
      break;
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
  window_class.hbrBackground = ::CreateSolidBrush(0);
  ::RegisterClassExW(&window_class);
  auto window =
      ::CreateWindow(kClassName, kWindowName, WS_OVERLAPPEDWINDOW,
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                     nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
  return window;
}

HWND GetNativeViewContainer(HWND window) {
  RECT window_rect;
  ::GetWindowRect(window, &window_rect);
  auto native_view_container = ::FindWindow(kClassName, kWindowName);
  ::SetWindowPos(native_view_container, window, window_rect.left,
                 window_rect.top, window_rect.right - window_rect.left,
                 window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
  ::SetWindowLongPtr(native_view_container, GWLP_USERDATA,
                     reinterpret_cast<LONG>(window));
  // Remove taskbar entry.
  // Using |ITaskbarList3| available on Windows 7 or higher. Modification to
  // |native_view_container|'s |GWL_STYLE| OR |GWL_EX_STYLE| made underlying
  // |HWND| obvious to user.
  ITaskbarList3* taskbar = nullptr;
  ::CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER,
                     IID_PPV_ARGS(&taskbar));
  taskbar->DeleteTab(native_view_container);
  taskbar->Release();
  ::ShowWindow(native_view_container, SW_SHOWNOACTIVATE);
  // TODO: Re-gain focus of |window_| properly once mouse reaches out.
  ::SetFocus(window);
  return native_view_container;
}

}  // namespace flutternativeview