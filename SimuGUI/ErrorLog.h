#pragma once

#ifndef ERRORLOG_H
#define ERRORLOG_H

#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>


class LOG {
public:
	static void logToSystem(const char* message) {

		std::ofstream file(".\\FMI\\error.log", std::ios::app);

		if (!file) {
			return;
		}

		auto now = std::chrono::system_clock::now();
		auto ticks = std::chrono::system_clock::to_time_t(now);
		auto local_time = localtime(&ticks);

		file << "[" << std::put_time(local_time, "%F") << "  "
			<< std::put_time(local_time, "%T") << "]  " << message << std::endl;

	}
};

#endif