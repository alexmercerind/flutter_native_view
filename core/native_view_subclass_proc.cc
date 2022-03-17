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

#include "native_view_subclass_proc.h"

#include <Commctrl.h>
#include <Windows.h>

namespace flutternativeview {

LRESULT NativeViewSubclassProc(HWND window, UINT message, WPARAM wparam,
                               LPARAM lparam, UINT_PTR subclass_id,
                               DWORD_PTR ref_data) noexcept {
  switch (message) {
    case WM_MOUSELEAVE: {
      // Gain focus to the |NativeViewCore::window_| again.
      NativeViewCore::GetInstance()->SetHitTestBehavior(0);
      break;
    }
    default:
      break;
  }
  return ::DefSubclassProc(window, message, wparam, lparam);
}

void SetNativeViewSubclassProc(HWND native_view, HWND window) {
  TRACKMOUSEEVENT event;
  event.cbSize = sizeof(event);
  event.hwndTrack = native_view;
  event.dwFlags = TME_LEAVE;
  event.dwHoverTime = 1;
  ::_TrackMouseEvent(&event);
  ::SetWindowSubclass(native_view, NativeViewSubclassProc, 69420, NULL);
}

}  // namespace flutternativeview
