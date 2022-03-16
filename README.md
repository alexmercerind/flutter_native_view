# [flutter_native_view](https://github.com/alexmercerind/flutter_native_view)

Embedding native windows into Flutter window.

## Description

A Flutter plugin / C++ library to embed other native Windows (`HWND` on Windows) into Flutter window.

Current API design allows to embed any arbitrary `HWND` completely from Dart as a `Widget`. This can be a good choice when client code wants to embed any arbitrary third-party window (which is already opened) into the Flutter window.
However, this is not ideal in most cases because there is almost no point of embedding a third-party window directly (which will lack programmatic control via some API).

On the other hand, a window created by the client code itself e.g. a webview instance window or a video-output window etc. (on which the client code has full programmatic control) will be an ideal window to embed, in that case:

- If client code decides to create an `HWND` through platform channel interface, they can use the setup present in [`core`](https://github.com/alexmercerind/flutter_native_view/tree/master/core) to embed a window.
- Since `dart:ffi` is very capable now, one can pass the `HWND` directly as `int` of the window they created using `dart:ffi` or [`win32`](https://github.com/timsneath/win32) through existing plugin API to embed it.

https://user-images.githubusercontent.com/28951144/158442361-6970e676-fa0d-430d-8eee-b3d862b2682c.mp4

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

Try running the [example](./example) application by cloning the repository.

## Motivation

https://github.com/flutter/flutter/issues/31713

Absence of official Platform View APIs in Flutter for Windows, Linux & macOS.

## Sponsor

You may sponsor this project at [Patreon](https://www.patreon.com/harmonoid) or make voluntary donations at [PayPal](https://www.paypal.me/alexmercerind).

### 💖 Current Sponsors

- [Ahmad Arif Aulia Sutarman](https://github.com/damywise)

## Features

#### Currently Implemented

- Placement of other Flutter `Widget`s on top of the `NativeView`.
- Multiple instances of `NativeView`.
- Window movement handling & `NativeView`s positioning.
- Window resize handling & `NativeView`s sizing.
- Windows 8 & higher support.
- Interractions with the `NativeView` e.g. mouse clicks or keyboard key presses.
- Proper disposing of `HWND` and instances.

#### Under Progress

- Finalized API.
- Customizable hit-test i.e. optional interactability with the `NativeView`s.
- Semi transparent `Widget`s on top of `NativeView`.
- Efficiently detecting changes in global positioning of `NativeView` `Widget`s.
- Removing Win32 specific types & `ifdef` magic.
- Defining z-order for each `NativeViewController`.
- Ideally, flutter view should be made transparent instead of masking a color to show the `native_view`s underneath (but I'm not talented enough to solve that yet).
  - Right now, I'm not sure if subclassing flutter view `HWND` can somehow solve this issue.
  - FYI https://github.com/flutter/flutter/issues/71735.

## Platforms

The plugin currently works on following platforms:

| Platform | State   |
| -------- | ------- |
| Windows  | Working |

## License

Currently licensing strictly.

GNU General Public License v3.0

Copyright (C) 2021, Hitesh Kumar Saini <<saini123hitesh@gmail.com>>
