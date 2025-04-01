#pragma once

#include "common/common.h"
#include "event.h"

namespace utils {
	enum class Severity {
		LOG,
		ERR,
		WARN
	};

	class Console {
	public:
		static void log(String message);
		static void error(String message);
		static void warn(String message);

		static Event<String, Severity> onMessage;
	private:
		static void print(String message, Severity type);
	};
}