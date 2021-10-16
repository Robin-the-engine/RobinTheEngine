#pragma once

namespace RTE {
	enum class JobPriority {
		LOWEST = 0,
		LOW = 100,
		MEDIUM = 500,
		HIGH = 900,
		HIGHEST = 1000
	};
}