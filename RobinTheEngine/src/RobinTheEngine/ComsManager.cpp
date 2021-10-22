#include "rtepch.h"
#include "ComsManager.h"
#include "RobinTheEngine/d3dUtils.h"


bool RTE::ComsManager::IsHaveComPtrResource(std::string name)
{
	return resourceMap.find(name) != resourceMap.end();
}

Microsoft::WRL::ComPtr<ID3D11DeviceChild> RTE::ComsManager::GetComPtrResource(std::string& name)
{
#ifdef _DEBUG
	RTE_ASSERT(this->IsHaveComPtrResource(name), "Resource with that name does not exist!")
#endif // 

		return resourceMap[name];
}

