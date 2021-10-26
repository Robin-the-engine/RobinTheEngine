#include "rtepch.h"
#include "GameObject.h"


namespace RTE
{
	GameObject Component::GetGameObject()
	{
		return scene->GetGameObject(owner);
	}
}