#include "page_info.h"

#include "../imguimanager.h"

void PageInfo::Draw() {
    ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "informationAbout" ).c_str() );

    ImGui::PushTextWrapPos( ImGui::GetCursorPos().x + 500 );
    ImGui::Text( m_LocalizationManager->GetTranslation("about").c_str(), 500);
    ImGui::PopTextWrapPos();
}
