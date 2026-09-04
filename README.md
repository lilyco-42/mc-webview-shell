# MC WebView2 Shell

极小 C 语言 WebView2 壳 - Windows 版 23KB Android WebView 替代品

## 体积对比

| 平台 | 体积 | 依赖 |
|------|------|------|
| Android WebView | 23 KB | 系统自带 |
| **Windows C 壳** | **191 KB** | WebView2 运行时(Win10+ 自带) |

## 单头文件引入

```c
#include "webview.h"  // 单头文件,无需额外依赖

int WINAPI wWinMain(HINSTANCE hi, HINSTANCE hr, PWSTR cmd, int show) {
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "MC Console");
    webview_set_size(w, 1100, 760, WEBVIEW_HINT_NONE);
    webview_navigate(w, "http://192.168.10.165:8765");
    webview_run(w);
    webview_destroy(w);
    return 0;
}
```

## 快速入门

### 方式 1: xmake (推荐)

```bash
xmake create -t console test
cd test
# 复制 main.c 到 src/
xmake
```

### 方式 2: CMake

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### 方式 3: 单文件编译

```bash
# 使用 amalgamate 后的 webview.h
cl /O2 /MT main.c /link /SUBSYSTEM:WINDOWS /ENTRY:wWinMainCRTStartup webview_static.lib ...
```

## Clash TUN 代理问题

若 WebView2 白屏(网络被 Clash TUN 阻断),在 `webview_create` 前添加:

```c
SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", L"--no-proxy-server");
```

或使用预编译的 `mc-webview-noproxy.exe`(已内置)。

## 文件说明

| 文件 | 说明 |
|------|------|
| `mc-webview.exe` | 标准版 (191 KB) |
| `mc-webview-noproxy.exe` | 绕过代理版 (192 KB) |
| `main.c` | 最小示例源码 |
| `webview.h` | 单头文件 amalgamation (211 KB) |
| `xmake.lua` | xmake 一键引入 |
| `CMakeLists.txt` | CMake 配置 |

## License

MIT
