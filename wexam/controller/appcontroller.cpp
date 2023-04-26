#include <iostream>
#include <thread>

#include "appcontroller.h"

void AppController::Start() {
	using namespace std::chrono_literals;

	while (!m_bShouldClose) {
		std::cout << "[Thread: " << std::this_thread::get_id() << "] " << "Controller thread" << std::endl;

		std::this_thread::sleep_for(1s);
	}
}

void AppController::OnDataChanged() {
	std::cout << "[Thread: " << std::this_thread::get_id() << "] " << "Controller on data change" << std::endl;
	m_bShouldClose = true;
}
