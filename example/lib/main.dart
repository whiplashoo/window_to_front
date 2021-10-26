import 'package:flutter/material.dart';
import 'package:window_to_front/window_to_front.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: TextButton(
          onPressed: () async {
            var previouslyActiveApp = await WindowToFront.activate();
            print(previouslyActiveApp);
          },
          child: Text("Test Platform Method"),
        ),
      ),
    );
  }
}
