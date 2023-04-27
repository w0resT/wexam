#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/model/iappmodel.h"

class PageSettings : public IPageView {
public:
	PageSettings(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~PageSettings() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;
};