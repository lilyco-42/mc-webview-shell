cmake_minimum_required(VERSION 3.20)
project(mc-webview C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# webview.h 是 webview/webview 的 amalgamation：
#   · C 侧（main.c）只拿到声明 —— WEBVIEW_API 在 C 下展开成 extern
#   · 实现要另编一个 C++ 单元，而那个单元需要 WebView2 SDK 的 WebView2.h
# 所以要么给 SDK 路径，要么用 xmake（自动拉取含 SDK 的上游 webview 包）。
set(WEBVIEW_SDK_DIR "" CACHE PATH "包含 WebView2.h 的目录（NuGet 包 Microsoft.Web.WebView2）")

if(NOT WEBVIEW_SDK_DIR)
    message(FATAL_ERROR
        "构建 mc-webview 需要 WebView2 SDK 的 WebView2.h。三条路：\n"
        "  1) 不构建，直接用仓库里的预编译产物\n"
        "       mc-webview.exe / mc-webview-noproxy.exe\n"
        "  2) 用 xmake（自动拉取上游 webview 包，含 SDK）\n"
        "       xmake\n"
        "  3) 装了 Microsoft.Web.WebView2 NuGet 包后指定头目录\n"
        "       cmake -B build -DWEBVIEW_SDK_DIR=<含 WebView2.h 的目录>\n"
        "       cmake --build build")
endif()

add_executable(mc-webview main.c)
set_target_properties(mc-webview PROPERTIES WIN32_EXECUTABLE ON)

# 生成实现单元（等价于上游 amalgamation 里的 webview.cc）
set(_impl "${CMAKE_CURRENT_BINARY_DIR}/webview_impl.cpp")
file(WRITE "${_impl}" "#define WEBVIEW_STATIC 1\n#include \"webview.h\"\n")
set_source_files_properties(${_impl} PROPERTIES LANGUAGE CXX)

target_sources(mc-webview PRIVATE ${_impl})
target_include_directories(mc-webview PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${WEBVIEW_SDK_DIR}
)

target_link_libraries(mc-webview PRIVATE
    user32 shell32 ole32 oleaut32 shlwapi version
)

# main.c 用的是 wWinMain，MinGW 需要 unicode CRT 入口（MSVC 自己会认）
if(MINGW)
    target_link_options(mc-webview PRIVATE -municode)
endif()
