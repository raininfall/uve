add_rules("mode.debug", "mode.release")

add_repositories("me git@gitee.com:raininfall/xmake-repo.git")
add_requires("gtest")
add_requires("libuv")

target("uve")
    set_kind("static")
    add_packages("libuv")
    add_includedirs("include")
    add_headerfiles("include/(**.h)")
    add_files("src/*.c")

    if (is_plat("windows")) then
	    add_cxflags("/MT")
        add_files("src/win/*.c")
    end

    if is_mode("debug") then
        add_defines("DEBUG")
        set_symbols("debug")
        set_optimize("none")
    end

target("test-uve")
    set_kind("binary")
    add_packages("gtest")
    add_deps("uve")
    add_includedirs("include")
    add_files("test/*.cpp")
    
    if (is_plat("windows")) then
	    add_cxflags("/MT")
    end

    if is_mode("debug") then
        add_defines("DEBUG")
        set_symbols("debug")
        set_optimize("none")
    end