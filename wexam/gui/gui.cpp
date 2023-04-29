#include <iostream>

#include "../defines.h"
#include "gui.h"
#include "imguimanager.h"

#include "wrapper/guiwrapper.h"

#include "pages/pages.h"

#include "../localization/localization_manager.h"

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
    m_PageManager = std::make_unique<PageManager>();

    // Creating and adding tabs to tab manager
    CreateTabs();

    // Initializing gui wrapper
    m_GUIWrapper = std::make_shared<GUIWrapper>();

    // TODO: make controller for it and move it outside form GUI
    // Initializing localization manager
    std::string fileName = ".\\resources\\localizations.lang";
    std::unique_ptr<ILocalizationReader> reader = std::make_unique<LocalizationReader>();
    std::unique_ptr<ILocalizationWriter> writer = std::make_unique<LocalizationWriter>();
    m_LocalizationManager = std::make_unique<LocalizationManager>(std::move(reader), std::move(writer), fileName);

    // Sets default language
    m_LocalizationManager->SetLanguage("en");

    // Get translations
    m_LocalizationManager->LoadTranslation();
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

// TODO: Add virtual destructor for each interfaces

void Gui::Draw() {
    static const ImVec2 startPos(0.f, 0.f);

    ImGui::SetNextWindowPos(startPos);
    ImGui::SetNextWindowSize(m_GUIWrapper->GetWindowSize());
    if (ImGui::Begin("##begin_main", nullptr, m_GUIWrapper->GetWindowFlags())) {
        ImGui::Text(m_LocalizationManager->GetTranslation("justTest").c_str());

        ImGui::PushFont(m_Segoeui18);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();

        ImGui::PushFont(m_SegoeuiBold18);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();

        ImGui::PushFont(m_SegoeuiBold32);
        ImGui::Text("Never gonna give you up");
        ImGui::PopFont();
        
        m_PageManager->Draw();

        m_PageManager->Draw(m_GUIWrapper->GetCurrentDrawPage());
    }
    ImGui::End();
}

void Gui::CreateTabs() {
    /*
    * TODO: TestsModel for tests, UsersModel for users
    */
    std::unique_ptr<IPageView> tabTests = std::make_unique<PageTests>(m_Model);
    std::unique_ptr<IPageView> tabUsers = std::make_unique<PageUsers>(m_Model);
    std::unique_ptr<IPageView> tabSettings = std::make_unique<PageSettings>(m_Model);
    std::unique_ptr<IPageView> tabInfo = std::make_unique<PageInfo>(m_Model);

    m_PageManager->AddTab(std::move(tabTests), GUIPages::Tests);
    m_PageManager->AddTab(std::move(tabUsers), GUIPages::Users);
    m_PageManager->AddTab(std::move(tabSettings), GUIPages::Settings);
    m_PageManager->AddTab(std::move(tabInfo), GUIPages::Info);
}

void Gui::CreateFonts() {
    ImGuiIO& io = ImGui::GetIO();

    m_Segoeui18 = io.Fonts->AddFontFromMemoryCompressedTTF(
        fonts::segoeui_compressed_data, fonts::segoeui_compressed_size, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_Segoeui18 != nullptr);

    m_SegoeuiBold18 = io.Fonts->AddFontFromMemoryCompressedTTF(
        fonts::segoeuib_compressed_data, fonts::segoeuib_compressed_size, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_SegoeuiBold18 != nullptr);

    m_SegoeuiBold32 = io.Fonts->AddFontFromMemoryCompressedTTF(
        fonts::segoeuib_compressed_data, fonts::segoeuib_compressed_size, 32.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(m_SegoeuiBold32 != nullptr);
}

void Gui::LoadStyle() {
    ImGui::StyleColorsDark();

    auto& style = ImGui::GetStyle();
    auto& color = style.Colors;

    color[ImGuiCol_Tab] = color[ImGuiCol_WindowBg];
    color[ImGuiCol_TabHovered] = ImVec4(0.259f, 0.588f, 0.980f, 0.392f);

    style.FramePadding = ImVec2(1, 2);
    style.FrameRounding = 3;
    style.WindowBorderSize = 0;
    style.WindowPadding = ImVec2(1, 5);
    style.WindowRounding = 7;
    style.ScrollbarRounding = 3;
    style.ScrollbarSize = 14;
    style.GrabRounding = 2;
    style.ChildBorderSize = 0;
    style.ItemSpacing = ImVec2(8, 5);
}

