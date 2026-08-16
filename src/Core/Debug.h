#pragma once
#include <iostream>
#include <sstream>
#include <string>
class Debug
{

	static constexpr const char* RESET = "\033[0m";
	static constexpr const char* RED = "\033[31m";
	static constexpr const char* GREEN = "\033[32m";
	static constexpr const char* YELLOW = "\033[33m";

public:
	template<typename... Args>
	static void Log(Args&&... args) {
		std::stringstream ss;
		(ss << ... << args);
		std::cout << GREEN << "[INFO]: " << RESET << ss.str() << std::endl;
	}

	template<typename... Args>
	static void LogError(Args&&... args) {
		std::stringstream ss;
		(ss << ... << args);
		std::cout << RED << "[ERROR]: " << RESET << ss.str() << std::endl;

	}

	template<typename... Args>
	static void LogWarning(Args&&... args) {
		std::stringstream ss;
		(ss << ... << args);
		std::cout << YELLOW << "[WARN]: " << RESET << ss.str() << std::endl;
	}
};

