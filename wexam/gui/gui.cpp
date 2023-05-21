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

#include "pages/pages.h"

#include "../tsystem/question_with_free_answer.h"
#include "../tsystem/question_with_options.h"
#include "../tsystem/testrepository.h"
#include "../tsystem/testmanager.h"
#include "../tsystem/testresult.h"
#include "../tsystem/test.h"

#include "../users/studentrepository.h"
#include "../users/usermanager.h"

#include "../security/aes_text_cipher.h"
#include "../security/totp_generator.h"
#include "../security/base32_encoder.h"
#include "../security/byte_order_converter.h"

#include "../localization/localization_manager.h"

#include "../FileBrowser/ImGuiFileBrowser.h"

void Gui::Init() {
    // Error callback func
    glfwSetErrorCallback( [] ( int error, const char* description ) {
        std::cout << "[error] GLFW (" << error << "): " << description << std::endl;
                          } );

    // Initializing GLFW
    // TODO: Make assert
    if ( !glfwInit() ) {
        std::cout << "[error] Failed to initialize GLFW" << std::endl;
        std::exit( EXIT_FAILURE );
    }

    // GL 3.0 + not resizable 
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    // Creating window
    m_GLFWWindow = glfwCreateWindow( defines::WINDOW_W, defines::WINDOW_H, defines::WINDOW_TITLE, nullptr, nullptr );

    // TODO: Make assert 
    if ( !m_GLFWWindow ) {
        std::cout << "[error] Failed to create window" << std::endl;
        glfwTerminate();
        std::exit( EXIT_FAILURE );
    }

    glfwMakeContextCurrent( m_GLFWWindow );

    // Enable vsync
    glfwSwapInterval( 1 );

    // Key Callback function
    glfwSetKeyCallback( m_GLFWWindow, [] ( GLFWwindow* window, int key, int scancode, int action, int mods ) {
        // Key 'Delete' for exit
        if ( key == GLFW_KEY_DELETE ) {
            glfwSetWindowShouldClose( window, GLFW_TRUE );
        }
                        } );

    // Set window resize event handler
    glfwSetWindowSizeCallback( m_GLFWWindow, [] ( GLFWwindow* window, int width, int height ) {
        glViewport( 0, 0, width, height );
                               } );

    // Initializing ImGui manager
    m_ImguiManager = std::make_unique<ImGuiManager>( m_GLFWWindow );
    m_ImguiManager->Init();

    // TODO: 
    // - scan folder for .lang files (files lang's to store all .lang files)
    // - make controller for it and move it outside form GUI
    // Initializing localization manager
    std::unique_ptr<ILocalizationReader> reader = std::make_unique<LocalizationReader>();
    std::unique_ptr<ILocalizationWriter> writer = std::make_unique<LocalizationWriter>();
    m_LocalizationManager = std::make_shared<LocalizationManager>( std::move( reader ), std::move( writer ), ( defines::RESOURCES_PATH + "localizations.lang" ) );

    // Sets default language
    m_LocalizationManager->SetLanguage( "en" );

    // Get translations
    m_LocalizationManager->LoadTranslation();

    // Initializing tests repository
    m_TestRepository = std::make_shared<TestRepository>();

    // Initializing AES and stuff
    static const std::string aes_key = "0123456789abcdef";
    std::unique_ptr<ITextCipher> aes_cipher = std::make_unique<AesTextCipher>( aes_key );

    // Initializing test manager
    m_TestManager = std::make_shared<TestManager>( std::move( aes_cipher ) );

    // Initializing tab manager
    m_PageManager = std::make_unique<PageManager>();

    // Initializing user repository
    m_UserRepository = std::make_shared<StudentRepository>();

    // Initializing user manager
    m_UserManager = std::make_shared<UserManager>();

    // Initilizing TOTP and stuff
    std::unique_ptr<Base32Encoder> encoder = std::make_unique<Base32Encoder>();
    m_TOTP = std::make_unique<TOTPGenerator>();
    m_key = "helloworld";

    // Load style
    LoadStyle();

    // Create and init fonts
    CreateFonts();

    // Create and add tabs to the tab manager
    CreateTabs();
}

void Gui::Run() {
    // Main loop
    while ( !m_bShouldClose ) {
        m_bShouldClose = glfwWindowShouldClose( m_GLFWWindow );

        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Updating ImGui
        m_ImguiManager->NewFrame();

        // Unauthorized
        if ( m_bSwitchAccout ) {
            DrawAuthPage();
        }
        else {
            // Showing main window
            Draw();
        }

        // Rendering ImGui & Window
        m_ImguiManager->Render();
        glfwSwapBuffers( m_GLFWWindow );
    }

    // Notify observers that the app is shutting down
    NotifyObservers();
}

void Gui::Shutdown() {
    // Cleanup
    m_ImguiManager->Shutdown();
    glfwDestroyWindow( m_GLFWWindow );
    glfwTerminate();
}

// TODO:
//  - Add nullptr check everywhere when using constructor with arg
// 	- Localization: add file for storage names from lang (so we don't need to edit each str after changes)
void Gui::Draw() {
    static const ImVec2 startPos( 0.f, 0.f );

    ImGui::SetNextWindowPos( startPos );
    ImGui::SetNextWindowSize( ImVec2( defines::WINDOW_W, defines::WINDOW_H ) );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, m_bIsUser ? ImVec2( 5, 5 ) : ImVec2(1, 5) );
    if ( ImGui::Begin( "##begin_main", nullptr, 
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | 
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize ) ) {
        if ( m_bIsAdmin ) {
            // Left side
            DrawLeftChild();

            ImGui::SameLine();

            // Right side
            DrawRightChild();
        }
        else if ( m_bIsUser ) {
            DrawStudentPage();
        }

        // Bottom; should be last
        DrawBottomBar();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Gui::CreateTabs() {
    std::unique_ptr<IPageView> tabTests = std::make_unique<PageTests>( m_LocalizationManager, m_TestRepository, m_TestManager );
    std::unique_ptr<IPageView> tabUsers = std::make_unique<PageUsers>( m_LocalizationManager, m_UserRepository, m_UserManager );
    std::unique_ptr<IPageView> tabSettings = std::make_unique<PageSettings>( m_LocalizationManager );
    std::unique_ptr<IPageView> tabInfo = std::make_unique<PageInfo>( m_LocalizationManager );

    m_PageManager->AddTab( std::move( tabTests ), GUIPages::Tests );
    m_PageManager->AddTab( std::move( tabUsers ), GUIPages::Users );
    m_PageManager->AddTab( std::move( tabSettings ), GUIPages::Settings );
    m_PageManager->AddTab( std::move( tabInfo ), GUIPages::Info );
}

void Gui::CreateFonts() {
    ImGuiIO& io = ImGui::GetIO();
    const auto& glyphRanges = io.Fonts->GetGlyphRangesCyrillic();

    m_Segoeui18 = io.Fonts->AddFontFromFileTTF( ( defines::RESOURCES_PATH + "segoeui.ttf" ).c_str(), 18.f, NULL, glyphRanges );
    IM_ASSERT( m_Segoeui18 != nullptr );

    m_SegoeuiBold18 = io.Fonts->AddFontFromFileTTF( ( defines::RESOURCES_PATH + "segoeuib.ttf" ).c_str(), 18.f, NULL, glyphRanges );
    IM_ASSERT( m_SegoeuiBold18 != nullptr );

    m_SegoeuiBold32 = io.Fonts->AddFontFromFileTTF( ( defines::RESOURCES_PATH + "segoeuib.ttf" ).c_str(), 32.f, NULL, glyphRanges );
    IM_ASSERT( m_SegoeuiBold32 != nullptr );
}

void Gui::LoadStyle() {
    auto& style = ImGui::GetStyle();
    auto& color = style.Colors;

    //color[ImGuiCol_WindowBg] = ImVec4(0.075f, 0.086f, 0.106f, 1.000f);
    color[ ImGuiCol_Tab ] = color[ ImGuiCol_WindowBg ];
    color[ ImGuiCol_TabHovered ] = ImVec4( 0.259f, 0.588f, 0.980f, 0.392f );

    style.FramePadding = ImVec2( 4, 2 );
    style.FrameRounding = 3;
    style.PopupRounding = 3;
    style.WindowBorderSize = 0;
    style.WindowPadding = ImVec2( 1, 5 );
    style.WindowRounding = 0;
    style.ScrollbarRounding = 3;
    style.ScrollbarSize = 14;
    style.GrabRounding = 2;
    style.ChildBorderSize = 0;
    style.ItemSpacing = ImVec2( 8, 5 );
}

// TOOD:
// - add translation
void Gui::DrawAuthPage() {
    static std::string userCode;
    static std::unique_ptr<ByteOrderConverter> convertor = std::make_unique<ByteOrderConverter>();
    static int password_attempts = 0;

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 5, 5 ) );

    ImGui::OpenPopup( "Authorization" );

    // Always center this window when appearing
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();
    ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
    ImGui::SetNextWindowSize( ImVec2( 300, 295 ) );
    if ( ImGui::BeginPopupModal( "Authorization", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings ) ) {
        std::string admin_label = password_attempts != 0 ? "Login [" + std::to_string( password_attempts ) + "/3]" : "Login";

        ImGui::SeparatorText( "Admin account" );
        ImGui::TextWrapped( "Enter the secret code to login as admininstator:" );

        ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail().x );
        ImGui::InputText( "##auth_totp_code", &userCode );

        bool need_disable_login = password_attempts >= 3;
        if ( need_disable_login )
            ImGui::BeginDisabled();

        if ( ImGui::Button( admin_label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 30)) ) {
            auto currentTime = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast< std::chrono::seconds >( currentTime.time_since_epoch() ).count() / 30;
            auto totp = m_TOTP->Generate( m_key, convertor->convertToBigEndian( timestamp ) );

            if ( totp == userCode ) {
                m_bSwitchAccout = false;
                m_bIsAdmin = true;
                password_attempts = 0;
            }
            else {
                ImGui::OpenPopup( "Incorrect code" );
                password_attempts++;
            }

            userCode.clear();
        }

        if ( need_disable_login ) 
            ImGui::EndDisabled();

        ImGui::SeparatorText( "Regular account" );
        ImGui::TextWrapped( "Login with a regular account" );

        if ( ImGui::Button( "Student", ImVec2( ImGui::GetContentRegionAvail().x, 30 ) ) ) {
            m_bSwitchAccout = false;
            m_bIsUser = true;
            userCode.clear();
        }

        ImGui::SeparatorText("Exiting the program");

        if ( ImGui::Button( "Exit", ImVec2( ImGui::GetContentRegionAvail().x, 30 ) ) ) 
            m_bShouldClose = true;

        ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
        if ( ImGui::BeginPopupModal( "Incorrect code", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings ) ) {
            ImGui::Text( "Incorrect code" );
            if ( ImGui::Button( "Ok", ImVec2( -1, 30 ) ) )
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar();
}

void Gui::DrawBottomBar() {
    using namespace std::literals;

    const auto gradient_size = ImVec2( 1000, 20 );
    const auto now = std::chrono::system_clock::now();
    const auto t_c = std::chrono::system_clock::to_time_t( now - 24h );

    std::stringstream ssTime;
    std::stringstream ssGroup;

    ssTime << m_LocalizationManager->GetTranslation( "bottomBarData" ) << ": " << std::put_time( std::localtime( &t_c ), "%F %T\n" );
    ssGroup << m_LocalizationManager->GetTranslation( "bottomBarLoggedInAs" ) << ": " << m_LocalizationManager->GetTranslation( m_bIsAdmin ? "admin" : "user" );

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetWindowPos();

    const ImVec2 p1 = ImVec2( p.x + gradient_size.x, p.y + gradient_size.y + 565 );
    const ImVec4 col1( 0.000f, 0.412f, 1.000f, 0.392f );
    const ImVec4 col2( 0.000f, 0.631f, 0.820f, 0.392f );

    const ImVec2 p_min = ImVec2( p.x, p.y + 560 );
    const ImVec2 p_max = p1;

    const float posText_y = p.y + gradient_size.y + 542.f;
    const ImVec2 posTextTime = ImVec2( p.x + gradient_size.x - 5 - ImGui::CalcTextSize( ssTime.str().c_str() ).x, posText_y );
    const ImVec2 posTextGroup = ImVec2( p.x + 5, posText_y );

    draw_list->AddRectFilledMultiColor( p_min, p_max, ImColor( col1 ), ImColor( col2 ), ImColor( col2 ), ImColor( col1 ) );
    draw_list->AddText( posTextTime, ImGui::GetColorU32( ImGuiCol_Text ), ssTime.str().c_str() );
    draw_list->AddText( ImVec2( p.x + 5, p.y + gradient_size.y + 542 ), ImGui::GetColorU32( ImGuiCol_Text ), ssGroup.str().c_str() );
}

void Gui::DrawLeftChild() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetWindowPos();

    const ImGuiWindowFlags childFlags = 0;

    const ImVec2 leftTopChildSize( ImGui::GetContentRegionAvail().x * 0.2f, 70 );

    ImGui::BeginChild( "##child_left_top", leftTopChildSize ); {
        static int circle_size = 50;
        static int circle_offset = 10;

        ImGui::SetCursorPos( ImVec2( 9, 5 ) );
        draw_list->AddCircleFilled( ImVec2( p.x + circle_offset + circle_size * 0.5f, p.y + circle_offset + circle_size * 0.5f ), 46 * 0.5f, ImColor( 100, 150, 255 ), 0 );

        ImGui::PushFont( m_SegoeuiBold18 );
        ImGui::SetCursorPos( ImVec2( 65, 14 ) );
        ImGui::Text( m_LocalizationManager->GetTranslation( "admin" ).c_str() ); // m_Auth->GetAccountName();
        ImGui::PopFont();

        ImGui::SetCursorPos( ImVec2( 65, 29 ) );
        ImGui::Text( m_LocalizationManager->GetTranslation( "localAccount" ).c_str() );

        ImGui::SetCursorPos( ImVec2( 0, 65 ) );
        ImGui::Separator();
    }
    ImGui::EndChild();

    const ImVec2 leftChildSize( ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y * 0.92f );

    ImGui::PushStyleVar( ImGuiStyleVar_SelectableTextAlign, ImVec2( 0.05f, 0.5f ) );
    ImGui::BeginChild( "##child_left", leftChildSize, false, childFlags ); {
        const ImVec2 selectableSize = ImVec2( ImGui::GetContentRegionAvail().x, 48 );
        const ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_None;

        static auto cur_pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos( ImVec2( cur_pos.x + 15, cur_pos.y + 10 ) );

        // Draw pages
        ImGui::BeginGroup();
        for ( int i = 0; i < static_cast< int >( GUIPages::MAX ); ++i ) {
            const bool isSelected = m_PageManager->GetCurrentPage() == static_cast< GUIPages >( i );
            const auto tabName = m_PageManager->GetPageName( static_cast< GUIPages >( i ) );
            if ( ImGui::SelectableTab( m_LocalizationManager->GetTranslation( tabName ).c_str(), isSelected, selectableFlags, selectableSize ) ) {
                m_PageManager->SetCurrentPage( static_cast< GUIPages >( i ) );
            }
        }
        ImGui::EndGroup();

        ImGui::SetCursorPos( ImVec2( cur_pos.x + 15, cur_pos.y + 385 ) );
        if ( ImGui::Button( m_LocalizationManager->GetTranslation( "logout" ).c_str(), ImVec2( 182, 40 ) ) ) {
            m_bSwitchAccout = true;
            m_bIsAdmin = false;
        }

        ImGui::SetCursorPos( ImVec2( cur_pos.x + 15, cur_pos.y + 430 ) );
        if ( ImGui::Button( m_LocalizationManager->GetTranslation( "exit" ).c_str(), ImVec2( 182, 40 ) ) ) {
            m_bShouldClose = true;
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void Gui::DrawRightChild() {
    const auto& curTabName = m_LocalizationManager->GetTranslation( m_PageManager->GetCurrentPageName() );
    const ImGuiWindowFlags childFlags = 0;

    ImGui::SetCursorPos( ImVec2( 230, 10 ) );
    ImGui::BeginChild( "##child_right", ImVec2( ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.928f ), false, childFlags ); {
        ImGui::PushFont( m_SegoeuiBold32 );
        ImGui::Text( curTabName.c_str() );
        ImGui::PopFont();

        m_PageManager->Draw( m_PageManager->GetCurrentPage() );
    }
    ImGui::EndChild();
}

void Gui::DrawStudentPage() {
    static bool show_main_page = true;
    static bool show_test_page = false;
    static bool show_result_page = false;

    static bool finish_clear_exit = false;
    static bool finish_clear_test = false;

    static int current_test_id = -1;
    static int current_user_id = -1;

    static bool useInput = false;
    static std::string bufName;
    
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
        | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
        | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX;

    //static ImVec2 outer_size_value = ImVec2( 0.0f, ImGui::GetContentRegionAvail().y * 0.71f );
    static ImVec2 button_size = ImVec2( 247, 25 );

    static std::vector<std::string> answerBuffer;

    if ( show_main_page ) {

        if ( ImGui::BeginTable( "##table_student_main", 2, ImGuiTableFlags_NoSavedSettings ) ) {
            // First columns - test list
            ImGui::TableNextColumn();

            ImGui::PushFont( m_SegoeuiBold32 );
            ImGui::Text( m_LocalizationManager->GetTranslation( "availableTests" ).c_str() );
            ImGui::PopFont();
            ImGui::NewLine();
            
            static ImGuiTextFilter filter_tests;
            ImGui::PushID( "test_search" );
            filter_tests.Draw( m_LocalizationManager->GetTranslation( "search" ).c_str(), ImGui::GetFontSize() * 18 );
            ImGui::PopID();

            if ( ImGui::BeginTable( "##table_student_main_tests", 2, flags, ImVec2( 0.0f, ImGui::GetContentRegionAvail().y * 0.525f ) ) ) {
                ImGui::TableSetupScrollFreeze( 0, 1 ); // Make top row always visible
                ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "title" ).c_str() );
                ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "description" ).c_str() );
                ImGui::TableHeadersRow();

                // Retrieve and print the test details
                for ( auto& testPtr : m_TestRepository->GetTests() ) {
                    if ( !testPtr ) {
                        continue;
                    }

                    if ( !filter_tests.PassFilter( testPtr->GetTitle().c_str() ) ) {
                        continue;
                    }

                    auto test_id = testPtr->GetId();

                    ImGui::TableNextRow();
                    ImGui::PushID( test_id );

                    for ( int column = 0; column < 2; column++ ) {
                        const bool item_is_selected = ( current_test_id == test_id );

                        if ( !ImGui::TableSetColumnIndex( column ) && column > 0 )
                            continue;

                        if ( column == 0 ) {
                            if ( ImGui::Selectable( testPtr->GetTitle().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
                                // Double click - unselect
                                if ( current_test_id == test_id ) {
                                    current_test_id = -1;
                                }
                                else {
                                    current_test_id = test_id;
                                }
                            }
                        }
                        else if ( column == 1 ) {
                            if ( ImGui::Selectable( testPtr->GetDescription().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
                                // Double click - unselect
                                if ( current_test_id == test_id ) {
                                    current_test_id = -1;
                                }
                                else {
                                    current_test_id = test_id;
                                }
                            }
                        }

                        if ( item_is_selected )
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }

            // Second columns - test settings
            ImGui::TableNextColumn();
            ImGui::PushFont( m_SegoeuiBold32 );
            ImGui::Text( m_LocalizationManager->GetTranslation( "testManagement" ).c_str() );
            ImGui::PopFont();
            ImGui::NewLine();

            bool need_disable_button = current_test_id <= 0 
                || ( useInput ? bufName.empty() : current_user_id <= 0 ) 
                || !m_TestRepository->FindTestById(current_test_id)->GetQuestionCount();

            if ( need_disable_button )
                ImGui::BeginDisabled();

            if ( ImGui::Button( m_LocalizationManager->GetTranslation( "startTest" ).c_str(), ImVec2( ImGui::GetFontSize() * 18, 25) ) ) {
                show_test_page = true;
                show_main_page = false;
            }

            if ( need_disable_button )
                ImGui::EndDisabled();

            //ImGui::Checkbox( m_LocalizationManager->GetTranslation( "inputName" ).c_str(), &useInput );

            if ( useInput ) {
                // bufName
                if ( ImGui::InputTextWithHint( m_LocalizationManager->GetTranslation( "name" ).c_str(),
                                               m_LocalizationManager->GetTranslation( "studentsName" ).c_str(), &bufName ) ) {
                }
            }
            else {
                static ImGuiTextFilter filter;

                ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "availableUsers" ).c_str() );
                filter.Draw( m_LocalizationManager->GetTranslation( "search" ).c_str(), ImGui::GetFontSize() * 18 );

                if ( ImGui::BeginTable( "##table_student_main_users", 2, flags, ImVec2( 0.0f, ImGui::GetContentRegionAvail().y * 0.525f ) ) ) {
                    ImGui::TableSetupScrollFreeze( 0, 1 ); // Make top row always visible
                    ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "name" ).c_str() );
                    ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "group" ).c_str() );
                    ImGui::TableHeadersRow();

                    // Retrieve and print the test details
                    for ( auto& userPtr : m_UserRepository->GetStudents() ) {
                        if ( !userPtr ) {
                            continue;
                        }

                        if ( !filter.PassFilter( userPtr->GetName().c_str() ) ) {
                            continue;
                        }

                        auto test_id = userPtr->GetId();

                        ImGui::TableNextRow();
                        ImGui::PushID( test_id );

                        for ( int column = 0; column < 2; column++ ) {
                            const bool item_is_selected = ( current_user_id == test_id );

                            if ( !ImGui::TableSetColumnIndex( column ) && column > 0 )
                                continue;

                            if ( column == 0 ) {
                                if ( ImGui::Selectable( userPtr->GetName().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
                                    // Double click - unselect
                                    if ( current_user_id == test_id ) {
                                        current_user_id = -1;
                                    }
                                    else {
                                        current_user_id = test_id;
                                    }
                                }
                            }
                            else if ( column == 1 ) {
                                if ( ImGui::Selectable( userPtr->GetGroup().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
                                    // Double click - unselect
                                    if ( current_user_id == test_id ) {
                                        current_user_id = -1;
                                    }
                                    else {
                                        current_user_id = test_id;
                                    }
                                }
                            }

                            if ( item_is_selected )
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::PopID();
                    }

                    ImGui::EndTable();
                }
            }

            ImGui::EndTable();
        }

        ImGui::Separator();

        static bool needDecrypt = false;
        static bool removeCurReposTest = true;
        static bool removeCurReposUser = true;
        static imgui_addons::ImGuiFileBrowser file_dialog;

        if ( ImGui::BeginTable( "##table_student_main_settings", 2, ImGuiTableFlags_NoSavedSettings ) ) {
            ImGui::TableNextColumn();
            ImGui::SeparatorText( "Tests import" );
            ImGui::Checkbox( m_LocalizationManager->GetTranslation( "useEncryption" ).c_str(), &needDecrypt );
            ImGui::Checkbox( m_LocalizationManager->GetTranslation( "removeCurTest" ).c_str(), &removeCurReposTest );

            if ( ImGui::Button( m_LocalizationManager->GetTranslation( "importTest" ).c_str(), ImVec2( 180, 30 ) ) ) {
                ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "importTest" ).c_str() );
            }

            ImGui::TableNextColumn();
            ImGui::SeparatorText( "Students import" );
            ImGui::Checkbox( m_LocalizationManager->GetTranslation( "removeCurUsers" ).c_str(), &removeCurReposUser );
            if ( ImGui::Button( m_LocalizationManager->GetTranslation( "importStudents" ).c_str(), ImVec2( 180, 30 ) ) ) {
                ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "importStudents" ).c_str() );
            }  

            if ( file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "importTest" ).c_str(),
                                             imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                             ImVec2( 700, 310 ), ".wxm" ) ) {
                if ( removeCurReposTest ) {
                    m_TestRepository->Clear();
                }

                m_TestManager->ImportTests( file_dialog.selected_path, m_TestRepository, needDecrypt, true, false );
            }

            if ( file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "importStudents" ).c_str(),
                                             imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                             ImVec2( 700, 310 ), ".wxm" ) ) {
                if ( removeCurReposUser ) {
                    m_UserRepository->Clear();
                }

                m_UserManager->ImportUsers( file_dialog.selected_path, m_UserRepository );
            }

            ImGui::EndTable();
        }

        ImGui::Separator();

        const char* items[] = { "English", "Russia" };
        static int item_current = 0;
        ImGui::SetNextItemWidth( 250 );
        if ( ImGui::Combo( m_LocalizationManager->GetTranslation( "settingsLanguage" ).c_str(), &item_current, items, IM_ARRAYSIZE( items ) ) ) {
            switch ( item_current ) {
            case 0:
                m_LocalizationManager->SetLanguage( "en" );
                break;
            case 1:
                m_LocalizationManager->SetLanguage( "ru" );
                break;
            default:
                break;
            }
        }

        ImGui::SameLine( 626 );

        if ( ImGui::Button( m_LocalizationManager->GetTranslation( "logout" ).c_str(), ImVec2( 180, 30 ) ) ) {
            m_bSwitchAccout = true;
            m_bIsUser = false;
        }

        ImGui::SameLine();

        if ( ImGui::Button( m_LocalizationManager->GetTranslation( "exit" ).c_str(), ImVec2( 180, 30 ) ) ) {
            m_bShouldClose = true;
        }

    }
    else if ( show_test_page ) {
        auto cur_test = m_TestRepository->FindTestById( current_test_id );
        assert( cur_test && "Bad current test id");

        auto cur_student = m_UserRepository->FindStudentById( current_user_id );
        assert( cur_student && "Bad current student id" );

        std::shared_ptr<ITest> completed_test = std::make_shared<Test>( cur_test );

        if ( ImGui::BeginTable( "##table_test_page_head", 2, ImGuiTableFlags_NoSavedSettings ) ) {
            ImGui::TableNextColumn();

            std::string strCurTest = "Test: " + cur_test->GetTitle();
            std::string strDescription = "Description: " + cur_test->GetDescription();
            std::string strQuestCount = "Quiestions: " + std::to_string( cur_test->GetQuestionCount() );

            ImGui::PushFont( m_SegoeuiBold32 );
            ImGui::TextWrapped( strCurTest.c_str() );
            ImGui::PopFont();

            ImGui::PushFont( m_SegoeuiBold18 );
            ImGui::TextWrapped( strDescription.c_str() );
            ImGui::TextWrapped( strQuestCount.c_str() );
            ImGui::PopFont();

            ImGui::TableNextColumn();

            if ( useInput ) {
                ImGui::Text( bufName.c_str() );
            }
            else {
                ImGui::Text( cur_student->GetName().c_str() );
                ImGui::Text( cur_student->GetGroup().c_str() );
            }


            ImGui::EndTable();
        }

        ImGui::Separator();
        ImGui::NewLine();
        ImGui::Separator();

        if ( ImGui::BeginTable( "##table_test_page_main", 1, ImGuiTableFlags_NoSavedSettings, ImVec2(0, 400) ) ) {
            ImGui::TableNextColumn();

            answerBuffer.resize( cur_test->GetQuestionCount() );

            for ( int i = 0; i < cur_test->GetQuestionCount(); ++i ) {
                auto cur_question = cur_test->GetQuestion( i );

                std::string strCurQuest = std::to_string( i + 1 ) + ". " + cur_question->GetQuestion();

                ImGui::PushFont( m_SegoeuiBold18 );
                ImGui::TextWrapped( strCurQuest.c_str() );
                ImGui::PopFont();
                
                ImGui::PushID( cur_question->GetId() );
                if ( cur_question->GetQuestionType() == QuestionType::FreeAnswer ) {
                    ImGui::InputTextWithHint( "Answer", "Answer", &answerBuffer[ i ] );
                }
                else if ( cur_question->GetQuestionType() == QuestionType::AnswerOptions ) {
                    for ( int j = 0; j < cur_question->GetAnswerOptions().size(); ++j ) {
                        const auto& opt = cur_question->GetAnswerOption( j );
                        const bool item_is_selected = ( answerBuffer[i] == opt );

                        std::string strOpt = "     " + std::to_string(j + 1) + ") " + opt;
                        if ( ImGui::Selectable( strOpt.c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap) ) {
                            // Double click - unselect
                            if ( answerBuffer[i] == opt ) {
                                answerBuffer[ i ].clear();
                            }
                            else {
                                answerBuffer[ i ] = opt;
                            }
                        }

                        if ( item_is_selected )
                            ImGui::SetItemDefaultFocus();     
                    }
                }
                ImGui::PopID();

                ImGui::Separator();
            }

            ImGui::EndTable();
        }

        if ( ImGui::Button( "Finish", button_size ) ) {
            ImGui::OpenPopup( "Test Completion");
        }

        ImGui::SameLine();

        if ( ImGui::Button( "Back", button_size ) ) {
            ImGui::OpenPopup( "Return to main page" );
        }

        if ( finish_clear_test ) {
            // Sets user answers
            for ( int i = 0; i < completed_test->GetQuestionCount(); ++i ) {
                completed_test->GetQuestion( i )->SetUserAnswer( answerBuffer[ i ] );
            }

            std::string strName = useInput ? bufName : cur_student->GetName();
            m_PassedTest[ strName ] = completed_test;

            bufName.clear();
            answerBuffer.clear();

            show_test_page = false;
            show_main_page = false;
            show_result_page = true;

            finish_clear_test = false;
        }

        if ( finish_clear_exit ) {
            bufName.clear();
            answerBuffer.clear();

            show_test_page = false;
            show_result_page = false;
            show_main_page = true;

            finish_clear_exit = false;
        }
    }
    else if ( show_result_page ) {
        auto cur_test = m_TestRepository->FindTestById( current_test_id );
        assert( cur_test && "Bad current test id" );

        auto cur_student = m_UserRepository->FindStudentById( current_user_id );
        assert( cur_student && "Bad current student id" );

        static bool show_result = true;

        for ( const auto& question : cur_test->GetQuestions() ) {
            if ( !question->HasCorrectAnswer() ) {
                show_result = false;
            }
        }

        if ( show_result ) {
            std::shared_ptr<ITestResult> result = std::make_shared<TestResult>( cur_test );
            result->EvaluateTest();

            std::string strCurTest = "Test: " + cur_test->GetTitle() + ", ID: " + std::to_string( cur_test->GetId() );
            std::string strDescription = "Description: " + cur_test->GetDescription();
            std::string strTotalQuest = "Total Questions: " + std::to_string(result->GetTotalQuestions());
            std::string strCorAnswers = "Correct Answers: " + std::to_string( result->GetCorrectAnswers());
            std::string strIncorAnswers = "Incorrect Answers: " + std::to_string( result->GetIncorrectAnswers());
            std::string strUnanswered = "Unanswered Questions: " + std::to_string( result->GetUnansweredQuestions());
            std::string strTotal = "Total score: " + std::to_string( static_cast<int>(result->GetScore()) ) + "%%";

            ImGui::PushFont( m_SegoeuiBold32 );
            ImGui::TextWrapped( cur_student->GetName().c_str());
            ImGui::Separator();
            ImGui::TextWrapped( strCurTest.c_str() );
            ImGui::PopFont();

            ImGui::PushFont( m_SegoeuiBold18 );
            ImGui::TextWrapped( strDescription.c_str() );
            ImGui::PopFont();

            ImGui::Separator();

            ImGui::TextWrapped( strTotalQuest.c_str() );
            ImGui::TextWrapped( strCorAnswers.c_str() );
            ImGui::TextWrapped( strIncorAnswers.c_str() );
            ImGui::TextWrapped( strUnanswered.c_str() );
            ImGui::TextWrapped( strTotal.c_str() );
        }
        else {
            ImGui::Text( "Test finished" );
        }

        ImGui::Separator();

        if ( ImGui::Button( "Return to main page", button_size ) ) {
            current_test_id = -1;
            current_user_id = -1;

            show_test_page = false;
            show_main_page = true;
            show_result_page = false;

            show_result = true;
        }
    }

    // Always center this window when appearing
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();
    ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
    ImGui::SetNextWindowSize( ImVec2( 300, 0 ) );
    if ( ImGui::BeginPopupModal( "Test Completion", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings ) ) {
        ImGui::TextWrapped( "Are you sure you want to complete the test?" );
        if ( ImGui::Button( "Yes", ImVec2( -1, 30 ) ) ) {
            finish_clear_test = true;
            ImGui::CloseCurrentPopup();
        }
        
        if ( ImGui::Button( "Cancel", ImVec2( -1, 30 ) ) ) {
            finish_clear_test = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
    ImGui::SetNextWindowSize( ImVec2( 300, 0 ) );
    if ( ImGui::BeginPopupModal( "Return to main page", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings ) ) {
        ImGui::TextWrapped( "Are you sure you want to return to the main page?" );
        if ( ImGui::Button( "Yes", ImVec2( -1, 30 ) ) ) {
            finish_clear_exit = true;
            ImGui::CloseCurrentPopup();
        }
        
        if ( ImGui::Button( "Cancel", ImVec2( -1, 30 ) ) ) {
            finish_clear_exit = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

