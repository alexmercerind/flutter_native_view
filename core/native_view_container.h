// This file is a part of libmpv.dart
// (https://github.com/alexmercerind/libmpv.dart).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#ifndef NATIVE_VIEW_CONTAINER_H_
#define NATIVE_VIEW_CONTAINER_H_

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

#include <Windows.h>

#include "native_view_core.h"

namespace flutternativeview {

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT HWND CreateNativeViewContainer();

DLLEXPORT HWND GetNativeViewContainer(HWND window);

#ifdef __cplusplus
}
#endif

}  // namespace flutternativeview

#endif
