#pragma once
#include <memory>
#include <unordered_map>

#include "GLFW/glfw3.h"

#include "../interfaces/gui/igui.h"
#include "../interfaces/gui/iimguimanager.h"
#include "../interfaces/gui/ipagemanager.h"
#include "../interfaces/tsystem/itestrepository.h"
#include "../interfaces/tsystem/itestmanager.h"
#include "../interfaces/localization/ilocalizationmanager.h"
#include "../interfaces/security/itotp.h"
#include "../interfaces/users/istudentrepository.h"
#include "../interfaces/users/iusermanager.h"

struct ImFont;

class Gui : public IGui{
public:
	Gui( ) :
		m_GLFWWindow( nullptr ), m_Segoeui18( nullptr ), 
		m_SegoeuiBold18( nullptr ), m_SegoeuiBold32( nullptr ),
		m_bShouldClose( false ), m_bSwitchAccout( true ),
		m_bIsAdmin( false ), m_bIsUser( false ) {}


	void Init() override;
	void Run() override;
	void Shutdown() override;

private:
	void Draw();
	void CreateTabs();
	void CreateFonts();
	void LoadStyle();

	void DrawAuthPage();
	void DrawBottomBar();
	void DrawLeftChild();
	void DrawRightChild();
	void DrawStudentPage();

private:
	GLFWwindow* m_GLFWWindow;
	std::unique_ptr<IImGuiManager> m_ImguiManager;
	std::unique_ptr<IPageManager> m_PageManager;
	std::shared_ptr<ITestRepository> m_TestRepository;
	std::shared_ptr<ITestManager> m_TestManager;
	std::shared_ptr<IStudentRepository> m_UserRepository;
	std::shared_ptr<IUserManager> m_UserManager;
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

	std::unordered_map<std::string, std::shared_ptr<ITest>> m_PassedTest;

	std::shared_ptr<ITOTP> m_TOTP;
	std::string m_key;

	ImFont* m_Segoeui18;
	ImFont* m_SegoeuiBold18;
	ImFont* m_SegoeuiBold32;

	bool m_bShouldClose;
	bool m_bSwitchAccout;
	bool m_bIsAdmin;
	bool m_bIsUser;
};