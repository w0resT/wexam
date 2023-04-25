#pragma once
#include "interfaces/iappmodel.h"

class AppModel : public IAppModel {
public:
	AppModel() {}
	~AppModel() {}

	void UpdateData() override;
	void GetAllData() override;

private:

};
