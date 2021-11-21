#pragma once
#include "Scene.h"

namespace RTE
{
	class Serializer
	{
		// For future
		// if will you ever use it in future don't forget to update
		// void registerUserType<Serializer>() in ScriptingAPI.h
		// to be able to use this enum from lua
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