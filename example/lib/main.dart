import 'package:flutter/material.dart';

import 'package:flutter_native_view/flutter_native_view.dart';

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
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: const Center(
          child: NativeView(
            width: 640.0,
            height: 480.0,
          ),
        ),
      ),
    );
  }
}
