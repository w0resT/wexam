#include "../defines.h"
#include "gui.h"
#include "imguimanager.h"

#include <iostream>

void Gui::Init() {
	// Initializing GLFW
    // TODO: Make assert
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // GL 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Creating window
    m_GLFWWindow = glfwCreateWindow(defines::WINDOW_W, defines::WINDOW_H, defines::WINDOW_TITLE, nullptr, nullptr);

    // TODO: Make assert 
    if (!m_GLFWWindow) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_GLFWWindow);

    // Enable vsync
    glfwSwapInterval(1); 

    // Initializing ImGui manager
    m_ptrImguiManager = std::make_unique<ImGuiManager>(m_GLFWWindow);
    m_ptrImguiManager->Init();

    // Callback-function for GLFW
    glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        // Key 'Delete' for exit
        if (key == GLFW_KEY_DELETE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        });
}

void Gui::Run() {
    // Main loop
    while (!glfwWindowShouldClose(m_GLFWWindow)) {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Updating ImGui
        m_ptrImguiManager->NewFrame();

        // Showing the demo window
        ImGui::ShowDemoWindow();

        // Rendering ImGui & Window
        m_ptrImguiManager->Render();
        glfwSwapBuffers(m_GLFWWindow);
    }
}

void Gui::Shutdown() {
    // Cleanup
    m_ptrImguiManager->Shutdown();
    glfwDestroyWindow(m_GLFWWindow);
    glfwTerminate();
}