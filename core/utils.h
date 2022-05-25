// This file is a part of libmpv.dart
// (https://github.com/alexmercerind/libmpv.dart).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include <dwmapi.h>

#include <cstdint>

namespace flutternativeview {

RTL_OSVERSIONINFOW GetWindowsVersion();

void SetWindowComposition(HWND window, int32_t accent_state,
                          int32_t gradient_color);

}  // namespace flutternativeview
