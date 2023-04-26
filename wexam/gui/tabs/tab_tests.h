#pragma once
#include <memory>

#include "../interfaces/itabview.h"
#include "../../model/interfaces/iappmodel.h"

class TabTests : public ITabView {
public:
	TabTests(std::shared_ptr<IAppModel> _model) : m_Model(std::move(_model)) {}
	~TabTests() {}

	void Draw() override;

private:
	std::shared_ptr<IAppModel> m_Model;

};