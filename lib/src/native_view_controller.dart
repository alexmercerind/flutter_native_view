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
  final int handle;
  final GlobalKey rendererKey = GlobalKey();
  final GlobalKey painterKey = GlobalKey();

  // [StreamController] to avoid race & send [Rect]s synchronously.
  final StreamController<void> resizeNativeViewStreamController =
      StreamController<void>();
  late final StreamSubscription<void> resizeNativeViewStreamSubscription;

  NativeViewController({required this.handle}) {
    kNativeViewControllers[handle] = this;
    resizeNativeViewStreamSubscription =
        resizeNativeViewStreamController.stream.listen((event) async {
      await refresh();
    });
  }

  Future<void> dispose() {
    kNativeViewControllers.remove(handle);
    return resizeNativeViewStreamSubscription.cancel();
  }

  Future<void> createNativeView() async {
    await channel.invokeMethod(
      kCreateNativeView,
      {
        'window_handle': handle,
        'rect': {
          'left': (painterKey.rect!.left * window.devicePixelRatio).toInt(),
          'top': (painterKey.rect!.top * window.devicePixelRatio).toInt(),
          'right': (painterKey.rect!.right * window.devicePixelRatio).toInt(),
          'bottom': (painterKey.rect!.bottom * window.devicePixelRatio).toInt(),
        },
        'device_pixel_ratio': window.devicePixelRatio,
      },
    );
  }

  Future<void> refresh() async {
    await channel.invokeMethod(
      kResizeNativeView,
      {
        'window_handle': handle,
        'rect': {
          'left': (painterKey.rect!.left * window.devicePixelRatio).toInt(),
          'top': (painterKey.rect!.top * window.devicePixelRatio).toInt(),
          'right': (painterKey.rect!.right * window.devicePixelRatio).toInt(),
          'bottom': (painterKey.rect!.bottom * window.devicePixelRatio).toInt(),
        },
      },
    );
    rendererKey.currentState!.setState(() {});
  }
}
