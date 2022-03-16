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

#ifndef NATIVE_VIEW_CONTAINER_H_
#define NATIVE_VIEW_CONTAINER_H_

#ifndef DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#endif

#include <Windows.h>

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
