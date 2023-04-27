#pragma once
#include <memory>

#include "../interfaces/ipageview.h"
#include "../../model/interfaces/iappmodel.h"

class PageInfo : public IPageView {
public:
	PageInfo(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~PageInfo() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};