// This file is a part of flutter_native_view
// (https://github.com/alexmercerind/flutter_native_view).
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

class NativeViewContainer {
 public:
  DLLEXPORT static NativeViewContainer* GetInstance();

  // Creates a new |NativeViewContainer| to hold various native views.
  // Transitions on this window are disabled using
  // |DWMWA_TRANSITIONS_FORCEDISABLED|.
  DLLEXPORT HWND NativeViewContainer::Create();

  // Returns the |NativeViewContainer|'s window handle for the given parent
  // |window|. The method also removes the taskbar entry of |handle_| & returns
  // focus back to |window| (if lost).
  // |window| is stored on |NativeViewContainer|'s window as |GWLP_USERDATA|.
  DLLEXPORT HWND NativeViewContainer::Get(HWND window);

 private:
  static std::unique_ptr<NativeViewContainer> NativeViewContainer::instance_;
  static constexpr auto kClassName = L"FLUTTER_NATIVE_VIEW";
  static constexpr auto kWindowName = L"flutter_native_view";

  static LRESULT CALLBACK NativeViewContainer::WindowProc(
      HWND const window, UINT const message, WPARAM const wparam,
      LPARAM const lparam) noexcept;

  HWND handle_;
};

#ifdef __cplusplus
}
#endif

}  // namespace flutternativeview

#endif
