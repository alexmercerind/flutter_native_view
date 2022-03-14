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
import 'package:flutter/services.dart';

import 'package:flutter_native_view/src/constants.dart';
import 'package:flutter_native_view/flutter_native_view.dart';

final MethodChannel channel = const MethodChannel('flutter_native_view')
  ..setMethodCallHandler((call) async {
    if (call.method == kQueryNativeViews) {
      List<int> handles = call.arguments;
      List<Map<String, int>> rects = [];
      for (final handle in handles) {
        rects.add(
          {
            'left': (kNativeViewControllers[handle]!.painterKey.rect!.left *
                    window.devicePixelRatio)
                .toInt(),
            'top': (kNativeViewControllers[handle]!.painterKey.rect!.top *
                    window.devicePixelRatio)
                .toInt(),
            'right': (kNativeViewControllers[handle]!.painterKey.rect!.right *
                    window.devicePixelRatio)
                .toInt(),
            'bottom': (kNativeViewControllers[handle]!.painterKey.rect!.bottom *
                    window.devicePixelRatio)
                .toInt(),
          },
        );
      }
      return rects;
    }
  });

final kNativeViewControllers = <int, NativeViewController>{};
