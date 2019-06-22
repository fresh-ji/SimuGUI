#pragma once

#ifndef ERRORLOG_H
#define ERRORLOG_H

#include <fstream>
#include <assert.h>
#include <time.h>

#include "Constant.h"

using namespace std;

class LOG {
public:
	static void logToSystem(const char* message) {

		std::ofstream file(LOG_FILE, std::ios::app);

		if (!file) {
			return;
		}

		file << getCurrentTime().data() << message << std::endl;
	}

	static string getCurrentTime() {
		time_t t = time(0);
		char ch[64];
		strftime(ch, sizeof(ch), "[%Y-%m-%d-%H-%M-%S]", localtime(&t));
		return ch;
	}
};

#endif