import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';

import 'package:flutter_native_view/flutter_native_view.dart';
import 'package:win32/win32.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  FlutterNativeView.ensureInitialized();
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final controller = NativeViewController(
    windowHandle: FindWindow(
      'Notepad'.toNativeUtf16(),
      nullptr,
    ),
  );

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: NativeView(
            controller: controller,
            width: 640.0,
            height: 480.0,
          ),
        ),
      ),
    );
  }
}
