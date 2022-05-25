// This file is a part of flutter_native_view (https://github.com/alexmercerind/flutter_native_view).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the LICENSE file.
import 'dart:io';
import 'dart:ffi';
import 'package:path/path.dart' as path;

/// Lookups for symbols in the bundled dynamic library.
///
abstract class FFI {
  static late final void Function() nativeViewCoreEnsureInitialized;
  static late final void Function(int, int, int, int, int, double)
      nativeViewCoreCreateNativeView;
  static late final void Function(int) nativeViewCoreDisposeNativeView;
  static late final void Function(int) nativeViewCoreSetHitTestBehavior;
  static late final void Function(int, int, int, int, int)
      nativeViewCoreResizeNativeView;

  static void ensureInitialized() {
    final library = DynamicLibrary.open(
      path.join(
        path.dirname(Platform.resolvedExecutable),
        'flutter_native_view_plugin.dll',
      ),
    );
    nativeViewCoreEnsureInitialized =
        library.lookupFunction<Void Function(), void Function()>(
            'NativeViewCoreEnsureInitialized');
    nativeViewCoreCreateNativeView = library.lookupFunction<
        Void Function(Uint32, Uint32, Uint32, Uint32, Uint32, Double),
        void Function(
            int, int, int, int, int, double)>('NativeViewCoreCreateNativeView');
    nativeViewCoreDisposeNativeView =
        library.lookupFunction<Void Function(Uint32), void Function(int)>(
            'NativeViewCoreDisposeNativeView');
    nativeViewCoreSetHitTestBehavior =
        library.lookupFunction<Void Function(Uint32), void Function(int)>(
            'NativeViewCoreSetHitTestBehavior');
    nativeViewCoreResizeNativeView = library.lookupFunction<
        Void Function(Uint32, Uint32, Uint32, Uint32, Uint32),
        void Function(
            int, int, int, int, int)>('NativeViewCoreResizeNativeView');
  }
}
