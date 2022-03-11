
import 'dart:async';

import 'package:flutter/services.dart';

class FlutterNativeView {
  static const MethodChannel _channel = MethodChannel('flutter_native_view');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
