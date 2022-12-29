// This file is a part of flutter_native_view (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the LICENSE file.
import 'package:flutter/services.dart';

import 'package:flutter_native_view/src/ffi.dart';
import 'package:flutter_native_view/src/constants.dart';

const _channel = MethodChannel(kMethodChannelName);

class FlutterNativeView {
  static Future<void> ensureInitialized() async {
    await _channel.invokeMethod('init');
    FFI.ensureInitialized();
    FFI.nativeViewCoreEnsureInitialized();
  }

  static Future<void> setFullScreen(bool isFullScreen) async {
    await _channel
        .invokeMethod('setFullScreen', {'isFullScreen': isFullScreen});
  }
}
