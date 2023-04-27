#pragma once
#include <memory>
#include <mutex>

#include "../interfaces/controller/iappcontroller.h"
#include "../interfaces/model/iappmodel.h"
#include "../interfaces/gui/igui.h"

class AppController : public IAppController {
public:
	AppController(std::shared_ptr<IAppModel> _model, std::shared_ptr<IGui> _gui)
		: m_Model(std::move(_model)), m_Gui(std::move(_gui)), m_bShouldClose(false) {}

	~AppController() {}

	void Start() override;

	virtual void OnDataChanged();

private:
	std::shared_ptr<IGui> m_Gui;
	std::shared_ptr<IAppModel> m_Model;

private:
	bool m_bShouldClose;

	std::mutex m_mutex;
	std::condition_variable m_cv;

};