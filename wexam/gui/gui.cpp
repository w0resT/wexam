#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>

// output data + time
#include <chrono>
#include <ctime>
#include <iomanip>

#include "../defines.h"
#include "gui.h"
#include "imguimanager.h"
#include "custom_imgui_features.h"

#include "wrapper/guiwrapper.h"

#include "pages/pages.h"

#include "../localization/localization_manager.h"


void Gui::Init() {
    // Error callback func
    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "[error] GLFW (" << error << "): " << description << std::endl;
        });

	// Initializing GLFW
    // TODO: Make assert
    if (!glfwInit()) {
        std::cout << "[error] Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // GL 3.0 + not resizable 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Creating window
    m_GLFWWindow = glfwCreateWindow(defines::WINDOW_W, defines::WINDOW_H, defines::WINDOW_TITLE, nullptr, nullptr);

    // TODO: Make assert 
    if (!m_GLFWWindow) {
        std::cout << "[error] Failed to create window" << std::endl;
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

    // TODO: 
    // - scan folder for .lang files (files lang's to store all .lang files)
    // - make controller for it and move it outside form GUI
    // Initializing localization manager
    std::unique_ptr<ILocalizationReader> reader = std::make_unique<LocalizationReader>();
    std::unique_ptr<ILocalizationWriter> writer = std::make_unique<LocalizationWriter>();
    m_LocalizationManager = std::make_shared<LocalizationManager>(std::move(reader), std::move(writer), (defines::RESOURCES_PATH + "localizations.lang"));

    // Sets default language
    m_LocalizationManager->SetLanguage("en");

    // Get translations
    m_LocalizationManager->LoadTranslation();

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
}

void Gui::Run() {
    // Main loop
    while (!m_bShouldClose) {
        m_bShouldClose = glfwWindowShouldClose(m_GLFWWindow);

        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Updating ImGui
        m_ImguiManager->NewFrame();

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

// TODO:
//  - Add nullptr check everywhere when using constructor with arg
// 	- Localization: add file for storage names from lang (so we don't need to edit each str after changes)
void Gui::Draw() {
    static const ImVec2 startPos(0.f, 0.f);

    ImGui::SetNextWindowPos(startPos);
    ImGui::SetNextWindowSize(m_GUIWrapper->GetWindowSize());
    if (ImGui::Begin("##begin_main", nullptr, m_GUIWrapper->GetWindowFlags())) {
        // Left side
        DrawLeftChild();

        ImGui::SameLine();

        // Right side
        DrawRightChild();

        // Bottom; should be last
        DrawBottomBar();
    }
    ImGui::End();
}

void Gui::CreateTabs() {
    /*
    * TODO: TestsModel for tests, UsersModel for users
    */

    std::unique_ptr<IPageView> tabTests = std::make_unique<PageTests>(m_LocalizationManager, m_Model);
    std::unique_ptr<IPageView> tabUsers = std::make_unique<PageUsers>(m_LocalizationManager, m_Model);
    std::unique_ptr<IPageView> tabSettings = std::make_unique<PageSettings>(m_LocalizationManager);
    std::unique_ptr<IPageView> tabInfo = std::make_unique<PageInfo>(m_LocalizationManager);

    m_PageManager->AddTab(std::move(tabTests), GUIPages::Tests);
    m_PageManager->AddTab(std::move(tabUsers), GUIPages::Users);
    m_PageManager->AddTab(std::move(tabSettings), GUIPages::Settings);
    m_PageManager->AddTab(std::move(tabInfo), GUIPages::Info);
}

void Gui::CreateFonts() {
    ImGuiIO& io = ImGui::GetIO();
    const auto& glyphRanges = io.Fonts->GetGlyphRangesCyrillic();

    m_Segoeui18 = io.Fonts->AddFontFromFileTTF((defines::RESOURCES_PATH + "segoeui.ttf").c_str(), 18.f, NULL, glyphRanges);
    IM_ASSERT(m_Segoeui18 != nullptr);

    m_SegoeuiBold18 = io.Fonts->AddFontFromFileTTF((defines::RESOURCES_PATH + "segoeuib.ttf").c_str(), 18.f, NULL, glyphRanges);
    IM_ASSERT(m_SegoeuiBold18 != nullptr);

    m_SegoeuiBold32 = io.Fonts->AddFontFromFileTTF((defines::RESOURCES_PATH + "segoeuib.ttf").c_str(), 32.f, NULL, glyphRanges);
    IM_ASSERT(m_SegoeuiBold32 != nullptr);
}

void Gui::LoadStyle() {
    auto& style = ImGui::GetStyle();
    auto& color = style.Colors;

    //color[ImGuiCol_WindowBg] = ImVec4(0.075f, 0.086f, 0.106f, 1.000f);
    color[ImGuiCol_Tab] = color[ImGuiCol_WindowBg];
    color[ImGuiCol_TabHovered] = ImVec4(0.259f, 0.588f, 0.980f, 0.392f);

    style.FramePadding = ImVec2(4, 2);
    style.FrameRounding = 3;
    style.WindowBorderSize = 0;
    style.WindowPadding = ImVec2(1, 5);
    style.WindowRounding = 0;
    style.ScrollbarRounding = 3;
    style.ScrollbarSize = 14;
    style.GrabRounding = 2;
    style.ChildBorderSize = 0;
    style.ItemSpacing = ImVec2(8, 5);
}

void Gui::DrawBottomBar() {
    using namespace std::literals;

    const auto gradient_size = ImVec2(1000, 20);
    const auto now = std::chrono::system_clock::now();
    const auto t_c = std::chrono::system_clock::to_time_t(now - 24h);

    std::stringstream ssTime;
    std::stringstream ssGroup;

    ssTime << m_LocalizationManager->GetTranslation("bottomBarData") << ": " << std::put_time(std::localtime(&t_c), "%F %T\n");
    ssGroup << m_LocalizationManager->GetTranslation("bottomBarLoggedInAs") << ": " << m_LocalizationManager->GetTranslation("admin");

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetWindowPos();  

    const ImVec2 p1 = ImVec2(p.x + gradient_size.x, p.y + gradient_size.y + 565);
    const ImVec4 col1(0.000f, 0.412f, 1.000f, 0.392f);
    const ImVec4 col2(0.000f, 0.631f, 0.820f, 0.392f);

    const ImVec2 p_min = ImVec2(p.x, p.y + 560);
    const ImVec2 p_max = p1;

    const float posText_y = p.y + gradient_size.y + 542.f;
    const ImVec2 posTextTime = ImVec2(p.x + gradient_size.x - 5 - ImGui::CalcTextSize(ssTime.str().c_str()).x, posText_y);
    const ImVec2 posTextGroup = ImVec2(p.x + 5, posText_y);

    draw_list->AddRectFilledMultiColor(p_min, p_max, ImColor(col1), ImColor(col2), ImColor(col2), ImColor(col1));
    draw_list->AddText(posTextTime, ImGui::GetColorU32(ImGuiCol_Text), ssTime.str().c_str());
    draw_list->AddText(ImVec2(p.x + 5, p.y + gradient_size.y + 542), ImGui::GetColorU32(ImGuiCol_Text), ssGroup.str().c_str());
}

void Gui::DrawLeftChild() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetWindowPos();

    const ImGuiWindowFlags childFlags = 0;

    const ImVec2 leftTopChildSize(ImGui::GetContentRegionAvail().x * 0.2f, 70);

    ImGui::BeginChild("##child_left_top", leftTopChildSize); {
        static int circle_size = 50;
        static int circle_offset = 10;

        ImGui::SetCursorPos(ImVec2(9, 5));
        draw_list->AddCircleFilled(ImVec2(p.x + circle_offset + circle_size * 0.5f, p.y + circle_offset + circle_size * 0.5f), 46 * 0.5f, ImColor(100, 150, 255), 0);

        ImGui::PushFont(m_SegoeuiBold18);
        ImGui::SetCursorPos(ImVec2(65, 14));
        ImGui::Text("Account name"); // m_Auth->GetAccountName();
        ImGui::PopFont();

        ImGui::SetCursorPos(ImVec2(65, 29));
        ImGui::Text(m_LocalizationManager->GetTranslation("localAccount").c_str());

        ImGui::SetCursorPos(ImVec2(0, 65));
        ImGui::Separator();
    }
    ImGui::EndChild();

    const ImVec2 leftChildSize(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y * 0.92f);

    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.05f, 0.5f));
    ImGui::BeginChild("##child_left", leftChildSize, false, childFlags); {

        const ImVec2 selectableSize = ImVec2(ImGui::GetContentRegionAvail().x, 48);
        const ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_None;

        static auto cur_pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cur_pos.x + 15, cur_pos.y + 10));

        // Draw pages
        ImGui::BeginGroup();
        for (int i = 0; i < static_cast<int>(GUIPages::MAX); ++i) {
            const bool isSelected = m_PageManager->GetCurrentPage() == static_cast<GUIPages>(i);
            const auto tabName = m_PageManager->GetPageName(static_cast<GUIPages>(i));
            if (ImGui::SelectableTab(m_LocalizationManager->GetTranslation(tabName).c_str(), isSelected, selectableFlags, selectableSize)) {
                m_PageManager->SetCurrentPage(static_cast<GUIPages>(i));
            }
        }
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(1);
}

void Gui::DrawRightChild() {
    const auto& curTabName = m_LocalizationManager->GetTranslation(m_PageManager->GetCurrentPageName());
    const ImGuiWindowFlags childFlags = 0;

    ImGui::SetCursorPos(ImVec2(230, 10));
    ImGui::BeginChild("##child_right", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.928f), false, childFlags); {

        ImGui::PushFont(m_SegoeuiBold32);
        ImGui::Text(curTabName.c_str());
        ImGui::PopFont();

        //ImGui::NewLine();

        m_PageManager->Draw(m_PageManager->GetCurrentPage());
    }
    ImGui::EndChild();
}

