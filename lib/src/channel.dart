import 'dart:ui';
import 'package:flutter/services.dart';

import 'package:flutter_native_view/src/constants.dart';
import 'package:flutter_native_view/flutter_native_view.dart';

final MethodChannel channel = const MethodChannel('flutter_native_view')
  ..setMethodCallHandler((call) async {
    if (call.method == kQueryNativeViews) {
      List<int> windowHandles = call.arguments;
      List<Map<String, int>> rects = [];
      for (final windowHandle in windowHandles) {
        rects.add(
          {
            'left':
                (kNativeViewControllers[windowHandle]!.globalKey.rect!.left *
                        window.devicePixelRatio)
                    .toInt(),
            'top': (kNativeViewControllers[windowHandle]!.globalKey.rect!.top *
                    window.devicePixelRatio)
                .toInt(),
            'right':
                (kNativeViewControllers[windowHandle]!.globalKey.rect!.right *
                        window.devicePixelRatio)
                    .toInt(),
            'bottom':
                (kNativeViewControllers[windowHandle]!.globalKey.rect!.bottom *
                        window.devicePixelRatio)
                    .toInt(),
          },
        );
      }
      return rects;
    }
  });

final kNativeViewControllers = <int, NativeViewController>{};
