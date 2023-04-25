#pragma once
#include "interfaces/iobserver.h"
#include <set>

class Observable : public IObservable {
public:
	Observable() {}
	~Observable() {}

	void RegisterObserver(std::shared_ptr<IObserver> _observer) override;
	void RemoveObserver(std::shared_ptr<IObserver> _observer) override;

	void NotifyObservers(/*Some data*/) override;

private:
	std::set<std::shared_ptr<IObserver>> m_Observers;

};