#pragma once
#include <memory>

class IObserver {
public:
	virtual void OnDataChanged(/*Some data*/) = 0;

	virtual ~IObserver() {}
};

class IObservable {
public:
	virtual void RegisterObserver(std::shared_ptr<IObserver> observer) = 0;
	virtual void RemoveObserver(std::shared_ptr<IObserver> observer) = 0;

	virtual void NotifyObservers(/*Some data*/) = 0;

	virtual ~IObservable() {}
};