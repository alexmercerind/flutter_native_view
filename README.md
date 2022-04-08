# [flutter_native_view](https://github.com/alexmercerind/flutter_native_view)

[![GPL](https://img.shields.io/badge/license-GPL-green.svg)](https://github.com/alexmercerind/flutter_native_view/LICENSE) • [![](https://img.shields.io/twitter/follow/alexmercerind)](https://twitter.com/alexmercerind) • [![Donate](https://img.shields.io/badge/Donate-PayPal-blue.svg)](https://paypal.me/alexmercerind) • [![pub package](https://img.shields.io/pub/v/flutter_native_view.svg)](https://pub.dartlang.org/packages/flutter_native_view)

Embedding native windows & components directly into Flutter window.

## Example

Video showing a Flutter Windows app running with embedded webview & VLC using [flutter_native_view](https://github.com/alexmercerind/flutter_native_view).

https://user-images.githubusercontent.com/28951144/159073594-813700fb-0988-424f-86b5-381beccf4247.mp4

Try running the [example](./example) application by cloning the repository.

## Sponsor

You may sponsor this project's future development & research at:
- [PayPal](https://www.paypal.me/alexmercerind) (one-time)
- [Patreon](https://www.patreon.com/harmonoid) (monthly-recurring)

It'll be a great motivation for me to continue.

### 💖 Current Sponsors

- [Ahmad Arif Aulia Sutarman](https://github.com/damywise) • 20$ • one-time


## Used By

- [dart_vlc](https://github.com/alexmercerind/dart_vlc#nativevideo)

## Description

This setup only uses Win32 APIs & no texture, intermediate buffers or copying of pixel buffers. Thus, it is **very performant**.

A Flutter plugin / Win32 setup to embed other native Windows (`HWND` on Windows) into Flutter window.

Current API design allows to embed any arbitrary `HWND` completely from Dart as a `Widget`. This can be a good choice when client code wants to embed any third-party window (which is already opened) into the Flutter window.
However, this is not ideal in most cases, because there is almost no point of embedding a window without having a programmatic control to it (via some API).

On the other hand, a window created by the client code itself e.g. a webview instance window or a video-output window etc. (on which the client code has full programmatic control) will be an ideal window to embed, in that case:

- If client code decides to create an `HWND` through platform channel interface, they can use the setup present in [`core`](https://github.com/alexmercerind/flutter_native_view/tree/master/core) to embed a window (or send back the `HWND` as `int64_t` to the plugin throught Dart).
- Since `dart:ffi` is very capable now, one can pass the `HWND` directly as `int` of the window they created using `dart:ffi` or [`package:win32`](https://github.com/timsneath/win32) through existing plugin API to embed it.

## Future

In future, I will create natively rendered, performant & less-bundle-sized webview & video playback plugins, if I get enough community support. Currently I'm only targetting Windows to limit the scope of work, though I plan for Linux support at some point.

## Setup

Add following lines to your `windows/runner/main.cpp` file:

```diff
  #include <flutter/dart_project.h>
  #include <flutter/flutter_view_controller.h>
  #include <windows.h>

+ #include "flutter_native_view/flutter_native_view_plugin.h"
  #include "flutter_window.h"
  #include "utils.h"
```

```diff
  window.SetQuitOnClose(true);

+ flutternativeview::CreateNativeViewContainer();

  ::MSG msg;
  while (::GetMessage(&msg, nullptr, 0, 0)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
```

## Docs

#### Initialize the plugin

```dart
Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  /// Initialize.
  await FlutterNativeView.ensureInitialized();
  runApp(const MyApp());
}
```

#### Create a controller & render a window

```dart
class _MyAppState extends State<MyApp> {
  /// Create a [NativeViewController].
  final controller = NativeViewController(
    /// Using [FindWindow] from `package:win32` to retrieve `HWND` as [int].
    handle: FindWindow(nullptr, 'VLC Media Player'.toNativeUtf16()),
    /// Make the [NativeView] interactable.
    hitTestBehavior: HitTestBehavior.translucent,
  );

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
            child: Padding(
              padding: const EdgeInsets.all(24.0),
              child: Stack(
                children: [
                  LayoutBuilder(
                    /// Create a [NativeView].
                    builder: (context, constraints) => NativeView(
                      /// Pass [NativeViewController] to render the window.
                      controller: controller,
                      width: constraints.maxWidth,
                      height: constraints.maxHeight,
                    ),
                  ),
                  Padding(
                    padding: const EdgeInsets.all(16.0),
                    child: FloatingActionButton(
                      onPressed: () {},
                      child: const Icon(Icons.edit),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
```

#### Dispose the native view & destory the window

```dart
controller.dispose();
```

## Features

#### Currently Implemented

- Placement of other Flutter `Widget`s on top of the `NativeView`.
- Multiple instances of `NativeView`.
- Window movement handling & `NativeView`s positioning.
- Window resize handling & `NativeView`s sizing.
- Windows 10 & higher support.
- Proper disposing of `HWND` and instances.
- Semi transparent `Widget`s on top of `NativeView`.
- Customizable hit-test i.e. optional interactability with the `NativeView`s.
- Placement of `NativeView`s inside scrollables like `ListView`s.

#### Under Progress

- Finalized API.
- General stability.
- Better maximize/minimize animations.
- Support for older Windows versions.
- Defining z-order for each `NativeViewController`.

## Known Issues

- Windows snapping layouts & window snapping (only when a `NativeView` is visible on screen).
- `HitTestBehavior.translucent` leaks through the title-bar.

## Motivation

https://github.com/flutter/flutter/issues/31713

Absence of official Platform View APIs in Flutter for Windows, Linux & macOS.

## Platforms

The plugin currently works on following platforms:

| Platform | State   |
| -------- | ------- |
| Windows  | Working |

I plan to add Linux support soon. For now, limiting the scope of work to just Windows.

## License

Currently licensing strictly.

GNU General Public License v3.0

Copyright (C) 2021, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>
