#pragma once
#include "Base.h"

namespace Engine
{	
	struct MeshVertex
	{
		Vector4 position;
		Vector3 normal;
		Vector3 tangent;
		Vector3 biTangent;
		Vector2 texCoord[2];

		unsigned int blendIndices[4]{};
		float blendWeights[4]{};
	};

	class Material;
	class VIBuffer;
	class Mesh : public Base
	{
		friend class SkinnedMeshRenderer;
		friend class MeshRenderer;
	public:
		explicit Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices);
		virtual ~Mesh() = default;

	public:
		void BindMaterial(Material* pMaterial) { _pMaterial = pMaterial; }

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		VIBuffer*	_pVIBuffer{ nullptr };
		Material*	_pMaterial{ nullptr };
	};
}