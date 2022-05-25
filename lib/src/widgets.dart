// This file is a part of libmpv.dart (https://github.com/alexmercerind/libmpv.dart).
//
// Copyright (c) 2022, Hitesh Kumar Saini <saini123hitesh@gmail.com>.
// All rights reserved.
// Use of this source code is governed by MIT license that can be found in the LICENSE file.
import 'package:flutter/widgets.dart';

import 'package:flutter_native_view/src/ffi.dart';
import 'package:flutter_native_view/src/native_view_controller.dart';

/// NativeView
/// ----------
/// Create a [NativeView] & pass [NativeViewController] as controller to render it's window.
///
/// ```dart
/// class _MyAppState extends State<MyApp> {
///   final controller = NativeViewController(
///     handle: FindWindow(nullptr, 'VLC Media Player'.toNativeUtf16()));
///
///   @override
///   Widget build(BuildContext context) {
///     return MaterialApp(
///       home: Scaffold(
///         body: Center(
///             child: Padding(
///               padding: const EdgeInsets.all(24.0),
///               child: Stack(
///                 children: [
///                   LayoutBuilder(
///                     builder: (context, constraints) => NativeView(
///                       // Pass [NativeViewController] to render the window.
///                       controller: controller,
///                       width: constraints.maxWidth,
///                       height: constraints.maxHeight,
///                     ),
///                   ),
///                   Padding(
///                     padding: const EdgeInsets.all(16.0),
///                     child: FloatingActionButton(
///                       onPressed: () {},
///                       child: const Icon(Icons.edit),
///                     ),
///                   ),
///                 ],
///               ),
///             ),
///           ),
///         ),
///       ),
///     );
///   }
/// }
/// ```
class NativeView extends StatefulWidget {
  final NativeViewController controller;
  final double width;
  final double height;
  const NativeView({
    Key? key,
    required this.controller,
    required this.width,
    required this.height,
  }) : super(key: key);

  @override
  State<NativeView> createState() => _NativeViewState();
}

class _NativeViewState extends State<NativeView>
    with AutomaticKeepAliveClientMixin {
  @override
  void initState() {
    super.initState();
    _ambiguate(WidgetsBinding.instance)!.addPostFrameCallback(
      (_) {
        widget.controller.createNativeView();
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    super.build(context);
    return _NativeViewHolder(
      key: widget.controller.rendererKey,
      controller: widget.controller,
      child: MouseRegion(
        onEnter: (_) {
          if (widget.controller.hitTestBehavior ==
              HitTestBehavior.translucent) {
            FFI.nativeViewCoreSetHitTestBehavior(1);
            setState(() {
              widget.controller.entered = true;
            });
          }
        },
        onExit: (_) {
          if (widget.controller.hitTestBehavior ==
              HitTestBehavior.translucent) {
            setState(() {
              widget.controller.entered = false;
            });
          }
        },
        child: CustomPaint(
          key: widget.controller.painterKey,
          painter: const _NativeViewPainter(),
          size: Size(
            widget.width,
            widget.height,
          ),
        ),
      ),
    );
  }

  @override
  bool get wantKeepAlive => true;
}

class _NativeViewHolder extends StatefulWidget {
  final NativeViewController controller;
  final Widget child;
  const _NativeViewHolder({
    Key? key,
    required this.controller,
    required this.child,
  }) : super(key: key);

  @override
  State<_NativeViewHolder> createState() => _NativeViewHolderState();
}

class _NativeViewHolderState extends State<_NativeViewHolder> {
  @override
  Widget build(BuildContext context) {
    widget.controller.resizeNativeViewStreamController.add(null);
    return widget.child;
  }
}

class _NativeViewPainter extends CustomPainter {
  const _NativeViewPainter();

  @override
  void paint(Canvas canvas, Size size) {
    canvas.drawRect(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Paint()
        ..blendMode = BlendMode.clear
        ..color = const Color(0x00000000),
    );
  }

  @override
  bool shouldRepaint(_) => false;
}

extension GlobalKeyExtension on GlobalKey {
  Rect? get rect {
    final renderObject = currentContext?.findRenderObject();
    final translation = renderObject?.getTransformTo(null).getTranslation();
    if (translation != null && renderObject?.paintBounds != null) {
      final offset = Offset(translation.x, translation.y);
      return renderObject!.paintBounds.shift(offset);
    } else {
      return null;
    }
  }
}

T? _ambiguate<T>(T? value) => value;
