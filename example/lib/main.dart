import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:win32/win32.dart';
import 'package:flutter/material.dart';

import 'package:flutter_native_view/flutter_native_view.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await FlutterNativeView.ensureInitialized();
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final controller = ScrollController();
  final controllers = [
    NativeViewController(
      handle: FindWindow(
        nullptr,
        'VLC Media Player'.toNativeUtf16(),
      ),
      hitTestBehavior: HitTestBehavior.translucent,
    ),
    NativeViewController(
      handle: FindWindow(
        nullptr,
        'This PC'.toNativeUtf16(),
      ),
      hitTestBehavior: HitTestBehavior.translucent,
    ),
  ];

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('flutter_native_view'),
        ),
        body: ListView(
          controller: controller,
          padding: const EdgeInsets.symmetric(vertical: 16.0),
          children: [
            Padding(
              padding:
                  const EdgeInsets.symmetric(vertical: 16.0, horizontal: 32.0),
              child: NativeView(
                controller: controllers[0],
                width: 640.0,
                height: 480.0,
              ),
            ),
            Padding(
              padding:
                  const EdgeInsets.symmetric(vertical: 16.0, horizontal: 32.0),
              child: NativeView(
                controller: controllers[1],
                width: 640.0,
                height: 480.0,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
