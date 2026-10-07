# MC WebView2 Shell

极小 C 语言 WebView2 �?- Windows �?23KB Android WebView 替代�?

## �?`webview-mini` 的关�?

两者共用同一�?`webview.h`（单头文�?amalgamation�?05 KB），区别只在定位�?

- **本仓 `mc-webview-shell`** �?场景成品：针�?MC 控制台，附预编译 exe �?Clash TUN
  白屏绕法，开箱即用�?
- **[`webview-mini`](https://github.com/lilyco-42/webview-mini)** �?通用起点：库 + 构建配置 +
  最小示例，STL-free，适合自己起新项目�?

## 体积对比

| 平台 | 体积 | 依赖 |
|------|------|------|
| Android WebView | 23 KB | 系统自带 |
| **Windows C �?* | **191 KB** | WebView2 运行�?Win10+ 自带) |

## 快速开�?

### 方式 0: 直接用（推荐�?

仓库里已经有预编译产物，**不需要任何工具链**�?

```
mc-webview.exe           标准�?
mc-webview-noproxy.exe   内置 --no-proxy-server（Clash TUN 场景用这个）
WebView2Loader.dll       可选，放在 exe 旁边即启用官方加载器
```

`WebView2Loader.dll` 不放也能跑：`webview.h` 默认打开了内置的 WebView2Loader
实现（`WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL`，读注册表定�?Edge WebView2 Runtime），
这也是本�?exe 的导入表里看不到它的原因 —�?它是运行�?`LoadLibrary` 加载的�?
放了则优先用 DLL 里的实现�?

### 方式 1: xmake（推荐，需�?MinGW�?

```bash
xmake
```

`xmake.lua` �?[lyco-mirror](https://github.com/lilyco-42/xmake-mirror) �?`webview-mini`
包（`webview.h` + 预编�?MinGW 静态库），**不需�?WebView2 SDK，也不需�?C++ 工具�?*�?
实测一�?`xmake` 就能产出 GUI 子系统的 `mc-webview.exe`�?

包只�?MinGW 版；MSVC 请走方式 2�?

### 方式 2: CMake

`main.c` �?C，�?`webview.h` �?C 下只给声明，实现要另编一�?C++ 单元 —�?
那个单元需�?WebView2 SDK �?`WebView2.h`�?

```bash
cmake -B build -DWEBVIEW_SDK_DIR=<�?WebView2.h 的目�?
cmake --build build
```

不传 `-DWEBVIEW_SDK_DIR` �?CMake 会直接把上面三条路打印出来，
不会让你编到一半才炸在 `#include "WebView2.h"` 上�?

### 方式 3: 命令行手编（MinGW-w64，不需�?SDK�?

�?`webview-mini` 里的预编译静态库，不需�?C++ 工具链也不需�?SDK�?

```bash
gcc main.c -I. -mwindows -municode -o mc-webview.exe \
    -L../webview-mini/lib -lwebview -lstdc++ \
    -luser32 -lshell32 -lole32 -loleaut32 -lshlwapi -lversion
```

- `-municode`：本文件用的�?`wWinMain`，MinGW 要走 unicode �?CRT 入口
  （MSVC �?`/ENTRY:wWinMainCRTStartup`�?
- `-lstdc++`：静态库里有 C++ 代码，少了会�?`undefined reference to operator new`

## 代码

```c
#include <windows.h>   // HINSTANCE / PWSTR / WINAPI 都在这里
#include "webview.h"

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

`windows.h` 不能�?—�?`webview.h` 只在 C++ 下才间接带上它，
�?C 编译�?`WINAPI`/`HINSTANCE` 会是未定义标识符�?

## Clash TUN 代理问题

�?WebView2 白屏(网络�?Clash TUN 阻断),�?`webview_create` 前添�?

```c
SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", L"--no-proxy-server");
```

或使用预编译�?`mc-webview-noproxy.exe`(已内�?�?

## 文件说明

| 文件 | 说明 |
|------|------|
| `mc-webview.exe` | 标准�?(191 KB) |
| `mc-webview-noproxy.exe` | 绕过代理�?(192 KB) |
| `WebView2Loader.dll` | WebView2 官方加载�?(161 KB)，可�?|
| `main.c` | 最小示例源�?|
| `webview.h` | 单头文件 amalgamation (205 KB) |
| `xmake.lua` | xmake 配置（从 lyco-mirror �?`webview-mini` 包） |
| `CMakeLists.txt` | CMake 配置（需 `-DWEBVIEW_SDK_DIR`�?|

## License

MIT
