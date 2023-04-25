#pragma once

class IAppModel {
public: 
	virtual void UpdateData() = 0;
	virtual void GetAllData() = 0;
};