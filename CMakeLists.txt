cmake_minimum_required(VERSION 3.20)
project(mc-webview C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 查找 WebView2
find_package(WebView2 QUIET)

add_executable(mc-webview main.c)

if(WebView2_FOUND)
    target_link_libraries(mc-webview PRIVATE WebView2::WebView2)
else()
    # 单头文件模式
    target_sources(mc-webview PRIVATE webview.cc)
endif()

target_link_libraries(mc-webview PRIVATE
    user32 shell32 ole32 oleaut32 shlwapi version
)
