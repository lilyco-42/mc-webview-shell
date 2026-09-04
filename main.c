// MC WebView2 Shell - 极小 WebView2 壳
// 单头文件引入: 只依赖 webview.h + webview.cc
// 编译: cl /O2 /MT main.c webview.cc /link /SUBSYSTEM:WINDOWS /ENTRY:wWinMainCRTStartup webview_static.lib ...
// 或: 直接包含 amalgamate 后的 webview.h
#include "webview.h"

int WINAPI wWinMain(HINSTANCE hi, HINSTANCE hr, PWSTR cmd, int show) {
    // 可选:绕过代理(解决 Clash TUN 阻断 WebView2 网络)
    // SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", L"--no-proxy-server");
    
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "MC Console");
    webview_set_size(w, 1100, 760, WEBVIEW_HINT_NONE);
    webview_navigate(w, "http://192.168.10.165:8765");
    webview_run(w);
    webview_destroy(w);
    return 0;
}
