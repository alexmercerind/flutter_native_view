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

NativeViewCore::~NativeViewCore() {}
