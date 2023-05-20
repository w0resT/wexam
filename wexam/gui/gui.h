#pragma once
#include <memory>

#include "GLFW/glfw3.h"

#include "../interfaces/gui/igui.h"
#include "../interfaces/gui/iimguimanager.h"
#include "../interfaces/gui/ipagemanager.h"
#include "../interfaces/gui/iguiwrapper.h"
#include "../interfaces/model/iappmodel.h"
#include "../interfaces/tsystem/itestrepository.h"
#include "../interfaces/tsystem/itestmanager.h"
#include "../interfaces/localization/ilocalizationmanager.h"
#include "../interfaces/security/itotp.h"

struct ImFont;

class Gui : public IGui{
public:
	Gui( std::shared_ptr<IAppModel> _model ) :
		m_GLFWWindow( nullptr ), m_Model( std::move( _model ) ),
		m_Segoeui18( nullptr ), m_SegoeuiBold18( nullptr ), m_SegoeuiBold32( nullptr ),
		m_bShouldClose( false ), m_bSwitchAccout( true ),
		m_bIsAdmin( false ), m_bIsUser( false ) {}

	~Gui() {}

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

private:
	GLFWwindow* m_GLFWWindow;
	std::shared_ptr<IAppModel> m_Model;
	std::unique_ptr<IImGuiManager> m_ImguiManager;
	std::unique_ptr<IPageManager> m_PageManager;
	std::shared_ptr<IGUIWrapper> m_GUIWrapper;
	std::shared_ptr<ITestRepository> m_TestRepository;
	std::shared_ptr<ITestManager> m_TestManager;
	std::shared_ptr<ILocalizationManager> m_LocalizationManager;

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