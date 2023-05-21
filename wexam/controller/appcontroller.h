#pragma once
#include <memory>
#include <mutex>
#include <condition_variable>

#include "../interfaces/controller/iappcontroller.h"
#include "../interfaces/gui/igui.h"

class AppController : public IAppController {
public:
	AppController(std::shared_ptr<IGui> _gui) : m_Gui(std::move(_gui)), m_bShouldClose(false) {}

	void Start() override;

	virtual void OnDataChanged();

private:
	std::shared_ptr<IGui> m_Gui;

private:
	bool m_bShouldClose;

	std::mutex m_mutex;
	std::condition_variable m_cv;

};