#include "CollisionManager.h"
#include "GameObject.h"
#include "Collider.h"

using namespace Engine;

void Engine::CollisionManager::CheckCollision(std::list<GameObject*>* src, std::list<GameObject*>* dst)
{
	for (auto& Src : *src)
	{
		std::vector<Collider*>& srcColliders = Src->GetColliders();
		for (auto Dst : *dst)
		{
			std::vector<Collider*>& dstColliders = Dst->GetColliders();
			for (auto& srcCollider : srcColliders)
			{
				if (!srcCollider->IsActive()) continue;
				for (auto& dstCollider : dstColliders)
				{
					if (!dstCollider->IsActive()) continue;
					if (srcCollider == dstCollider) continue;

					bool isCollide = srcCollider->IsPrevColided(dstCollider);

					CollisionInfo infoSrc, infoDst;
					infoSrc.itSelf = srcCollider;
					infoSrc.other = dstCollider;
					infoDst.itSelf = dstCollider;
					infoDst.other = srcCollider;

					if (srcCollider->IsCollide(dstCollider))
					{
						srcCollider->InsertOther(dstCollider);
						dstCollider->InsertOther(srcCollider);

						if (isCollide)
						{
							Src->OnCollision(infoSrc);
							Dst->OnCollision(infoDst);
						}
						else
						{
							Src->OnCollisionEnter(infoSrc);
							Dst->OnCollisionEnter(infoDst);
						}
					}
					else
					{
						if (isCollide)
						{
							srcCollider->EraseOther(dstCollider);
							dstCollider->EraseOther(srcCollider);

							Src->OnCollisionExit(infoSrc);
							Dst->OnCollisionExit(infoDst);
						}
					}
				}
			}
		}
	}
}

CollisionManager* CollisionManager::Create()
{
	return new CollisionManager;
}

void CollisionManager::Free()
{
}
