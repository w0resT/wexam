#include <iostream>

#include "../defines.h"
#include "gui.h"
#include "imguimanager.h"

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

    // Initializing ImGui manager
    m_ptrImguiManager = std::make_unique<ImGuiManager>(m_GLFWWindow);
    m_ptrImguiManager->Init();

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
}

void Gui::Run() {
    // Main loop
    while (!m_bShouldClose) {
        m_bShouldClose = glfwWindowShouldClose(m_GLFWWindow);

        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Updating ImGui
        m_ptrImguiManager->NewFrame();

        // Showing the demo window
        //ImGui::ShowDemoWindow();
        if (ImGui::Begin("wxm_begin", nullptr)) {
            ImGui::Text("Test");

            if (ImGui::Button("Click for event")) {
                NotifyObservers();
                m_bShouldClose = true;
            }
        }
        ImGui::End();

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

