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

import 'dart:ui';
import 'dart:async';
import 'package:flutter/widgets.dart';

import 'package:flutter_native_view/src/ffi.dart';
import 'package:flutter_native_view/src/widgets.dart';

/// [NativeViewController] creates a new controller to control an instance of [NativeView].
///
/// ```dart
/// final controller = NativeViewController(
///   handle: FindWindow(
///     nullptr,
///     'VLC Media Player'.toNativeUtf16(),
///   ),
/// );
/// ```
///
/// Pass the `HWND` of the window as [handle].
/// In above example, we are using `FindWindow` from `package:win32` to discover handle of the `VLC Media Player`'s window.
///
///
/// **Other Notes**
///
/// When [NativeView] is no longer in use always make sure to dispose it using [dispose].
///
/// ```dart
/// controller.dispose();
/// ```
///
/// The [NativeView] can be explicitly re-drawn using [refresh].
///
/// ```dart
/// controller.refresh();
/// ```
///
class NativeViewController {
  final int handle;
  final GlobalKey rendererKey = GlobalKey();
  final GlobalKey painterKey = GlobalKey();

  /// [StreamController] to avoid race & send [Rect]s synchronously.
  final StreamController<void> resizeNativeViewStreamController =
      StreamController<void>();
  late final StreamSubscription<void> resizeNativeViewStreamSubscription;

  HitTestBehavior hitTestBehavior;
  bool entered = false;

  NativeViewController({
    required this.handle,
    this.hitTestBehavior = HitTestBehavior.opaque,
  }) {
    resizeNativeViewStreamSubscription =
        resizeNativeViewStreamController.stream.listen(
      (event) {
        refresh();
      },
    );
  }

  /// Disposes the [NativeViewController] instance & releases the resources.
  Future<void> dispose() {
    FFI.nativeViewCoreDisposeNativeView(handle);
    return resizeNativeViewStreamSubscription.cancel();
  }

  /// Creates a new [NativeView].
  void createNativeView() {
    FFI.nativeViewCoreCreateNativeView(
      handle,
      (painterKey.rect!.left * window.devicePixelRatio).toInt(),
      (painterKey.rect!.top * window.devicePixelRatio).toInt(),
      (painterKey.rect!.right * window.devicePixelRatio).toInt(),
      (painterKey.rect!.bottom * window.devicePixelRatio).toInt(),
      window.devicePixelRatio,
    );
  }

  /// Creates a new [NativeView].
  ///
  /// NOTE: [HitTestBehavior.deferToChild] does not work.
  ///
  void setHitTestBehavior(HitTestBehavior value) {
    hitTestBehavior = value;
  }

  /// Causes [NativeView] associated with this [NativeViewController] to redraw & update its positioning.
  ///
  /// TODO: Fix [force] argument.
  ///
  void refresh({bool force = true}) {
    FFI.nativeViewCoreResizeNativeView(
      handle,
      (painterKey.rect!.left * window.devicePixelRatio).toInt(),
      (painterKey.rect!.top * window.devicePixelRatio).toInt(),
      (painterKey.rect!.right * window.devicePixelRatio).toInt(),
      (painterKey.rect!.bottom * window.devicePixelRatio).toInt(),
    );
    rendererKey.currentState!.setState(() {});
  }
}
