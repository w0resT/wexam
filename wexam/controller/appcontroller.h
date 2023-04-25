#pragma once
#include <memory>

#include "interfaces/iappcontroller.h"
#include "../model/interfaces/iappmodel.h"
#include "../gui/interfaces/igui.h"

class AppController : public IAppController {
public:
	AppController(std::shared_ptr<IAppModel> _model, std::shared_ptr<IGui> _gui)
		: m_Model(std::move(_model)), m_Gui(std::move(_gui)), m_bShouldClose(false) {}

	~AppController() {}

	void Start() override;

	virtual void OnDataChanged();

private:
	bool m_bShouldClose;

	std::shared_ptr<IGui> m_Gui;
	std::shared_ptr<IAppModel> m_Model;

};