#include "ResourceManager.h"
#include "Resource.h"

using namespace Engine;

void Engine::ResourceManager::Free()
{
	for (auto& type : _resources)
	{
		for (auto& resource : type.second)
		{
			if (resource.second.use_count())
			{
				int a = 0;
			}
		}
	}
}