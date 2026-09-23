// MC WebView2 Shell - 极小 WebView2 壳
//
// 构建：xmake（自动拉取含 WebView2 SDK 的上游 webview 包）
//   或者直接用仓库根目录的预编译产物 mc-webview.exe / mc-webview-noproxy.exe
//
// 注意 webview.h 是 webview/webview 的 amalgamation：
//   · C++ 侧含实现，但需要 WebView2 SDK 的 WebView2.h
//   · C 侧（本文件）只拿到声明 —— WEBVIEW_API 展开成 extern，
//     实现来自另编的 C++ 单元，所以不能只 include 头文件就完事。
#include <windows.h>   // HINSTANCE / PWSTR / WINAPI 都在这里
#include "webview.h"

int WINAPI wWinMain(HINSTANCE hi, HINSTANCE hr, PWSTR cmd, int show) {
    (void)hi; (void)hr; (void)cmd; (void)show;

    // 可选:绕过代理(解决 Clash TUN 阻断 WebView2 网络)
    // SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", L"--no-proxy-server");

    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "MC Console");
    webview_set_size(w, 1100, 760, WEBVIEW_HINT_NONE);
    webview_navigate(w, "http://192.168.10.165:8765");  // 改成你自己的控制台地址
    webview_run(w);
    webview_destroy(w);
    return 0;
}
