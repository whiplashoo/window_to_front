import Cocoa
import FlutterMacOS

public class WindowToFrontPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "window_to_front", binaryMessenger: registrar.messenger)
    let instance = WindowToFrontPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "activate":
      let res = NSWorkspace.shared.frontmostApplication?.localizedName
      let mainWindow = NSApp.windows[0]
      if (mainWindow.isMiniaturized) {
        mainWindow.deminiaturize(self);
      }
      NSApplication.shared.activate(ignoringOtherApps: true)
      result(res)
    default:
      result(FlutterMethodNotImplemented)
    }
  }
}
