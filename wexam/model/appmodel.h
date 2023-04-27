#pragma once
#include "../interfaces/model/iappmodel.h"

class AppModel : public IAppModel {
public:
	AppModel() {}
	~AppModel() {}

	void UpdateData() override;
	void GetAllData() override;

private:

};
