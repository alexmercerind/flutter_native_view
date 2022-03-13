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

import 'dart:async';
import 'dart:ui';
import 'package:flutter/widgets.dart';

import 'package:flutter_native_view/src/channel.dart';
import 'package:flutter_native_view/src/constants.dart';
import 'package:flutter_native_view/src/widgets.dart';

class NativeViewController {
  final int windowHandle;
  final GlobalKey globalKey = GlobalKey();

  // [StreamController] to avoid race & send [Rect]s synchronously.
  final StreamController<void> resizeNativeViewStreamController =
      StreamController<void>();

  NativeViewController({required this.windowHandle}) {
    kNativeViewControllers[windowHandle] = this;
  }

  Future<void> createNativeView() async {
    await channel.invokeMethod(
      kCreateNativeView,
      {
        'window_handle': windowHandle,
        'rect': {
          'left': (globalKey.rect!.left * window.devicePixelRatio).toInt(),
          'top': (globalKey.rect!.top * window.devicePixelRatio).toInt(),
          'right': (globalKey.rect!.right * window.devicePixelRatio).toInt(),
          'bottom': (globalKey.rect!.bottom * window.devicePixelRatio).toInt(),
        },
        'device_pixel_ratio': window.devicePixelRatio,
      },
    );
    resizeNativeViewStreamController.stream.listen((event) async {
      await channel.invokeMethod(
        kResizeNativeView,
        {
          'window_handle': windowHandle,
          'rect': {
            'left': (globalKey.rect!.left * window.devicePixelRatio).toInt(),
            'top': (globalKey.rect!.top * window.devicePixelRatio).toInt(),
            'right': (globalKey.rect!.right * window.devicePixelRatio).toInt(),
            'bottom':
                (globalKey.rect!.bottom * window.devicePixelRatio).toInt(),
          },
        },
      );
    });
  }
}
