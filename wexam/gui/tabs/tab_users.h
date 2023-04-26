#pragma once
#include <memory>

#include "../interfaces/itabview.h"
#include "../../model/interfaces/iappmodel.h"

class TabUsers : public ITabView {
public:
	TabUsers(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~TabUsers() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};