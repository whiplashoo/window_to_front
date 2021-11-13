#include "include/window_to_front/window_to_front_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <string>
#include <malloc.h>

namespace {
using flutter::EncodableValue;

class WindowToFrontPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  WindowToFrontPlugin(flutter::PluginRegistrarWindows *registrar);

  virtual ~WindowToFrontPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<>> result);

  // The registrar for this plugin, for accessing the window.
  flutter::PluginRegistrarWindows *registrar_;
};

// static
void WindowToFrontPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<>>(
          registrar->messenger(), "window_to_front",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<WindowToFrontPlugin>(registrar);

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WindowToFrontPlugin::WindowToFrontPlugin(flutter::PluginRegistrarWindows *registrar)
  : registrar_(registrar) {}

WindowToFrontPlugin::~WindowToFrontPlugin() {}

void WindowToFrontPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("activate") == 0) {
    HWND m_hWnd = registrar_->GetView()->GetNativeWindow();
    HWND hCurWnd = ::GetForegroundWindow();
    std::string appName;
    int len = GetWindowTextLengthA(hCurWnd) + 1;
	if (len != 0) {
		char *text = (char *)alloca(len);
		len = GetWindowTextA(hCurWnd, text, len);
		appName = std::string(text, len);
	}
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow(m_hWnd);
    ::SetFocus(m_hWnd);
    ::SetActiveWindow(m_hWnd);
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    result->Success(EncodableValue(appName));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void WindowToFrontPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  WindowToFrontPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}