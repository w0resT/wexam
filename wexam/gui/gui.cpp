#include <iostream>

#include "../defines.h"
#include "gui.h"
#include "imguimanager.h"

#include "tabs.h"

// Fonts
#include "../resources/segoeui.h"
#include "../resources/segoeuib.h"

void Gui::Init() {
    // Error callback func
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[error] GLFW (" << error << "): " << description << std::endl;
        });

	// Initializing GLFW
    // TODO: Make assert
    if (!glfwInit()) {
        std::cerr << "[error] Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // GL 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Creating window
    m_GLFWWindow = glfwCreateWindow(defines::WINDOW_W, defines::WINDOW_H, defines::WINDOW_TITLE, nullptr, nullptr);

    // TODO: Make assert 
    if (!m_GLFWWindow) {
        std::cerr << "[error] Failed to create window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_GLFWWindow);

    // Enable vsync
    glfwSwapInterval(1); 

    // Key Callback function
    glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Key 'Delete' for exit
        if (key == GLFW_KEY_DELETE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        });

    // Set window resize event handler
    glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    // Initializing ImGui manager
    m_ImguiManager = std::make_unique<ImGuiManager>(m_GLFWWindow);
    m_ImguiManager->Init();

    // Load style
    LoadStyle();

    // Create and init fonts
    CreateFonts();

    // Initializing tab manager
    m_TabManager = std::make_unique<TabManager>();

    // Creating and adding tabs to tab manager
    CreateTabs();
}

void Gui::Run() {
    // Main loop
    while (!m_bShouldClose) {
        m_bShouldClose = glfwWindowShouldClose(m_GLFWWindow);

        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Updating ImGui
        m_ImguiManager->NewFrame();

        // Showing the demo window
        //ImGui::ShowDemoWindow();

        // Showing main window
        Draw();

        // Rendering ImGui & Window
        m_ImguiManager->Render();
        glfwSwapBuffers(m_GLFWWindow);
    }

    // Notify observers that the app is shutting down
    NotifyObservers();
}

void Gui::Shutdown() {
    // Cleanup
    m_ImguiManager->Shutdown();
    glfwDestroyWindow(m_GLFWWindow);
    glfwTerminate();
}

void Gui::Draw() {
    if (ImGui::Begin("wxm_begin", nullptr)) {
        ImGui::Text("Test");

        ImGui::PushFont(m_Segoeui18);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();

        ImGui::PushFont(m_SegoeuiBold18);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();

        ImGui::PushFont(m_SegoeuiBold32);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();

        m_TabManager->Draw();
    }
    ImGui::End();
}

void Gui::CreateTabs() {
    std::unique_ptr<ITabView> tabTests = std::make_unique<TabTests>(m_Model);
    std::unique_ptr<ITabView> tabUsers = std::make_unique<TabUsers>(m_Model);
    std::unique_ptr<ITabView> tabSettings = std::make_unique<TabSettings>(m_Model);
    std::unique_ptr<ITabView> tabInfo = std::make_unique<TabInfo>(m_Model);

    m_TabManager->AddTab(std::move(tabTests));
    m_TabManager->AddTab(std::move(tabUsers));
    m_TabManager->AddTab(std::move(tabSettings));
    m_TabManager->AddTab(std::move(tabInfo));
}

void Gui::CreateFonts() {
    ImGuiIO& io = ImGui::GetIO();

    m_Segoeui18 = io.Fonts->AddFontFromMemoryCompressedTTF(fonts::segoeui_compressed_data, fonts::segoeui_compressed_size, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_Segoeui18 != nullptr);

    m_SegoeuiBold18 = io.Fonts->AddFontFromMemoryCompressedTTF(fonts::segoeuib_compressed_data, fonts::segoeuib_compressed_size, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_SegoeuiBold18 != nullptr);

    m_SegoeuiBold32 = io.Fonts->AddFontFromMemoryCompressedTTF(fonts::segoeuib_compressed_data, fonts::segoeuib_compressed_size, 32.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_SegoeuiBold32 != nullptr);
}

void Gui::LoadStyle() {
    ImGui::StyleColorsDark();
}

