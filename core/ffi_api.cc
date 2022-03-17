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
