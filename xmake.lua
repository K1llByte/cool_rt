-- Package Requirements
add_requires("fmt")
add_requires("glm")
add_requires("libgd")

-------------------- Ray Tracer -------------------

-- Target RayTracer
target("rt")
    -- Common Compiler Options
    set_languages("cxx20") -- -std=c++20
    set_warnings("allextra") -- -Wall -Wextra -Wfatal-errors (if error enabled)
    set_optimize("fastest") -- -O3
    -- FIXME: Not working
    add_cxxflags("-Wshadow", "-Wpedantic")
    set_targetdir("bin/")
    -- Compiler Options
    add_includedirs("src")
    add_includedirs("lib/tiny_png_out")
    -- Dependencies
    add_packages("glm")
    add_packages("fmt")
    -- Library
    set_kind("binary")
    add_files("src/**.cpp")
    add_files("lib/tiny_png_out/TinyPngOut.cpp")