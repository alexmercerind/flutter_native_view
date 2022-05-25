// This file is a part of flutter_native_view
// (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#ifndef NATIVE_VIEW_CORE_H_
#define NATIVE_VIEW_CORE_H_

#include <Windows.h>

#include <cmath>
#include <functional>
#include <map>
#include <optional>
#include <thread>
#include <vector>

#include "native_view_container.h"
#include "native_view_subclass_proc.h"
#include "utils.h"

namespace flutternativeview {

class NativeViewCore {
 public:
  static constexpr auto kNativeViewPositionAndShowDelay = 300;

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
  std::map<HWND, RECT> native_views_ = {};
  uint64_t last_thread_time_ = 0;
  WPARAM last_wm_size_wparam_ = SIZE_RESTORED;
  bool was_window_hidden_due_to_minimize_ = false;
  static std::unique_ptr<NativeViewCore> instance_;
  static std::optional<int32_t> proc_id_;
};

}  // namespace flutternativeview

#endif
