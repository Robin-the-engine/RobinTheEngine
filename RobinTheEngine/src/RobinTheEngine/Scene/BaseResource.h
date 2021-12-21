#pragma once
#include <string>

namespace RTE {

	class BaseResource
	{
	public:
		using ResourceID = std::string;
	protected:
		ResourceID id = "invalid";

	public:
		BaseResource() = default;
		BaseResource(const BaseResource&) = default;

		BaseResource(ResourceID id) : id(id) { }

		const ResourceID GetResourceID() const { return id; }
	};

}
