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

#ifndef NATIVE_VIEW_CORE_H_
#define NATIVE_VIEW_CORE_H_

#include <Windows.h>

#include <cmath>
#include <functional>
#include <map>
#include <optional>
#include <vector>

#include "native_view_container.h"
#include "native_view_subclass_proc.h"
#include "utils.h"

namespace flutternativeview {

class NativeViewCore {
 public:
  static NativeViewCore* GetInstance();

  static void SetInstance(std::unique_ptr<NativeViewCore> instance);

  static std::optional<int32_t> GetProcId();

  static void SetProcId(std::optional<int32_t> proc_id);

  NativeViewCore(HWND window, HWND child_window);

  void EnsureInitialized();

  void CreateNativeView(HWND native_view, RECT rect, double device_pixel_ratio);

  void DisposeNativeView(HWND native_view);

  void SetHitTestBehavior(int32_t hittest_behavior);

  void ResizeNativeView(HWND native_view, RECT rect);

  std::optional<HRESULT> WindowProc(HWND hwnd, UINT message, WPARAM wparam,
                                    LPARAM lparam);

  ~NativeViewCore();

 private:
  void RedrawNativeViews();

  RECT GetGlobalRect(int32_t left, int32_t top, int32_t right, int32_t bottom);

  HWND window_ = nullptr;
  HWND child_window_ = nullptr;
  HWND native_view_container_ = nullptr;
  double device_pixel_ratio_ = 1.0;
  int32_t title_bar_height_ = 0;
  std::map<HWND, RECT> native_views_ = {};
  static std::unique_ptr<NativeViewCore> instance_;
  static std::optional<int32_t> proc_id_;
};

}  // namespace flutternativeview

#endif
