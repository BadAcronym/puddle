---@diagnostic disable: undefined-global, undefined-field

workspace("puddle")
    configurations({"debug", "asan", "release"})
    platforms{"linux", "windows"}
    location("build")
    architecture("x86_64")

project("svtest")
    language("C")
    cdialect("C99")
    warnings("Extra")
    targetname("svtest")
    kind("ConsoleApp")
    toolset("clang")
    buildoptions({"-Wextra", "-Wall", "-Wpedantic", "-Wconversion", "-Wshadow", "-Wformat"})

    filter("configurations:asan")
        defines{"ASAN"}

    filter("configurations:debug")
        defines{"DEBUG"}

    filter("configurations:debug or asan")
        staticruntime("off")
        runtime("debug")
        symbols("On")
        buildoptions({"-gfull", "-O1"})
        linkoptions({"-gfull", "-O1"})

    filter("configurations:release")
        defines{"NDEBUG"}
        staticruntime("off")
        runtime("release")
        symbols("Off")
        optimize("Speed")

    filter("platforms:linux")
        system("linux")
        defines("BUILD_LINUX")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/svtest/")
        files({ "./src/linux_string_view*",
                "./include/linux_string_view*",
                "./src/string_view*",
                "./include/string_view*" })
        includedirs({ "./include/", "/usr/include/", "./vendor/"})
        libdirs("./bin/%{cfg.buildcfg}/")
        linkoptions({"-fuse-ld=mold", "-lm"})

    filter("platforms:windows")
        system("windows")
        defines("BUILD_WINDOWS")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/")
        files({ "./src/win32_string_view*",
                "./include/win32_string_view*",
                "./src/string_view*",
                "./include/string_view*" })
        includedirs({"./include/"})
        libdirs("./bin/%{cfg.buildcfg}/")
        linkoptions({"-fuse-ld=lld"})

    filter({"platforms:linux", "configurations:asan"})
        buildoptions({"-fsanitize=address,leak,undefined", "-fno-omit-frame-pointer",
                      "-static-libasan"})
        linkoptions({"-fsanitize=address,leak,undefined", "-fno-omit-frame-pointer",
                     "-static-libasan"})

    filter({"platforms:windows", "configurations:debug or asan"})
        buildoptions("-gcodeview");
        linkoptions("-gcodeview");
        kind("ConsoleApp")

    filter({"platforms:windows", "configurations:asan"})
        toolset("clang-cl")
        buildoptions({"/fsanitize=address", "/Zi", "/INCREMENTAL:NO"})
        linkoptions{"/link clang_rt.asan_dynamic-x86_64.lib clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"}
        editandcontinue("Off")

    -- filter({"platforms:windows", "configurations:release"})
        -- linkoptions({"/NODEFAULTLIB:MSVCRTD"})
