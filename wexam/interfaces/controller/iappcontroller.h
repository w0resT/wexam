#pragma once
#include "../iobserver.h"

class IAppController: public IObserver {
public:
	virtual void Start() = 0;

	virtual ~IAppController() {}
};