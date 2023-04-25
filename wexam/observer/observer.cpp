#include "observer.h"

void Observable::RegisterObserver(std::shared_ptr<IObserver> _observer) {
	m_Observers.insert(_observer);
}

void Observable::RemoveObserver(std::shared_ptr<IObserver> _observer) {
	m_Observers.erase(_observer);
}

void Observable::NotifyObservers() {
	for (const auto& observer : m_Observers) {
		observer->OnDataChanged();
	}
}
