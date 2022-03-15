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

import 'dart:io';
import 'dart:ffi';
import 'package:path/path.dart' as path;

abstract class FFI {
  static late final void Function(int) nativeViewCoreEnsureInitialized;
  static late final void Function(int) nativeViewCoreUpdateLayeredColor;
  static late final void Function(int, int, int, int, int, double)
      nativeViewCoreCreateNativeView;
  static late final void Function(int) nativeViewCoreDisposeNativeView;
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
        library.lookupFunction<Void Function(Uint32), void Function(int)>(
            'NativeViewCoreEnsureInitialized');
    nativeViewCoreUpdateLayeredColor =
        library.lookupFunction<Void Function(Uint32), void Function(int)>(
            'NativeViewCoreUpdateLayeredColor');
    nativeViewCoreCreateNativeView = library.lookupFunction<
        Void Function(Uint32, Uint32, Uint32, Uint32, Uint32, Double),
        void Function(
            int, int, int, int, int, double)>('NativeViewCoreCreateNativeView');
    nativeViewCoreDisposeNativeView =
        library.lookupFunction<Void Function(Uint32), void Function(int)>(
            'NativeViewCoreDisposeNativeView');
    nativeViewCoreResizeNativeView = library.lookupFunction<
        Void Function(Uint32, Uint32, Uint32, Uint32, Uint32),
        void Function(
            int, int, int, int, int)>('NativeViewCoreResizeNativeView');
  }
}
