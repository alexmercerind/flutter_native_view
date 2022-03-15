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

#ifndef FFI_API_H_
#define FFI_API_H_

#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include "native_view_core.h"

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT void NativeViewCoreEnsureInitialized(uint32_t layered_color);

DLLEXPORT void NativeViewCoreUpdateLayeredColor(uint32_t layered_color);

DLLEXPORT void NativeViewCoreCreateNativeView(uint32_t native_view,
                                              uint32_t left, uint32_t top,
                                              uint32_t right, uint32_t bottom,
                                              double device_pixel_ratio);

DLLEXPORT void NativeViewCoreDisposeNativeView(uint32_t native_view);

DLLEXPORT void NativeViewCoreResizeNativeView(uint32_t native_view,
                                              uint32_t left, uint32_t top,
                                              uint32_t right, uint32_t bottom);

#ifdef __cplusplus
}
#endif

#endif
