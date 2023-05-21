#include "page_users.h"
#include "../../users/student.h"

#include "../imguimanager.h"

#include "../../FileBrowser/ImGuiFileBrowser.h"

void PageUsers::Draw() {
	static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
		| ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX;

	static ImVec2 outer_size_value = ImVec2( 0.0f, 0.0f );
	static ImVec2 button_size = ImVec2( 247, 25 );

	static int current_student_id = -1;
	static int prev_student_id = -1;

	static std::string bufName;
	static std::string bufGroup;

	if ( ImGui::BeginTable( "##table_users_main", 2, ImGuiTableFlags_NoSavedSettings ) ) {
		// First columns - users list
		ImGui::TableNextColumn();
		ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "availableUsers" ).c_str() );

		static ImGuiTextFilter filter;
		filter.Draw( m_LocalizationManager->GetTranslation( "search" ).c_str(), ImGui::GetFontSize() * 18 );

		if ( ImGui::BeginTable( "##table_main_users", 2, flags, outer_size_value ) ) {
			ImGui::TableSetupScrollFreeze( 0, 1 ); // Make top row always visible
			ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "name" ).c_str() );
			ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "group" ).c_str() );
			ImGui::TableHeadersRow();

			// Retrieve and print the test details
			for ( auto& studentPtr : m_Repository->GetStudents() ) {
				if ( !studentPtr ) {
					continue;
				}

				if ( !filter.PassFilter( studentPtr->GetName().c_str() ) ) {
					continue;
				}

				auto student_id = studentPtr->GetId();

				ImGui::TableNextRow();
				ImGui::PushID( student_id );

				for ( int column = 0; column < 2; column++ ) {
					const bool item_is_selected = ( current_student_id == student_id );

					if ( !ImGui::TableSetColumnIndex( column ) && column > 0 )
						continue;

					if ( column == 0 ) {
						if ( ImGui::Selectable( studentPtr->GetName().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
							current_student_id = student_id;
						}
					}
					else if ( column == 1 ) {
						if ( ImGui::Selectable( studentPtr->GetGroup().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap) ) {
							current_student_id = student_id;
						}
					}

					if ( item_is_selected )
						ImGui::SetItemDefaultFocus();
				}
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		// Updating stuff on switching users
		if ( prev_student_id != current_student_id ) {
			auto cur_student = m_Repository->FindStudentById( current_student_id );
			if ( cur_student ) {
				bufName = cur_student->GetName();
				bufGroup = cur_student->GetGroup();
			}

			prev_student_id = current_student_id;
		}

		// Second columns - test settings
		ImGui::TableNextColumn();
		ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "usersManagement" ).c_str() );

		if ( ImGui::InputTextWithHint( m_LocalizationManager->GetTranslation( "name" ).c_str(), 
									   m_LocalizationManager->GetTranslation( "studentsName" ).c_str(), &bufName) ) {
			auto cur_student = m_Repository->FindStudentById( current_student_id );
			if ( cur_student ) {
				cur_student->SetName( bufName );
			}
		}

		if ( ImGui::InputTextWithHint( m_LocalizationManager->GetTranslation( "group" ).c_str(), 
									   m_LocalizationManager->GetTranslation( "studentsGroup" ).c_str(), &bufGroup) ) {
			auto cur_student = m_Repository->FindStudentById( current_student_id );
			if ( cur_student ) {
				cur_student->SetGroup( bufGroup );
			}
		}

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.474f, 0.957f, 0.100f, 0.500f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.674f, 1.0f, 0.300f, 0.550f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.774f, 1.0f, 0.400f, 0.600f ) );
		if ( ImGui::Button( m_LocalizationManager->GetTranslation( "addNewStudent" ).c_str(), button_size) ) {
			// Get higher student ID for adding a new student (higher_id + 1)
			unsigned int higher_id = 0;
			for ( const auto& student : m_Repository->GetStudents() ) {
				if ( student && student->GetId() > higher_id ) {
					higher_id = student->GetId();
				}
			}

			std::shared_ptr<IStudent> new_student = std::make_shared<Student>();
			auto new_id = higher_id + 1u;
			new_student->SetId( new_id );

			if ( bufName.empty() ) {
				bufName = "Student name #" + std::to_string( new_id );
			}

			if ( bufGroup.empty() ) {
				bufGroup = "Student group #" + std::to_string( new_id );
			}

			new_student->SetName( bufName );
			new_student->SetGroup( bufGroup );

			m_Repository->AddStudent( new_student );

			bufName.clear();
			bufGroup.clear();
		}
		ImGui::PopStyleColor( 3 );

		bool need_disable_rm_test = current_student_id < 0;
		if ( need_disable_rm_test ) {
			ImGui::BeginDisabled();
		}

		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.943f, 0.304f, 0.215f, 0.500f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 1.0f, 0.504f, 0.415f, 0.550f ) );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 1.0f, 0.604f, 0.515f, 0.600f ) );
		if ( ImGui::Button( m_LocalizationManager->GetTranslation( "removeStudent" ).c_str(), button_size) ) {
			m_Repository->RemoveStudent( current_student_id );

			unsigned int higher_id = 0;
			for ( const auto& student : m_Repository->GetStudents() ) {
				if ( student && student->GetId() > higher_id ) {
					higher_id = student->GetId();
				}
			}

			if ( higher_id > 0 ) {
				current_student_id = higher_id;
			}
			else {
				current_student_id = -1;
			}

			bufName.clear();
			bufGroup.clear();
		}
		ImGui::PopStyleColor( 3 );

		if ( need_disable_rm_test ) {
			ImGui::EndDisabled();
		}

		ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "importexport" ).c_str() );

		static std::string filePath;
		static bool removeCurRepos = true;
		static imgui_addons::ImGuiFileBrowser file_dialog;

		ImGui::Checkbox( m_LocalizationManager->GetTranslation( "removeCurUsers" ).c_str(), &removeCurRepos);

		ImGui::InputText( m_LocalizationManager->GetTranslation( "pathToFile" ).c_str(), &filePath );

		if ( ImGui::Button( m_LocalizationManager->GetTranslation( "import" ).c_str(), ImVec2( 119, 25 ) ) ) {
			ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "importStudents" ).c_str() );
		}

		ImGui::SameLine();

		if ( ImGui::Button( m_LocalizationManager->GetTranslation( "export" ).c_str(), ImVec2( 119, 25 ) ) ) {
			ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "exportStudents" ).c_str() );
		}

		if ( file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "importStudents" ).c_str(), 
										 imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, 
										 ImVec2( 700, 310 ), ".wxm" ) ) {
			if ( removeCurRepos ) {
				m_Repository->Clear();
			}

			filePath = file_dialog.selected_path;
			m_Manager->ImportUsers( filePath, m_Repository );
		}

		if ( file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "exportStudents" ).c_str(),
										 imgui_addons::ImGuiFileBrowser::DialogMode::SAVE,
										 ImVec2( 700, 310 ), ".wxm" ) ) {
			filePath = file_dialog.selected_path;
			m_Manager->ExportUsers( filePath, m_Repository );
		}

		ImGui::EndTable();
	}
}
