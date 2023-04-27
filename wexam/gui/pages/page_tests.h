#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/model/iappmodel.h"

class PageTests : public IPageView {
public:
	PageTests(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~PageTests() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};