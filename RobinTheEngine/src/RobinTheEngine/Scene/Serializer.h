#pragma once
#include "Scene.h"

namespace RTE
{
	class Serializer
	{
		// For future
		enum class SerializerMode
		{
			Text,
			Binary
		};

	private:
		Scene& scene;

	public:
		Serializer(Scene& scene);

		void Serialize(std::string path, SerializerMode mode = SerializerMode::Text);
		void Deserialize(std::string path, SerializerMode mode = SerializerMode::Text);
	};

	class SerializerException : std::exception {};
	class BadScene : SerializerException {};
}