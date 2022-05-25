// This file is a part of libmpv.dart
// (https://github.com/alexmercerind/libmpv.dart).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include "ffi_api.h"

void NativeViewCoreEnsureInitialized() {
  flutternativeview::NativeViewCore::GetInstance()->EnsureInitialized();
}

void NativeViewCoreCreateNativeView(uint32_t native_view, uint32_t left,
                                    uint32_t top, uint32_t right,
                                    uint32_t bottom,
                                    double device_pixel_ratio) {
  RECT rect;
  rect.left = left;
  rect.top = top;
  rect.right = right;
  rect.bottom = bottom;
  flutternativeview::NativeViewCore::GetInstance()->CreateNativeView(
      reinterpret_cast<HWND>(native_view), rect, device_pixel_ratio);
}

void NativeViewCoreDisposeNativeView(uint32_t native_view) {
  flutternativeview::NativeViewCore::GetInstance()->DisposeNativeView(
      reinterpret_cast<HWND>(native_view));
}
void NativeViewCoreSetHitTestBehavior(uint32_t hittest_behavior) {
  flutternativeview::NativeViewCore::GetInstance()->SetHitTestBehavior(
      hittest_behavior);
}

void NativeViewCoreResizeNativeView(uint32_t native_view, uint32_t left,
                                    uint32_t top, uint32_t right,
                                    uint32_t bottom) {
  RECT rect;
  rect.left = left;
  rect.top = top;
  rect.right = right;
  rect.bottom = bottom;
  flutternativeview::NativeViewCore::GetInstance()->ResizeNativeView(
      reinterpret_cast<HWND>(native_view), rect);
}
