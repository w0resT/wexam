#pragma once
#include <memory>

#include "../interfaces/itabview.h"
#include "../../model/interfaces/iappmodel.h"

class TabSettings : public ITabView {
public:
	TabSettings(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~TabSettings() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;
};