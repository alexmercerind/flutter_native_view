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

#include <Windows.h>

#include <optional>
#include <unordered_map>

#include "utils.h"

class NativeViewCore {
 public:
  NativeViewCore(HWND window);

  int32_t EnsureInitialized(COLORREF layered_color);

  void UpdateLayeredColor(COLORREF layered_color);

  void CreateNativeView(HWND window, int32_t left, int32_t top, int32_t right,
                        int32_t bottom, double device_pixel_ratio);

  std::optional<HRESULT> WindowProc(HWND hwnd, UINT message, WPARAM wparam,
                                    LPARAM lparam);

  ~NativeViewCore();

 private:
  RECT GetGlobalRect(int32_t left, int32_t top, int32_t right, int32_t bottom,
                     double device_pixel_ratio);

  HWND window_ = nullptr;
  double device_pixel_ratio_ = 1.0;
  std::unordered_map<HWND, RECT> native_views_ = {};
};
