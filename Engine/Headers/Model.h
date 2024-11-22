#pragma once
#include "Resource.h"

namespace Engine
{
	class Mesh;
	class Skeleton;
	class Animation;
	class Texture;
	class Model : public Resource
	{
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
	public:
		explicit Model() = default;
		virtual ~Model();

	public:
		Skeleton* GetSkeleton() const { return _pSkeleton; }

		// Resource을(를) 통해 상속됨
		HRESULT LoadResource(const std::filesystem::path& filePath) override;

	private:
		void LoadNode(aiNode* paiNode, const aiScene* paiScene, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfo, std::vector<unsigned int>& materialIndex);
		void LoadMesh(aiNode* paiNode, aiMesh* paiMesh, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfo, std::vector<unsigned int>& materialIndex);
		void LoadMaterials(const aiScene* paiScene, const std::filesystem::path& filePath, std::vector<std::vector<std::shared_ptr<Texture>>>& textures);
		void FindMissingBone(aiNode* paiNode, std::unordered_map<std::string, std::pair<unsigned int, Matrix>>& boneInfo);
		std::shared_ptr<Texture> LoadTexture(const std::filesystem::path& rootPath, const aiScene* paiScene, aiString& filePath, aiMaterial* pMaterial, aiTextureType type, const wchar_t* format);

	private:
		void Free() override;

	private:
		std::vector<Mesh*>			_meshs;
		std::shared_ptr<Animation>	_animation;
		Skeleton*					_pSkeleton = nullptr;
		unsigned int				_boneCount = 0;
	};
}