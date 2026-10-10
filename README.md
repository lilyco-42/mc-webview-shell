# MC WebView2 Shell

极小 C 语言 WebView2 壳 - Windows 版 23KB Android WebView 替代品

## 与 `webview-mini` 的关系

两者共用同一个 `webview.h`（单头文件 amalgamation，205 KB），区别只在定位：

- **本仓 `mc-webview-shell`** — 场景成品：针对 MC 控制台，附预编译 exe 与 Clash TUN
  白屏绕法，开箱即用。
- **[`webview-mini`](https://github.com/lilyco-42/webview-mini)** — 通用起点：库 + 构建配置 +
  最小示例，STL-free，适合自己起新项目。

## 体积对比

| 平台 | 体积 | 依赖 |
|------|------|------|
| Android WebView | 23 KB | 系统自带 |
| **Windows C 壳** | **191 KB** | WebView2 运行时(Win10+ 自带) |

## 快速开始

### 方式 0: 直接用（推荐）

仓库里已经有预编译产物，**不需要任何工具链**：

```
mc-webview.exe           标准版
mc-webview-noproxy.exe   内置 --no-proxy-server（Clash TUN 场景用这个）
WebView2Loader.dll       可选，放在 exe 旁边即启用官方加载器
```

`WebView2Loader.dll` 不放也能跑：`webview.h` 默认打开了内置的 WebView2Loader
实现（`WEBVIEW_MSWEBVIEW2_BUILTIN_IMPL`，读注册表定位 Edge WebView2 Runtime），
这也是本仓 exe 的导入表里看不到它的原因 —— 它是运行时 `LoadLibrary` 加载的。
放了则优先用 DLL 里的实现。

### 方式 1: xmake（推荐，需要 MinGW）

```bash
xmake
```

`xmake.lua` 从 [lyco-mirror](https://github.com/lilyco-42/xmake-mirror) 取 `webview-mini`
包（`webview.h` + 预编译 MinGW 静态库），**不需要 WebView2 SDK，也不需要 C++ 工具链**。
实测一条 `xmake` 就能产出 GUI 子系统的 `mc-webview.exe`。

包只有 MinGW 版；MSVC 请走方式 2。

### 方式 2: CMake

`main.c` 是 C，而 `webview.h` 在 C 下只给声明，实现要另编一个 C++ 单元 ——
那个单元需要 WebView2 SDK 的 `WebView2.h`：

```bash
cmake -B build -DWEBVIEW_SDK_DIR=<含 WebView2.h 的目录>
cmake --build build
```

不传 `-DWEBVIEW_SDK_DIR` 时 CMake 会直接把上面三条路打印出来，
不会让你编到一半才炸在 `#include "WebView2.h"` 上。

### 方式 3: 命令行手编（MinGW-w64，不需要 SDK）

借 `webview-mini` 里的预编译静态库，不需要 C++ 工具链也不需要 SDK：

```bash
gcc main.c -I. -mwindows -municode -o mc-webview.exe \
    -L../webview-mini/lib -lwebview -lstdc++ \
    -luser32 -lshell32 -lole32 -loleaut32 -lshlwapi -lversion
```

- `-municode`：本文件用的是 `wWinMain`，MinGW 要走 unicode 版 CRT 入口
  （MSVC 用 `/ENTRY:wWinMainCRTStartup`）
- `-lstdc++`：静态库里有 C++ 代码，少了会报 `undefined reference to operator new`

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

`windows.h` 不能省 —— `webview.h` 只在 C++ 下才间接带上它，
纯 C 编译时 `WINAPI`/`HINSTANCE` 会是未定义标识符。

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
| `WebView2Loader.dll` | WebView2 官方加载器 (161 KB)，可选 |
| `main.c` | 最小示例源码 |
| `webview.h` | 单头文件 amalgamation (205 KB) |
| `xmake.lua` | xmake 配置（从 lyco-mirror 取 `webview-mini` 包） |
| `CMakeLists.txt` | CMake 配置（需 `-DWEBVIEW_SDK_DIR`） |

## License

MIT
