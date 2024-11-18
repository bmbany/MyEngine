#include "Mesh.h"
#include "VIBuffer.h"
#include "Material.h"

Engine::Mesh::Mesh(std::vector<MeshVertex>& vertices, std::vector<unsigned int>& indices)
{	
	_pVIBuffer = new VIBuffer;

	VIBuffer::VIBufferInfo info
	{
		.vertices = vertices.data(),
		.indices = indices.data(),
		.vertexByteWidth = (unsigned int)vertices.size() * sizeof(MeshVertex),
		.vertexBufferStride = sizeof(MeshVertex),
		.vertexBufferOffset = 0,
		.indexByteWidth = sizeof(unsigned int) * (unsigned int)indices.size(),
		.indexCount = (unsigned int)indices.size()
	};

	_pVIBuffer->Initialize(info);
}

void Engine::Mesh::Free()
{
	SafeRelease(_pVIBuffer);
	SafeRelease(_pMaterial);
}
