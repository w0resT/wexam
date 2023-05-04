#include "page_settings.h"

#include "../imguimanager.h"

void PageSettings::Draw() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiTabBarFlags tabFlags = ImGuiTabBarFlags_None;

    auto FramePadding = style.FramePadding;
    auto ItemInnerSpacing = style.ItemInnerSpacing;

    style.FramePadding = ImVec2(20, 8);
    style.ItemInnerSpacing = ImVec2(10, 4);

    if (ImGui::BeginTabBar("##UITab", tabFlags)) {

        if (ImGui::BeginTabItem("Language")) {
            ImGui::NewLine();
            style.FramePadding = FramePadding;
            style.ItemInnerSpacing = ItemInnerSpacing;

            ImGui::SeparatorText("Main");

            // TODO:
            // - Get available languages from m_LocalizationManager not hardcode
            const char* items[] = { "English", "Russia" };
            static int item_current = 0;
            if (ImGui::Combo(m_LocalizationManager->GetTranslation("settingsLanguage").c_str(), &item_current, items, IM_ARRAYSIZE(items))) {
                switch (item_current)
                {
                case 0:
                    m_LocalizationManager->SetLanguage("en");
                    break;
                case 1:
                    m_LocalizationManager->SetLanguage("ru");
                    break;
                default:
                    break;
                }
            }

            ImGui::EndTabItem();
        }

        style.FramePadding = ImVec2(20, 8);
        style.ItemInnerSpacing = ImVec2(10, 4);

        if (ImGui::BeginTabItem("Sizes")) {
            ImGui::NewLine();
            style.FramePadding = FramePadding;
            style.ItemInnerSpacing = ItemInnerSpacing;

            ImGui::SeparatorText("Main");
            //if (ImGui::ShowStyleSelector("Themes##Selector")) {
            //    // Load our settings
            //    m_Gui->ReloadStyle();
            //}
            //ImGui::Separator();

            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");

            ImGui::SeparatorText("Borders");
            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");

            ImGui::SeparatorText("Rounding");
            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");

            ImGui::SeparatorText("Widgets");
            ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                style.WindowMenuButtonPosition = window_menu_button_position - 1;
            ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
            ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
            //ImGui::SameLine(); HelpMarker("Alignment applies when a button is larger than its text content.");
            ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
            //ImGui::SameLine(); HelpMarker("Alignment applies when a selectable is larger than its text content.");
            ImGui::SliderFloat("SeparatorTextBorderSize", &style.SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat2("SeparatorTextAlign", (float*)&style.SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat2("SeparatorTextPadding", (float*)&style.SeparatorTextPadding, 0.0f, 40.0f, "%0.f");
            ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

            //ImGui::SeparatorText("Bottom");
            //ImGui::ColorEdit4("Color 1", &colf1.x);
            //ImGui::ColorEdit4("Color 2", &colf2.x);
            ImGui::EndTabItem();
        }

        style.FramePadding = ImVec2(20, 8);
        style.ItemInnerSpacing = ImVec2(10, 4);

        if (ImGui::BeginTabItem("Colors")) {
            ImGui::NewLine();
            style.FramePadding = FramePadding;
            style.ItemInnerSpacing = ItemInnerSpacing;

            static ImGuiTextFilter filter;
            filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

            ImGui::SameLine(540);

            static ImGuiColorEditFlags alpha_flags = 0;
            if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
            if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
            if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; };

            ImGui::Separator();

            ImGui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NavFlattened);
            ImGui::PushItemWidth(-160);
            for (int i = 0; i < ImGuiCol_COUNT; i++) {
                const char* name = ImGui::GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;

                ImGui::PushID(i);
                ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                ImGui::TextUnformatted(name);
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        style.FramePadding = FramePadding;
        style.ItemInnerSpacing = ItemInnerSpacing;
        ImGui::EndTabBar();
    }
}
