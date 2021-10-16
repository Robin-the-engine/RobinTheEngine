#pragma once

#include "BaseRTEException.h"

namespace RTE {
	class JobException: BaseRTEException {
		using BaseRTEException::BaseRTEException;
	};
}