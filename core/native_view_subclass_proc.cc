// This file is a part of libmpv.dart
// (https://github.com/alexmercerind/libmpv.dart).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include "native_view_subclass_proc.h"

#include <Commctrl.h>
#include <Windows.h>

namespace flutternativeview {

LRESULT NativeViewSubclassProc(HWND window, UINT message, WPARAM wparam,
                               LPARAM lparam, UINT_PTR subclass_id,
                               DWORD_PTR ref_data) noexcept {
  switch (message) {
    case WM_ERASEBKGND: {
      // Prevent erasing of |window| when it is unfocused and minimized or
      // moved out of screen etc.
      return 1;
    }
    case WM_SIZE: {
      // Prevent unnecessary maxmize, minimize or restore messages for |window|.
      // Since it is |SetParent|'ed into native view container.
      return 1;
    }
    default:
      break;
  }
  return ::DefSubclassProc(window, message, wparam, lparam);
}

void SetNativeViewSubclassProc(HWND native_view, HWND window) {
  ::SetWindowSubclass(native_view, NativeViewSubclassProc, 69420, NULL);
}

}  // namespace flutternativeview
