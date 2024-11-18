#include "ConstantBuffer.h"
#include "GraphicManager.h"

using namespace Engine;

void Engine::ConstantBuffer::Intialize()
{
	_pDevice = g_pGraphicMgr->GetDevice();
	_pDeviceContext = g_pGraphicMgr->GetDeviceContext();
}

void Engine::ConstantBuffer::AddConstantBuffer(const std::filesystem::path& name, const unsigned int byteWidth)
{
	const wchar_t* tag = name.c_str();

	if (_constantBuffers.find(tag) == _constantBuffers.end())
	{
		D3D11_BUFFER_DESC constantBuffer
		{
			.ByteWidth = byteWidth,
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE
		};

		ID3D11Buffer* pConstantBuffer = nullptr;
		FAILED_CHECK_THROW(_pDevice->CreateBuffer(&constantBuffer, nullptr, &pConstantBuffer), L"message");

		_constantBuffers[tag] = { pConstantBuffer, byteWidth };
	}
}

void Engine::ConstantBuffer::SetConstantBuffer(const wchar_t* name, ShaderType type, unsigned int start, unsigned int numBuffers)
{
	switch (type)
	{
	case ShaderType::Vertex:
		_pDeviceContext->VSSetConstantBuffers(start, numBuffers, &_constantBuffers[name].pBuffer);
		break;
	case ShaderType::Pixel:
		_pDeviceContext->PSSetConstantBuffers(start, numBuffers, &_constantBuffers[name].pBuffer);
		break;
	}
}

void Engine::ConstantBuffer::UpdateConstantBuffer(const wchar_t* name, const void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	Slot& slot = _constantBuffers[name];

	HRESULT hr = _pDeviceContext->Map(slot.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, slot.size);
	_pDeviceContext->Unmap(slot.pBuffer, 0);
}

void Engine::ConstantBuffer::Free()
{
	for (auto& pair : _constantBuffers)
		SafeRelease(pair.second.pBuffer);
}
