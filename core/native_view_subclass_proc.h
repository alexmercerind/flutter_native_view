// This file is a part of flutter_native_view
// (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include <Windows.h>

#include "native_view_core.h"

namespace flutternativeview {

LRESULT NativeViewSubclassProc(HWND window, UINT message, WPARAM wparam,
                               LPARAM lparam, UINT_PTR subclass_id,
                               DWORD_PTR ref_data) noexcept;

void SetNativeViewSubclassProc(HWND native_view, HWND window);

}  // namespace flutternativeview
