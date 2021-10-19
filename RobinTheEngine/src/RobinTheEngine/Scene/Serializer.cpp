#include "Serializer.h"
#include "GameObject.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace RTE
{
	Serializer::Serializer(Scene& scene) : scene(scene) { }

	void Serializer::Serialize(std::string path, SerializerMode mode = SerializerMode::Text)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene name" << YAML::Value << scene.name;
		out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;

		scene.registry.each([&](auto entity)
			{
				out << YAML::BeginMap; // GameObject
				out << YAML::Key << "GameObject ID" << YAML::Value << "TODO"; // TODO: GameObject ID goes here

				// Transform (GameObject always has Transform)
				{
					Transform& transform = scene.registry.get<Transform>(entity);
					out << YAML::Key << "Transform" << YAML::BeginMap;

					out << YAML::Key << "Parent" << YAML::Value << "TODO"; // transform->parent->id
					out << YAML::Key << "Translation" << YAML::Value << transform.GetPosition();
					out << YAML::Key << "Rotation" << YAML::Value << transform.GetRotation();
					out << YAML::Key << "Scale" << YAML::Value << transform.GetScale();

					out << YAML::EndMap;
				} // Transform
				
				// MeshRenderer
				if (MeshRenderer* meshRenderer = scene.registry.try_get<MeshRenderer>(entity))
				{
					out << YAML::Key << "MeshRenderer" << YAML::BeginMap;

					out << YAML::Key << "Mesh" << YAML::Value << meshRenderer->GetMesh().GetResourceID();
					out << YAML::Key << "Material" << YAML::Value << meshRenderer->GetMaterial().GetResourceID();

					out << YAML::EndMap;
				} // MeshRenderer
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	void Serializer::Deserialize(std::string path, SerializerMode mode = SerializerMode::Text)
	{
		YAML::Node data;
		data = YAML::LoadFile(path);

		if (!data["Scene name"]) throw BadScene();

		scene.name = data["Scene name"].as<std::string>();

		auto gameObjects = data["GameObjects"];
		if (gameObjects)
		{
			for (auto data : gameObjects)
			{
				std::string id = data["GameObject ID"].as<std::string>(); // TODO

				GameObject gameObject = scene.CreateGameObject();

				if (auto componentData = data["Transform"])
				{
					Transform& transform = gameObject.GetComponent<Transform>();

					// tc.SetParent(transformComponent["Parent"].as<GameObject ID>());
					transform.SetPosition(componentData["Translation"].as<DirectX::XMFLOAT3>());
					transform.SetRotation(componentData["Rotation"].as<DirectX::XMFLOAT3>());
					transform.SetScale(componentData["Scale"].as<DirectX::XMFLOAT3>());
				}

				if (auto componentData = data["MeshRenderer"])
				{
					IMesh mesh = ResourceManager::Get<IMesh>(data["Mesh"].as<std::string>());
					IMaterial material = ResourceManager::Get<IMaterial>(data["Material"].as<std::string>());
					gameObject.AddComponent<MeshRenderer>(mesh, material);
				}
			}
		}

	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const DirectX::XMFLOAT3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
}

// YAML convert specialization
namespace YAML {

	template<>
	struct convert<DirectX::XMFLOAT3>
	{
		static Node encode(const DirectX::XMFLOAT3& data)
		{
			Node node;
			node.push_back(data.x);
			node.push_back(data.y);
			node.push_back(data.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT3& data)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			data.x = node[0].as<float>();
			data.y = node[1].as<float>();
			data.z = node[2].as<float>();
			return true;
		}
	};
}