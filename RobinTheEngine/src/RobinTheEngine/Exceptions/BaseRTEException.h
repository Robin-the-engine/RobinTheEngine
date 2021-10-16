#pragma once

#include <exception>

namespace RTE {
	class BaseRTEException: std::exception {
		using std::exception::exception;
	};
}
