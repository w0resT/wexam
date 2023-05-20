#include "page_tests.h"

#include "../imguimanager.h"
#include "../../tsystem/test.h"
#include "../../tsystem/question_with_free_answer.h"
#include "../../tsystem/question_with_options.h"

#include "../../FileBrowser/ImGuiFileBrowser.h"

namespace FD {
	imgui_addons::ImGuiFileBrowser file_dialog;
}

// TODO: add try catch
void PageTests::Draw() {
	// Translate enum class types to string
	auto QuestionTypeToStr = [&] ( QuestionType type ) {
		switch ( type ) {
		case QuestionType::FreeAnswer:
			return m_LocalizationManager->GetTranslation( "freeAnswer" );
			break;
		case QuestionType::AnswerOptions:
			return m_LocalizationManager->GetTranslation( "answerWithOptions" );
			break;
		default:
			return m_LocalizationManager->GetTranslation( "error" );
			break;
		}
	};

	static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
		| ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY
		| ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_PadOuterX;

	static ImVec2 outer_size_value = ImVec2( 0.0f, 0.0f );
	static ImVec2 button_size = ImVec2( 247, 25 );

	static int current_test_id = -1;
	static int current_question_id = -1;

	static int prev_question_id = -1;

	static std::string bufTitle;
	static std::string bufDesc;

	static std::string bufQuestion;
	static std::string bufCorrectAnswer;

	static std::string bufQuestionOpt;
	static std::vector<std::string> question_opts;

	static bool show_main_page = true;
	static bool show_test_editor = false;

	if ( show_main_page ) {
		if ( ImGui::BeginTable( "##table_tests_main", 2, ImGuiTableFlags_NoSavedSettings ) ) {
			// First columns - test list
			ImGui::TableNextColumn();
			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "availableTests" ).c_str() );

			static ImGuiTextFilter filter;
			filter.Draw( m_LocalizationManager->GetTranslation( "search" ).c_str(), ImGui::GetFontSize() * 18 );

			if ( ImGui::BeginTable( "##table_main_tests", 1, flags, outer_size_value ) ) {
				// Retrieve and print the test details
				for ( auto& testPtr : m_Repository->GetTests() ) {
					if ( !testPtr ) {
						continue;
					}

					if ( !filter.PassFilter( testPtr->GetTitle().c_str() ) ) {
						continue;
					}

					auto test_id = testPtr->GetId();

					ImGui::TableNextRow();
					ImGui::PushID( test_id );

					for ( int column = 0; column < 1; column++ ) {
						const bool item_is_selected = ( current_test_id == test_id );

						if ( !ImGui::TableSetColumnIndex( column ) && column > 0 )
							continue;

						if ( ImGui::Selectable( testPtr->GetTitle().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap ) ) {
							current_test_id = test_id;
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
			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "testManagement" ).c_str() );

			ImGui::InputText( m_LocalizationManager->GetTranslation( "title" ).c_str(), &bufTitle );

			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.474f, 0.957f, 0.100f, 0.500f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.674f, 1.0f, 0.300f, 0.550f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.774f, 1.0f, 0.400f, 0.600f ) );
			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "addNewTest" ).c_str(), button_size ) ) {
				// Get higher test ID for adding new test (higher_id + 1)
				unsigned int higher_test_id = 0;
				for ( const auto& test : m_Repository->GetTests() ) {
					if ( test && test->GetId() > higher_test_id ) {
						higher_test_id = test->GetId();
					}
				}

				std::shared_ptr<ITest> new_test = std::make_shared<Test>();
				auto new_test_id = higher_test_id + 1u;
				new_test->SetId( new_test_id );

				if ( bufTitle.empty() ) {
					bufTitle = "New test #" + std::to_string( new_test_id );
				}

				new_test->SetTitle( bufTitle );

				m_Repository->AddTest( new_test );

				bufTitle.clear();
			}
			ImGui::PopStyleColor( 3 );

			bool need_disable_rm_test = current_test_id < 0;
			if ( need_disable_rm_test ) {
				ImGui::BeginDisabled();
			}

			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.943f, 0.304f, 0.215f, 0.500f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 1.0f, 0.504f, 0.415f, 0.550f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 1.0f, 0.604f, 0.515f, 0.600f ) );
			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "removeTest" ).c_str(), button_size ) ) {
				m_Repository->RemoveTest( current_test_id );

				unsigned int higher_test_id = 0;
				for ( const auto& test : m_Repository->GetTests() ) {
					if ( test && test->GetId() > higher_test_id ) {
						higher_test_id = test->GetId();
					}
				}

				if ( higher_test_id > 0 ) {
					current_test_id = higher_test_id;
				}
				else {
					current_test_id = -1;
				}
			}
			ImGui::PopStyleColor( 3 );

			if ( need_disable_rm_test ) {
				ImGui::EndDisabled();
			}

			bool need_disable_edit_test = current_test_id < 0;
			if ( need_disable_edit_test ) {
				ImGui::BeginDisabled();
			}

			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.9f, 0.7f, 0.0f, 0.500f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 1.0f, 0.9f, 0.2f, 0.550f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 1.0f, 1.0f, 0.3f, 0.600f ) );
			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "editTest" ).c_str(), button_size ) ) {
				show_test_editor = true;
				show_main_page = false;

				auto cur_test = m_Repository->FindTestById( current_test_id );
				if ( cur_test ) {
					bufTitle = cur_test->GetTitle();
					bufDesc = cur_test->GetDescription();
				}
			}
			ImGui::PopStyleColor( 3 );

			if ( need_disable_edit_test ) {
				ImGui::EndDisabled();
			}

			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "importexport" ).c_str() );

			static std::string filePath;

			static bool needDecrypt = false;
			static bool needCorAns = true;
			static bool removeCurRepos = true;

			ImGui::Checkbox( m_LocalizationManager->GetTranslation( "useEncryption" ).c_str(), &needDecrypt );
			ImGui::SameLine();
			ImGui::Checkbox( m_LocalizationManager->GetTranslation( "addCorAnswers" ).c_str(), &needCorAns );
			ImGui::Checkbox( m_LocalizationManager->GetTranslation( "removeCurTest" ).c_str(), &removeCurRepos );

			ImGui::InputText( m_LocalizationManager->GetTranslation( "pathToFile" ).c_str(), &filePath );

			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "import" ).c_str(), ImVec2(119, 25) ) ) {
				ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "importTest" ).c_str() );
			}

			ImGui::SameLine();

			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "export" ).c_str(), ImVec2( 119, 25 ) ) ) {
				ImGui::OpenPopup( m_LocalizationManager->GetTranslation( "exportTest" ).c_str() );
			}
			
			if ( FD::file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "importTest" ).c_str(), 
												 imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, 
												 ImVec2( 700, 310 ), ".wxm" ) ) {
				if ( removeCurRepos ) {
					m_Repository->Clear();
				}

				filePath = FD::file_dialog.selected_path;
				m_Manager->ImportTests( filePath, m_Repository, needDecrypt, needCorAns, false );
			}

			if ( FD::file_dialog.showFileDialog( m_LocalizationManager->GetTranslation( "exportTest" ).c_str(), 
												 imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, 
												 ImVec2( 700, 310 ), ".wxm" ) ) {
				filePath = FD::file_dialog.selected_path;
				m_Manager->ExportTests( filePath, m_Repository, needDecrypt, needCorAns, false );
			}

			ImGui::EndTable();
		}
	}
	else if ( show_test_editor ) {
		static int qt_current_idx = 0;
		static int question_opt_current_idx = -1;
		static int prev_question_opt_idx = -1;

		auto cur_test = m_Repository->FindTestById( current_test_id );
		assert( cur_test && "Failed to add a new question: test id error" );

		if ( ImGui::BeginTable( "##table_tests_editor", 2, ImGuiTableFlags_NoSavedSettings ) ) {
			// First column - question list
			ImGui::TableNextColumn();
			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "questions" ).c_str() );

			static ImGuiTextFilter filter;
			filter.Draw( m_LocalizationManager->GetTranslation( "search" ).c_str(), ImGui::GetFontSize() * 18 );

			if ( ImGui::BeginTable( "##table_questions", 2, flags, outer_size_value ) ) {
				ImGui::TableSetupScrollFreeze( 0, 1 ); // Make top row always visible
				//ImGui::TableSetupColumn( "ID", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthFixed ); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
				ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "question" ).c_str() );
				ImGui::TableSetupColumn( m_LocalizationManager->GetTranslation( "type" ).c_str() );
				ImGui::TableHeadersRow();

				// Retrieve and print the questions in the test
				auto questions = cur_test->GetQuestions();

				for ( auto& questionPtr : questions ) {
					if ( !filter.PassFilter( questionPtr->GetQuestion().c_str() ) )
						continue;

					auto question_id = questionPtr->GetId();

					ImGui::TableNextRow();

					ImGui::PushID( question_id );

					for ( int column = 0; column < 2; column++ ) {
						const bool item_is_selected = ( current_question_id == question_id );

						if ( !ImGui::TableSetColumnIndex( column ) && column > 0 )
							continue;
						if ( column == 0 ) {
							if ( ImGui::Selectable( questionPtr->GetQuestion().c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap) ) {
								current_question_id = question_id;
							}
						}
						else if ( column == 1 ) {
							if ( ImGui::Selectable( QuestionTypeToStr( questionPtr ->GetQuestionType()).c_str(), item_is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap) ) {
								current_question_id = question_id;
							}
						}

						if ( item_is_selected )
							ImGui::SetItemDefaultFocus();
					}

					ImGui::PopID();
				}

				ImGui::EndTable();
			}

			// Updating buffer for question
			if ( prev_question_id != current_question_id ) {
				auto question = cur_test->FindQuestionById( current_question_id );
				if ( question ) {
					bufQuestion = question->GetQuestion();
					bufCorrectAnswer = question->GetCorrectAnswer();

					if ( question->GetQuestionType() == QuestionType::FreeAnswer ) {
						qt_current_idx = 0;
						question_opts.clear();
					}
					else if ( question->GetQuestionType() == QuestionType::AnswerOptions ) {
						qt_current_idx = 1;
						question_opts = question->GetAnswerOptions();
					}
					else {
						assert( "Unknown question type" );
					}

					bufQuestionOpt.clear();

					
				}

				prev_question_id = current_question_id;
			}

			// Second column - test settings
			ImGui::TableNextColumn();
			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "testEditing" ).c_str() );

			if ( ImGui::InputText( m_LocalizationManager->GetTranslation( "testTitle" ).c_str(), &bufTitle ) ) {
				cur_test->SetTitle( bufTitle );
			}

			if ( ImGui::InputTextMultiline( m_LocalizationManager->GetTranslation( "testDescription" ).c_str(), &bufDesc, ImVec2( 0, 70 ) ) ) {
				cur_test->SetDescription( bufDesc );
			}

			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "questionsEditing" ).c_str() );
			if ( ImGui::InputText( m_LocalizationManager->GetTranslation( "questionTitle" ).c_str(), &bufQuestion ) ) {
				auto question = cur_test->FindQuestionById( current_question_id );
				if ( question ) {
					question->SetQuestion( bufQuestion );
				}
			}

			if ( ImGui::Combo( m_LocalizationManager->GetTranslation( "questionType" ).c_str(), &qt_current_idx, "Free answer\0Answer with options\0\0" ) ) {
				// Replacing test for switching types
				auto cur_question = cur_test->FindQuestionById( current_question_id );
				if ( cur_question ) {
					// Obj for new question
					std::shared_ptr<IQuestion> new_question;

					// Free answer
					if ( qt_current_idx == 0 ) {
						new_question = std::make_shared<QuestionWithFreeAnswer>();
						question_opts.clear();
					}
					// Answer with options
					else if ( qt_current_idx == 1 ) {
						new_question = std::make_shared<QuestionWithAnswerOptions>();
					}

					new_question->SetId( cur_question->GetId() );
					new_question->SetQuestion( cur_question->GetQuestion() );
					new_question->SetCorrectAnswer( cur_question->GetCorrectAnswer() );

					// Replacing
					cur_test->RemoveQuestion( cur_question->GetId() );
					cur_test->AddQuestion( new_question );
				}
			}

			bool need_disable_question_stuff = current_question_id < 0;
			if ( need_disable_question_stuff ) {
				ImGui::BeginDisabled();
			}

			if ( ImGui::InputText( m_LocalizationManager->GetTranslation( "correctAnswer" ).c_str(), &bufCorrectAnswer ) ) {
				auto cur_question = cur_test->FindQuestionById( current_question_id );
				if ( cur_question ) {
					cur_question->SetCorrectAnswer( bufCorrectAnswer );
				}
			}

			if ( need_disable_question_stuff ) {
				ImGui::EndDisabled();
			}

			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.474f, 0.957f, 0.100f, 0.500f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.674f, 1.0f, 0.300f, 0.550f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.774f, 1.0f, 0.400f, 0.600f ) );
			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "addQuestion" ).c_str(), ImVec2(119, 25) ) ) {
				// Set a question with free answer as default type of questions
				std::shared_ptr<IQuestion> new_question = std::make_shared<QuestionWithFreeAnswer>();

				// Get higher question ID for the new question (higher_id + 1)
				auto questions = cur_test->GetQuestions();
				unsigned int higher_question_id = 0;
				for ( auto it = questions.begin(); it != questions.end(); ++it ) {
					if ( ( *it )->GetId() > higher_question_id ) {
						higher_question_id = ( *it )->GetId();
					}
				}

				auto new_question_id = higher_question_id + 1u;
				bufQuestion = "New question #" + std::to_string( new_question_id );

				new_question->SetId( new_question_id );
				new_question->SetQuestion( bufQuestion );
				new_question->SetCorrectAnswer( "None" );

				// Add question to the test
				cur_test->AddQuestion( new_question );

				// Clearing question buffer
				bufQuestion.clear();
			}
			ImGui::PopStyleColor( 3 );

			ImGui::SameLine();

			ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.943f, 0.304f, 0.215f, 0.500f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 1.0f, 0.504f, 0.415f, 0.550f ) );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 1.0f, 0.604f, 0.515f, 0.600f ) );

			bool need_disable_rm_question = current_question_id < 0;
			if ( need_disable_rm_question ) {
				ImGui::BeginDisabled();
			}

			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "removeQuestion" ).c_str(), ImVec2( 119, 25 ) ) ) {
				cur_test->RemoveQuestion( current_question_id );

				// Get higher question ID for the new question (higher_id + 1)
				auto questions = cur_test->GetQuestions();
				unsigned int higher_question_id = 0;
				for ( auto it = questions.begin(); it != questions.end(); ++it ) {
					if ( ( *it )->GetId() > higher_question_id ) {
						higher_question_id = ( *it )->GetId();
					}
				}

				if ( higher_question_id > 0 ) {
					current_question_id = higher_question_id;
				}
				else {
					current_question_id = -1;
				}

				bufQuestion.clear();
			}

			if ( need_disable_rm_question ) {
				ImGui::EndDisabled();
			}

			ImGui::PopStyleColor( 3 );

			ImGui::SeparatorText( m_LocalizationManager->GetTranslation( "answerEditing" ).c_str() );
			// Answer with options
			auto cur_question = cur_test->FindQuestionById( current_question_id );
			if ( cur_question && cur_question->GetQuestionType() == QuestionType::AnswerOptions ) {

				// Check if the answer option was changed
				if ( question_opt_current_idx > -1 && prev_question_opt_idx != question_opt_current_idx ) {
					assert( !question_opts.empty() && "Question options were empty!" );

					bufQuestionOpt = question_opts[ question_opt_current_idx ];

					prev_question_opt_idx = question_opt_current_idx;
				}

				if ( ImGui::InputText( m_LocalizationManager->GetTranslation( "answerOption" ).c_str(), &bufQuestionOpt ) ) {
					if ( question_opt_current_idx > -1 && !question_opts.empty() ) {
						question_opts[ question_opt_current_idx ] = bufQuestionOpt;
						auto cur_question = cur_test->FindQuestionById( current_question_id );
						if ( cur_question ) {
							cur_question->SetAnswerOptions( question_opts );
						}
					}
				}

				ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.474f, 0.957f, 0.100f, 0.500f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 0.674f, 1.0f, 0.300f, 0.550f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 0.774f, 1.0f, 0.400f, 0.600f ) );
				if ( ImGui::Button( m_LocalizationManager->GetTranslation( "addAnswer" ).c_str(), ImVec2( 119, 25 ) ) ) {
					bufQuestionOpt = "Question option #" + std::to_string( question_opts.size() );
					question_opts.push_back( bufQuestionOpt );
					auto cur_question = cur_test->FindQuestionById( current_question_id );
					if ( cur_question ) {
						cur_question->SetAnswerOptions( question_opts );
					}
					bufQuestionOpt.clear();
				}
				ImGui::PopStyleColor( 3 );

				ImGui::SameLine();

				ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.943f, 0.304f, 0.215f, 0.500f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4( 1.0f, 0.504f, 0.415f, 0.550f ) );
				ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4( 1.0f, 0.604f, 0.515f, 0.600f ) );

				bool need_disable_rm_opt = question_opt_current_idx < 0;
				if ( need_disable_rm_opt ) {
					ImGui::BeginDisabled();
				}

				if ( ImGui::Button( m_LocalizationManager->GetTranslation( "removeAnswer" ).c_str(), ImVec2( 119, 25 ) ) ) {
					question_opts.erase( question_opts.begin() + question_opt_current_idx );
					auto cur_question = cur_test->FindQuestionById( current_question_id );
					if ( cur_question ) {
						cur_question->SetAnswerOptions( question_opts );
					}
					
					// Get higher question ID for the new question (higher_id + 1)
					auto higher_answer_opt = question_opts.size() - 1;

					if ( higher_answer_opt >= 0 ) {
						question_opt_current_idx = higher_answer_opt;
					}
					else {
						question_opt_current_idx = -1;
					}

					bufQuestionOpt.clear();
				}

				if ( need_disable_rm_opt ) {
					ImGui::EndDisabled();
				}

				ImGui::PopStyleColor( 3 );

				// add answer option, remove answer option
				if ( ImGui::BeginListBox( m_LocalizationManager->GetTranslation( "answerOptions" ).c_str(), ImVec2(0, 100) ) ) {
					for ( int n = 0; n < question_opts.size(); n++ ) {
						const bool is_selected = ( question_opt_current_idx == n );
						ImGui::PushID( std::string( std::string( "##answer_opt" ) + std::to_string( n ) ).c_str() );
						if ( ImGui::Selectable( question_opts[ n ].c_str(), is_selected) )
							question_opt_current_idx = n;

						// Set the initial focus when opening the combo
						if ( is_selected )
							ImGui::SetItemDefaultFocus();

						ImGui::PopID();
					}
					ImGui::EndListBox();
				}
			}

			ImGui::Separator();

			if ( ImGui::Button( m_LocalizationManager->GetTranslation( "back" ).c_str(), button_size ) ) {
				show_test_editor = false;
				show_main_page = true;

				bufTitle.clear();
				bufDesc.clear();
				bufQuestion.clear();
				bufCorrectAnswer.clear();
				bufQuestionOpt.clear();
				question_opts.clear();

				qt_current_idx = 0;
				question_opt_current_idx = -1;
				prev_question_opt_idx = -1;
			}

			ImGui::EndTable();
		}
	}
}
