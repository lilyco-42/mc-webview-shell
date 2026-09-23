add_rules("mode.debug", "mode.release")

-- 走 lyco-mirror 的 webview-mini 包：装 webview.h + 预编译 MinGW 静态库，
-- 并自动补 webview / stdc++ 链接项（见 xmake-mirror 的配方）。
--
-- 注意：官方 xmake-repo 里没有 webview 这个包，写 add_requires("webview") 会直接报
-- "package(webview) not found" —— 必须显式加下面这个仓库。
add_repositories("lyco-mirror https://github.com/lilyco-42/xmake-mirror.git")
add_requires("webview-mini")

target("mc-webview")
    set_kind("binary")
    add_files("main.c")
    add_packages("webview-mini")
    add_syslinks("user32", "shell32", "ole32", "oleaut32", "shlwapi", "version")
    set_languages("c11")
    if is_plat("mingw") then
        -- main.c 用的是 wWinMain：
        --   -municode → unicode 版 CRT 入口，否则链接报 undefined reference to `WinMain'
        --   -mwindows → GUI 子系统，否则会多弹一个控制台窗口
        -- xmake 会先探测这些标志「有没有用」，探测失败就静默丢掉，所以要 force。
        add_ldflags("-mwindows", "-municode", {force = true})
    end
    if is_mode("release") then
        set_optimize("smallest")
    end
target_end()
