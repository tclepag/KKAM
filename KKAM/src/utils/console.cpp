#include "utils/console.h"

namespace utils {
	Event<String, Severity> Console::onMessage;
	void Console::log(String message) {
		print(message, Severity::LOG);
	}
	void Console::error(String message) {
		print(message, Severity::ERR);
	}
	void Console::warn(String message) {
		print(message, Severity::WARN);
	}
	void Console::print(String message, Severity type) {
		onMessage.notify(message, type);

#ifdef _MSC_VER
		OutputDebugString(message.wc_str());
#endif
	}
}