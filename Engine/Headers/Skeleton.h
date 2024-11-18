#pragma once
#include "Base.h"

namespace Engine
{
	struct Bone
	{
		Matrix local;
		Matrix offset;
		std::string name;
		std::vector<Bone> children;
		int id;
	};

	class Skeleton : public Base
	{
		friend class AnimationController;
	public:
		explicit Skeleton() = default;
		virtual ~Skeleton() = default;

	public:
		const Bone& GetBone() const { return _rootBone; }

	public:
		void Initialize(const aiScene* paiScene, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfo);

	private:
		bool LoadSkeleton(Bone& bone, aiNode* paiNode, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfo);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		Bone _rootBone;
		Bone _spine;
	};
}