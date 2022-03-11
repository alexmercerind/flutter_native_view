/// This file is part of flutter_native_view (https://github.com/alexmercerind/flutter_native_view).
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

import 'package:flutter/widgets.dart';
import 'package:flutter/services.dart';

class FlutterNativeView {
  static const MethodChannel _channel = MethodChannel('flutter_native_view');

  static Future<void> ensureInitialized({
    Color layeredColor = _kDefaultLayeredColor,
  }) async {
    initializationType = await _channel.invokeMethod(
      _kEnsureInitialized,
      {
        'layered_color': {
          'R': layeredColor.red,
          'G': layeredColor.green,
          'B': layeredColor.blue,
        },
      },
    );
  }

  static Future<void> updateLayeredColor({
    Color layeredColor = _kDefaultLayeredColor,
  }) async {
    layeredColor = layeredColor;
    initializationType = await _channel.invokeMethod(
      _kEnsureInitialized,
      {
        'layered_color': {
          'R': layeredColor.red,
          'G': layeredColor.green,
          'B': layeredColor.blue,
        },
      },
    );
  }
}

Color layeredColor = _kDefaultLayeredColor;
int initializationType = 1;

const Color _kDefaultLayeredColor = Color(0xFFB66146);

const String _kEnsureInitialized = 'EnsureInitialized';
const String _kUpdateLayeredColor = 'UpdateLayeredColor';
const String _kCreateNativeView = 'CreateNativeView';
const String _kDestroyNativeView = 'DestroyNativeView';
