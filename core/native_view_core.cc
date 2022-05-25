// This file is a part of flutter_native_view
// (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include "native_view_core.h"

namespace flutternativeview {

NativeViewCore* NativeViewCore::GetInstance() { return instance_.get(); }

void NativeViewCore::SetInstance(std::unique_ptr<NativeViewCore> instance) {
  instance_ = std::move(instance);
}

std::optional<int32_t> NativeViewCore::GetProcId() { return proc_id_; }

void NativeViewCore::SetProcId(std::optional<int32_t> proc_id) {
  proc_id_ = proc_id;
}

NativeViewCore::NativeViewCore(HWND window, HWND child_window)
    : window_(window), child_window_(child_window) {}

void NativeViewCore::EnsureInitialized() {
  flutternativeview::SetWindowComposition(window_, 6, 0);
  native_view_container_ =
      flutternativeview::NativeViewContainer::GetInstance()->Get(window_);
}

void NativeViewCore::CreateNativeView(HWND native_view, RECT rect,
                                      double device_pixel_ratio) {
  ::SetParent(native_view, native_view_container_);
  ::ShowWindow(native_view, SW_SHOW);
  flutternativeview::SetNativeViewSubclassProc(native_view, window_);
  auto style = ::GetWindowLongPtr(native_view, GWL_STYLE);
  style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
             WS_EX_APPWINDOW);
  ::SetWindowLongPtr(native_view, GWL_STYLE, style);
  device_pixel_ratio_ = device_pixel_ratio;
  native_views_[native_view] = rect;
  // Position the |native_view| at the correct position behind the |window_|
  // (parent).
  auto global_rect =
      GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom);
  ::SetWindowPos(native_view, window_, global_rect.left, global_rect.top,
                 global_rect.right - global_rect.left,
                 global_rect.bottom - global_rect.top, SWP_NOACTIVATE);
}

void NativeViewCore::DisposeNativeView(HWND native_view) {
  ::SendMessage(native_view, WM_CLOSE, 0, 0);
  native_views_.erase(native_view);
}

void NativeViewCore::SetHitTestBehavior(int32_t hittest_behavior) {
  hittest_behavior;
  LONG ex_style = ::GetWindowLong(window_, GWL_EXSTYLE);
  if (hittest_behavior) {
    ex_style |= (WS_EX_TRANSPARENT | WS_EX_LAYERED);
  } else {
    ex_style &= ~(WS_EX_TRANSPARENT | WS_EX_LAYERED);
  }
  ::SetWindowLong(window_, GWL_EXSTYLE, ex_style);
}

void NativeViewCore::ResizeNativeView(HWND native_view, RECT rect) {
  // TODO: Examine behavior in future.
  // if (native_views_[native_view].left == rect.left &&
  //     native_views_[native_view].top == rect.top &&
  //     native_views_[native_view].right == rect.right &&
  //     native_views_[native_view].bottom == rect.bottom) {
  //   return;
  // }
  native_views_[native_view] = rect;
  auto global_rect =
      GetGlobalRect(rect.left, rect.top, rect.right, rect.bottom);
  // Move the |native_view|, since this happens when the size of the viewport
  // is likely changed, thus redraw is required.
  ::MoveWindow(native_view, global_rect.left, global_rect.top,
               global_rect.right - global_rect.left,
               global_rect.bottom - global_rect.top, TRUE);
}

std::optional<HRESULT> NativeViewCore::WindowProc(HWND hwnd, UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam) {
  switch (message) {
    case WM_ACTIVATE: {
      RECT window_rect;
      ::GetWindowRect(window_, &window_rect);
      // Position |native_view| such that it's z order is behind |window_| &
      // redraw aswell.
      ::SetWindowPos(native_view_container_, window_, window_rect.left,
                     window_rect.top, window_rect.right - window_rect.left,
                     window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
      break;
    }
    case WM_SIZE: {
      // Keeping the |native_view_container_| hidden when minimizing the app &
      // showing it again only when the app is restored.
      // ---- INTENTIONALLY COMMENTED OUT ----
      // if (last_wm_size_wparam_ == SIZE_MINIMIZED) {
      //   std::thread([=]() {
      //     std::this_thread::sleep_for(
      //         std::chrono::milliseconds(kNativeViewPositionAndShowDelay));
      //     ::ShowWindow(native_view_container_, SW_SHOWNOACTIVATE);
      //   }).detach();
      // }

      // Handle Windows's minimize & maximize animations properly.
      // Since |SetWindowPos| & other Win32 APIs on |native_view_container_|
      // do not re-produce the same DWM animations like  actual user
      // interractions on the |window_| do (though both windows are overlapped
      // tightly but maximize and minimze animations can't be mimiced for the
      // both of them at the same time), the best solution is to make the
      // |window_| opaque & hide |native_view_container_| & alter it's position.
      // After that, finally make |native_view_container_| visible again &
      // |window_| transparent again. This approach is not perfect, but it's the
      // best we can do. The minimize & maximize animations on the |window_|
      // look good with just a slight glitch on the visible native views. In
      // future, maybe replacing the |NativeView| widget (Flutter-side) with
      // equivalent window screenshot will result in a totally seamless
      // experience.
      if (wparam != SIZE_RESTORED || last_wm_size_wparam_ == SIZE_MINIMIZED ||
          last_wm_size_wparam_ == SIZE_MAXIMIZED) {
        SetWindowComposition(window_, 0, 0);
        ::ShowWindow(native_view_container_, SW_HIDE);
        last_thread_time_ =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count();
        // A possibly-failed attempt to avoid race-condition & jitter.
        std::thread(
            [=](uint64_t time) {
              if (time < last_thread_time_) {
                return;
              }
              std::this_thread::sleep_for(
                  std::chrono::milliseconds(kNativeViewPositionAndShowDelay));
              SetWindowComposition(window_, 6, 0);
              // Handling SIZE_MINIMIZED separately.
              if (wparam != SIZE_MINIMIZED) {
                ::ShowWindow(native_view_container_, SW_SHOWNOACTIVATE);
              }
            },
            last_thread_time_)
            .detach();
      }
      last_wm_size_wparam_ = wparam;
      break;
    }
    // Keep |native_view_container_| behind the |window_|.
    case WM_MOVE:
    case WM_MOVING:
    case WM_WINDOWPOSCHANGED: {
      RECT window_rect;
      ::GetWindowRect(window_, &window_rect);
      if (window_rect.right - window_rect.left > 0 &&
          window_rect.bottom - window_rect.top > 0) {
        ::SetWindowPos(native_view_container_, window_, window_rect.left,
                       window_rect.top, window_rect.right - window_rect.left,
                       window_rect.bottom - window_rect.top, SWP_NOACTIVATE);
      }
      break;
    }
    case WM_CLOSE: {
      ::SendMessage(native_view_container_, WM_CLOSE, 0, 0);
      for (const auto& [native_view, rect] : native_views_) {
        ::SendMessage(native_view, WM_CLOSE, 0, 0);
      }
      native_views_.clear();
      break;
    }
    default:
      break;
  }
  return std::nullopt;
}

void NativeViewCore::RedrawNativeViews() {
  ::RedrawWindow(native_view_container_, 0, 0,
                 RDW_INVALIDATE | RDW_ALLCHILDREN);
}

RECT NativeViewCore::GetGlobalRect(int32_t left, int32_t top, int32_t right,
                                   int32_t bottom) {
  // Expanding the client area so that window boundaries don't leave empty
  // transparent airspace.
  left -= static_cast<int32_t>(ceil(device_pixel_ratio_));
  top -= static_cast<int32_t>(ceil(device_pixel_ratio_));
  right += static_cast<int32_t>(ceil(device_pixel_ratio_));
  bottom += static_cast<int32_t>(ceil(device_pixel_ratio_));
  RECT window_rect;
  ::GetClientRect(window_, &window_rect);
  RECT rect;
  rect.left = window_rect.left + left;
  rect.top = window_rect.top + top;
  rect.right = window_rect.left + right;
  rect.bottom = window_rect.top + bottom;
  return rect;
}

NativeViewCore::~NativeViewCore() {
  // Destroy existing |native_views_|.
  for (const auto& [native_view, rect] : native_views_) {
    ::SendMessage(native_view, WM_CLOSE, 0, 0);
  }
  native_views_.clear();
}

std::unique_ptr<NativeViewCore> NativeViewCore::instance_ = nullptr;

std::optional<int32_t> NativeViewCore::proc_id_ = std::nullopt;

}  // namespace flutternativeview
