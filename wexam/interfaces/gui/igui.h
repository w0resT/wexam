#pragma once
#include "../../observer/observer.h"

class IGui: public Observable {
public:
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;

	virtual ~IGui() {}
};