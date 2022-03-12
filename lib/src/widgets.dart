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

import 'package:flutter/material.dart';

import 'package:flutter_native_view/src/value_notifiers.dart';
import 'package:flutter_native_view/src/native_view_controller.dart';

class _PaintRemover extends CustomPainter {
  const _PaintRemover();

  @override
  void paint(Canvas canvas, Size size) {
    canvas.drawRect(
      Rect.fromLTWH(0, 0, size.width, size.height),
      Paint()
        ..blendMode = BlendMode.clear
        ..color = Colors.transparent,
    );
  }

  @override
  bool shouldRepaint(oldDelegate) => false;
}

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

class _NativeViewState extends State<NativeView> {
  bool initialized = false;

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance!.addPostFrameCallback((timeStamp) async {
      await widget.controller
          .createNativeView(widget.controller.globalKey.rect!);
      setState(() {
        initialized = true;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return ValueListenableBuilder<int>(
      key: widget.controller.globalKey,
      valueListenable: initializationTypeNotifier,
      builder: (context, initializationType, _) =>
          ValueListenableBuilder<Color>(
        valueListenable: layeredColorNotifier,
        builder: (context, layeredColor, _) => initialized
            ? initializationType == 0
                ? CustomPaint(
                    painter: const _PaintRemover(),
                    size: Size(widget.width, widget.height),
                  )
                : Container(
                    color: layeredColor,
                    width: widget.width,
                    height: widget.height,
                  )
            : Container(
                color: Colors.transparent,
                width: widget.width,
                height: widget.height,
              ),
      ),
    );
  }
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
