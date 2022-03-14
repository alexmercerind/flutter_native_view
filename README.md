# [flutter_native_view](https://github.com/alexmercerind/flutter_native_view)

Embedding native windows into Flutter window.

## Description

A Flutter plugin / C++ library to embed other native Windows (`HWND` on Windows) into Flutter window.

Current API design allows to embed any arbitrary `HWND` completely from Dart as a `Widget`. This can be a good choice when client code wants to embed any arbitrary third-party window (which is already opened) into the Flutter window.
However, this is not ideal in most cases because there is almost no point of embedding a third-party window directly (which will lack programmatic control via some API).

On the other hand, a window created by the client code itself e.g. a webview instance window or a video-output window etc. (on which the client code has full programmatic control) will be an ideal window to embed, in that case:

- If client code decides to create an `HWND` through platform channel interface, they can use the setup present in [`core`](https://github.com/alexmercerind/flutter_native_view/tree/master/core) to embed a window.
- Since `dart:ffi` is very capable now, one can pass the `HWND` directly as `int` of the window they created using `dart:ffi` or [`win32`](https://github.com/timsneath/win32) through existing plugin API to embed it.

https://user-images.githubusercontent.com/28951144/158262120-3fdd89e4-22b7-4fb2-9c99-98979df06884.mp4

## Example

```dart
class _MyAppState extends State<MyApp> {
  // Create a [NativeViewController].
  final controller = NativeViewController(
    // Using [FindWindow] from `package:win32` to retrieve [HWND] as [int].
    handle: FindWindow(nullptr, 'VLC Media Player'.toNativeUtf16()));

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
                    builder: (context, constraints) => NativeView(
                      // Pass [NativeViewController] to render the window.
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

## Motivation

https://github.com/flutter/flutter/issues/31713

Absence of official Platform View APIs in Flutter for Windows, Linux & macOS.

## Features

#### Currently Implemented

- Placement of other Flutter `Widget`s on top of the `NativeView`.
- Multiple instances of `NativeView`.
- Window movement handling & `NativeView`s positioning.
- Window resize handling & `NativeView`s sizing.
- Windows 8 & higher support.
- Interractions with the `NativeView` e.g. mouse clicks or keyboard presses.

#### Under Progress

- Finalized API.
- Fixing airspace issues.
- Customizable hit-test.
- Proper disposing of `HWND` and instances.
- Efficiently detecting changes in the global positioning of the "cutout".
- Removing Win32 specific types & `ifdef` magic.
- Not sending `Color`s in a `flutter::EncodableMap` ¯\\_(ツ)\_/¯.
- Currently this setup is using `WS_EX_LAYERED` & `SetLayeredWindowAttributes` & it's not good.
  - Ideally, flutter view should be made transparent instead of masking a color to show the `native_view`s underneath (but I'm not talented enough to solve that yet).
  - Right now, I'm not sure if subclassing flutter view `HWND` can somehow solve this issue.
  - FYI https://github.com/flutter/flutter/issues/71735.

## Platforms

The plugin currently works on following platforms:

| Platform | State   |
| -------- | ------- |
| Windows  | Working |
| Linux    | Not Yet |
| macOS    | Not Yet |

## License

Currently licensing strictly.

GNU General Public License v3.0

Copyright (C) 2021, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>
