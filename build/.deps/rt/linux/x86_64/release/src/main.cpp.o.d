{
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Wall",
            "-Wextra",
            "-O3",
            "-std=c++20",
            "-Isrc",
            "-isystem",
            "/home/kb/.xmake/packages/g/glm/0.9.9+8/65b1ad153bda4a43b0454eba7969327f/include",
            "-isystem",
            "/home/kb/.xmake/packages/f/fmt/8.1.1/212ea05ee1514fb8bc1d6d9f35de8988/include",
            "-Wshadow",
            "-Wpedantic"
        }
    },
    depfiles_gcc = "build/.objs/rt/linux/x86_64/release/src/main.cpp.o: src/main.cpp  src/ray.hpp src/intersection.hpp src/sphere.hpp src/camera.hpp  src/utils.hpp src/scene.hpp\
",
    files = {
        "src/main.cpp"
    }
}