#pragma once
#include "../../observer/interfaces/iobserver.h"

class IAppController: public IObserver {
public:
	virtual void Start() = 0;
};