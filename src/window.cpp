#include "window.hpp"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void GuiWindow::set_theme()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    // Palette
    // const auto AQUA         = ImVec4(0.408f, 0.616f, 0.416f, 1.f); // 689d6a
    const auto DARK_GREY       = ImVec4(0.06f, 0.05f, 0.07f, 1.00f); // 0f0d12
    const auto DARK_GREY_HOVER = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    const auto GREY            = ImVec4(0.10f, 0.09f, 0.12f, 1.00f); // 1a171f
    const auto GREY_HOVER      = ImVec4(0.56f, 0.56f, 0.58f, 1.00f); // 8f8f94

    style->WindowPadding     = ImVec2(15, 15);
    style->WindowRounding    = 5.0f;
    style->FramePadding      = ImVec2(5, 5);
    style->FrameRounding     = 4.0f;
    style->ItemSpacing       = ImVec2(12, 8);
    style->ItemInnerSpacing  = ImVec2(8, 6);
    style->IndentSpacing     = 25.0f;
    style->ScrollbarSize     = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize       = 5.0f;
    style->GrabRounding      = 3.0f;

    style->Colors[ImGuiCol_Text]                  = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_WindowBg]              = DARK_GREY;
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]               = GREY;
    style->Colors[ImGuiCol_FrameBgHovered]        = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_FrameBgActive]         = GREY_HOVER;
    style->Colors[ImGuiCol_TitleBg]               = GREY;
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]             = GREY;
    style->Colors[ImGuiCol_ScrollbarBg]           = GREY;
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = GREY_HOVER;
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = DARK_GREY;
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive]      = DARK_GREY;
    style->Colors[ImGuiCol_Button]                = GREY;
    style->Colors[ImGuiCol_ButtonHovered]         = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_ButtonActive]          = GREY_HOVER;
    style->Colors[ImGuiCol_Header]                = GREY;
    style->Colors[ImGuiCol_HeaderHovered]         = GREY_HOVER;
    style->Colors[ImGuiCol_HeaderActive]          = DARK_GREY;
    style->Colors[ImGuiCol_Separator]             = GREY_HOVER;
    style->Colors[ImGuiCol_SeparatorHovered]      = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_SeparatorActive]       = GREY_HOVER;

    style->Colors[ImGuiCol_Tab]                   = DARK_GREY;
    style->Colors[ImGuiCol_TabHovered]            = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_TabActive]             = GREY;
    style->Colors[ImGuiCol_TabUnfocused]          = DARK_GREY;
    style->Colors[ImGuiCol_TabUnfocusedActive]    = GREY;

    style->Colors[ImGuiCol_DockingPreview]        = DARK_GREY_HOVER;
    style->Colors[ImGuiCol_DockingEmptyBg]        = DARK_GREY_HOVER;

    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = GREY_HOVER;
    style->Colors[ImGuiCol_ResizeGripActive]      = DARK_GREY;
    // style->Colors[ImGuiCol_CloseButton]        = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    // style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    // style->Colors[ImGuiCol_CloseButtonActive]  = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    // style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

GuiWindow::GuiWindow(size_t _width, size_t _height)
        : width(_width)
        , height(_height)
        , framebuffer(_width, _height)
{
// Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
        throw 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // SETUP IMAGE TO RENDER TO
    // Create a OpenGL texture identifier
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer.width, framebuffer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<void*>(framebuffer.data));

    // Create window with graphics context
    window = glfwCreateWindow(width, height, "Cool RayTracer", NULL, NULL);
    if (window == NULL)
        throw 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    io.Fonts->AddFontFromFileTTF("/tmp/Ruda-VariableFont_wght.ttf", 16.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    set_theme();
}

GuiWindow::~GuiWindow()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void GuiWindow::present()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        // glBindTexture(GL_TEXTURE_2D, image_texture);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer.width, framebuffer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<void*>(framebuffer.data));
        ImGui::Image((void*)(intptr_t)image_texture, ImVec2(framebuffer.width, framebuffer.height));

        // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}