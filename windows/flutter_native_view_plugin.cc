// This file is a part of flutter_native_view
// (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.
#include "include/flutter_native_view/flutter_native_view_plugin.h"

#include <flutter/method_channel.h>
#include <flutter/method_result_functions.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

namespace {

class FlutterNativeViewPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  FlutterNativeViewPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~FlutterNativeViewPlugin();

 private:
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  HWND GetChildWindow();
  HWND GetWindow();

  flutter::PluginRegistrarWindows* registrar_ = nullptr;
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_ =
      nullptr;
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

HWND FlutterNativeViewPlugin::GetChildWindow() {
  return registrar_->GetView()->GetNativeWindow();
}

HWND FlutterNativeViewPlugin::GetWindow() {
  return ::GetAncestor(GetChildWindow(), GA_ROOT);
}

FlutterNativeViewPlugin::~FlutterNativeViewPlugin() {}

void FlutterNativeViewPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name() == "init") {
    if (flutternativeview::NativeViewCore::GetProcId()) {
      registrar_->UnregisterTopLevelWindowProcDelegate(
          flutternativeview::NativeViewCore::GetProcId().value());
      flutternativeview::NativeViewCore::SetProcId(std::nullopt);
    }
    flutternativeview::NativeViewCore::SetInstance(
        std::move(std::make_unique<flutternativeview::NativeViewCore>(
            GetWindow(), GetChildWindow())));
    if (!flutternativeview::NativeViewCore::GetProcId()) {
      flutternativeview::NativeViewCore::SetProcId(
          registrar_->RegisterTopLevelWindowProcDelegate(
              [=](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
                return flutternativeview::NativeViewCore::GetInstance()
                    ->WindowProc(hwnd, message, wparam, lparam);
              }));
    }
    result->Success(flutter::EncodableValue(nullptr));
  } else if (method_call.method_name() == "setFullScreen") {
    const flutter::EncodableMap& args =
        std::get<flutter::EncodableMap>(*method_call.arguments());
    bool isFullScreen =
        std::get<bool>(args.at(flutter::EncodableValue("isFullScreen")));
    flutternativeview::NativeViewContainer::GetInstance()->SetFullScreen(
        isFullScreen);
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
