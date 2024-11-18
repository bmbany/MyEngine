#include "Skeleton.h"

using namespace Engine;

void Engine::Skeleton::Initialize(const aiScene* paiScene, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfoTable)
{
	LoadSkeleton(_rootBone, paiScene->mRootNode, boneInfoTable);

	std::queue<const Bone*> bfs;
	bfs.push(&_rootBone);

	while (!bfs.empty())
	{
		const Bone* bone = bfs.front();
		bfs.pop();

		if (bone->name == "Bip01-Spine1")
		{
			_spine = *bone;
			break;
		}

		for (auto& child : bone->children)
			bfs.push(&child);
	}
}

bool Engine::Skeleton::LoadSkeleton(Bone& bone, aiNode* paiNode, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfoTable)
{
	if (boneInfoTable.find(paiNode->mName.C_Str()) != boneInfoTable.end())
	{
		bone.name = paiNode->mName.C_Str();
		bone.id = boneInfoTable[bone.name].first;
		bone.offset = boneInfoTable[bone.name].second;
		bone.local = XMMatrixTranspose(XMMATRIX(&paiNode->mTransformation.a1));

		for (unsigned int i = 0; i < paiNode->mNumChildren; i++)
		{
			Bone child;
			if (LoadSkeleton(child, paiNode->mChildren[i], boneInfoTable))
			{
				bone.children.push_back(child);
			}
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < paiNode->mNumChildren; i++)
		{
			if (LoadSkeleton(bone, paiNode->mChildren[i], boneInfoTable))
			{
				return true;
			}
		}
	}

	return false;
}

void Engine::Skeleton::Free()
{
}
