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
		//We always should call BaseResource(id)
		BaseResource() = delete;
		BaseResource(const BaseResource&) = default;

		BaseResource(ResourceID id) : id(id) { }
		virtual ~BaseResource() {};
		const ResourceID GetResourceID() const { return id; }
	};

}
