#pragma once
#include <memory>

#include "../../interfaces/gui/ipageview.h"
#include "../../interfaces/model/iappmodel.h"

class PageUsers : public IPageView {
public:
	PageUsers(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~PageUsers() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};