#pragma once
#include <iostream>
#include <sstream>
#include <string>

class Debug
{
public:
	/// @brief Logs an informational message to the console.
	/// @tparam Args Types of the arguments to format.
	/// @param args Values to concatenate and print.
	template <typename... Args>
	static void Log(Args &&...args)
	{
		std::stringstream ss;
		(ss << ... << args);
		std::cout << GREEN << "[INFO]: " << RESET << ss.str() << '\n';
	}

	/// @brief Logs an error message to the standard error stream.
	/// @tparam Args Types of the arguments to format.
	/// @param args Values to concatenate and print.
	template <typename... Args>
	static void LogError(Args &&...args)
	{
		std::stringstream ss;
		(ss << ... << args);
		std::cout << RED << "[ERROR]: " << RESET << ss.str() << '\n';
	}

	/// @brief Logs a warning message to the console.
	/// @tparam Args Types of the arguments to format.
	/// @param args Values to concatenate and print.
	template <typename... Args>
	static void LogWarning(Args &&...args)
	{
		std::stringstream ss;
		(ss << ... << args);
		std::cout << YELLOW << "[WARN]: " << RESET << ss.str() << '\n';
	}

private:
	static constexpr const char *RESET = "\033[0m";
	static constexpr const char *RED = "\033[31m";
	static constexpr const char *GREEN = "\033[32m";
	static constexpr const char *YELLOW = "\033[33m";
};
