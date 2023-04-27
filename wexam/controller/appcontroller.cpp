#include <iostream>
#include <thread>

#include "appcontroller.h"

void AppController::Start() {
	using namespace std::chrono_literals;

	while (/*!m_bShouldClose*/true) {
		// Waiting for event
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this] { return m_bShouldClose; });

		std::cout << "[Thread: " << std::this_thread::get_id() << "] " << "Controller thread" << std::endl;

		m_bShouldClose = false;

		//std::this_thread::sleep_for(1s);
		break;
	}
}

void AppController::OnDataChanged() {
	std::cout << "[Thread: " << std::this_thread::get_id() << "] " << "Controller on data change" << std::endl;

	m_bShouldClose = true;

	std::unique_lock<std::mutex> lock(m_mutex);
	lock.unlock();
	m_cv.notify_one();
}
