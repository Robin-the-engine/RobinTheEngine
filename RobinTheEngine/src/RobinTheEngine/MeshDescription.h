#pragma once

#include <string>

namespace RTE {
	struct MeshDesc {
		std::string key;
		int layout;
		std::string path;
		MeshDesc() :layout(-1), path(""), key("") {}
	};
}
