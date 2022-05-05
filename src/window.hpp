#pragma once

#include "render_target.hpp"
#include "image.hpp"

#include <GL/gl.h>
#include <glm/glm.hpp>

#include <fmt/core.h>

// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

class GuiWindow : public abc::RenderTarget
{
    private:
    size_t      width;
    size_t      height;
    GLFWwindow* window;
    // Internal state
    bool   show_demo_window = true;
    bool   show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Image to render to
    Image  framebuffer;
    GLuint image_texture;
    
    public:
    GuiWindow(size_t _width, size_t _height);
    ~GuiWindow();

    constexpr void set_pixel(size_t x, size_t y, glm::vec<3,uint8_t> value) override
    {
        framebuffer.set_pixel(x,y,value);
    }

    constexpr glm::vec<3,uint8_t> get_pixel(size_t x, size_t y) const override
    {
        return glm::vec<3,uint8_t>{0,0,0};
    }

    constexpr size_t get_width() const override { return width; }
    constexpr size_t get_height() const override { return width; }


    bool should_close() const { return glfwWindowShouldClose(window); }
    void present();
    void set_theme();
};