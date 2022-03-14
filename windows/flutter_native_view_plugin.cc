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
#include "include/flutter_native_view/flutter_native_view_plugin.h"

/// |HWND| <-> |int32_t| unsafe conversion.
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include <Windows.h>
#include <flutter/method_channel.h>
#include <flutter/method_result_functions.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include "native_view_core.h"

namespace {

static constexpr auto kEnsureInitialized = "EnsureInitialized";
static constexpr auto kUpdateLayeredColor = "UpdateLayeredColor";
static constexpr auto kCreateNativeView = "CreateNativeView";
static constexpr auto kResizeNativeView = "ResizeNativeView";
static constexpr auto kDestroyNativeView = "DestroyNativeView";

static constexpr auto kQueryNativeViews = "QueryNativeViews";

class FlutterNativeViewPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  FlutterNativeViewPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~FlutterNativeViewPlugin();

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  HWND GetWindow();

  flutter::PluginRegistrarWindows* registrar_ = nullptr;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_ =
      nullptr;
  std::unique_ptr<NativeViewCore> native_view_core_ = nullptr;
};

void FlutterNativeViewPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto plugin = std::make_unique<FlutterNativeViewPlugin>(registrar);
  registrar->AddPlugin(std::move(plugin));
}

FlutterNativeViewPlugin::FlutterNativeViewPlugin(
    flutter::PluginRegistrarWindows* registrar)
    : registrar_(registrar) {
  channel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), "flutter_native_view",
      &flutter::StandardMethodCodec::GetInstance());
  channel_->SetMethodCallHandler([=](const auto& call, auto result) {
    HandleMethodCall(call, std::move(result));
  });
}

HWND FlutterNativeViewPlugin::GetWindow() {
  return ::GetAncestor(registrar_->GetView()->GetNativeWindow(), GA_ROOT);
}

FlutterNativeViewPlugin::~FlutterNativeViewPlugin() {}

void FlutterNativeViewPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (native_view_core_ == nullptr) {
    native_view_core_ = std::make_unique<NativeViewCore>(
        GetWindow(), registrar_->GetView()->GetNativeWindow());
  }
  if (method_call.method_name().compare(kEnsureInitialized) == 0) {
    auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto layered_color = std::get<flutter::EncodableMap>(
        arguments[flutter::EncodableValue("layered_color")]);
    auto R = std::get<int32_t>(layered_color[flutter::EncodableValue("R")]);
    auto G = std::get<int32_t>(layered_color[flutter::EncodableValue("G")]);
    auto B = std::get<int32_t>(layered_color[flutter::EncodableValue("B")]);
    auto initialization_type =
        native_view_core_->EnsureInitialized(RGB(R, G, B));
    registrar_->RegisterTopLevelWindowProcDelegate(
        [=](HWND hwnd, UINT message, WPARAM wparam,
            LPARAM lparam) -> std::optional<HRESULT> {
          return native_view_core_->WindowProc(hwnd, message, wparam, lparam);
        });
    result->Success(flutter::EncodableValue(initialization_type));
  } else if (method_call.method_name().compare(kUpdateLayeredColor) == 0) {
    auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto layered_color = std::get<flutter::EncodableMap>(
        arguments[flutter::EncodableValue("layered_color")]);
    auto R = std::get<int32_t>(layered_color[flutter::EncodableValue("R")]);
    auto G = std::get<int32_t>(layered_color[flutter::EncodableValue("G")]);
    auto B = std::get<int32_t>(layered_color[flutter::EncodableValue("B")]);
    native_view_core_->UpdateLayeredColor(RGB(R, G, B));
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kCreateNativeView) == 0) {
    auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto window_handle =
        std::get<int32_t>(arguments[flutter::EncodableValue("window_handle")]);
    auto rect_map = std::get<flutter::EncodableMap>(
        arguments[flutter::EncodableValue("rect")]);
    auto left = std::get<int32_t>(rect_map[flutter::EncodableValue("left")]);
    auto top = std::get<int32_t>(rect_map[flutter::EncodableValue("top")]);
    auto right = std::get<int32_t>(rect_map[flutter::EncodableValue("right")]);
    auto bottom =
        std::get<int32_t>(rect_map[flutter::EncodableValue("bottom")]);
    auto device_pixel_ratio = std::get<double>(
        arguments[flutter::EncodableValue("device_pixel_ratio")]);
    RECT rect;
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    native_view_core_->CreateNativeView((HWND)window_handle, rect,
                                        device_pixel_ratio);
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name().compare(kResizeNativeView) == 0) {
    auto arguments = std::get<flutter::EncodableMap>(*method_call.arguments());
    auto window_handle =
        std::get<int32_t>(arguments[flutter::EncodableValue("window_handle")]);
    auto rect_map = std::get<flutter::EncodableMap>(
        arguments[flutter::EncodableValue("rect")]);
    auto left = std::get<int32_t>(rect_map[flutter::EncodableValue("left")]);
    auto top = std::get<int32_t>(rect_map[flutter::EncodableValue("top")]);
    auto right = std::get<int32_t>(rect_map[flutter::EncodableValue("right")]);
    auto bottom =
        std::get<int32_t>(rect_map[flutter::EncodableValue("bottom")]);
    RECT rect;
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    native_view_core_->ResizeNativeView((HWND)window_handle, rect);
    result->Success(flutter::EncodableValue(nullptr));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void FlutterNativeViewPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  FlutterNativeViewPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
