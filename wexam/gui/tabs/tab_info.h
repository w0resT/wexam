#pragma once
#include <memory>

#include "../interfaces/itabview.h"
#include "../../model/interfaces/iappmodel.h"

class TabInfo : public ITabView {
public:
	TabInfo(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~TabInfo() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};