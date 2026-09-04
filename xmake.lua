add_rules("mode.debug", "mode.release")
add_requires("webview", {configs = {runtime = "static"}})

target("mc-webview")
    set_kind("binary")
    add_files("main.c")
    add_packages("webview")
    add_syslinks("user32", "shell32", "ole32", "oleaut32", "shlwapi", "version")
    set_languages("c11")
    if is_mode("release") then
        set_optimize("smallest")
    end
target_end()
