#include "Sampler.h"
#include "GraphicManager.h"

Engine::Sampler::~Sampler()
{
	Free();
}

HRESULT Engine::Sampler::LoadResource(const std::filesystem::path& filePath)
{
	D3D11_SAMPLER_DESC sampDesc
	{
		.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		.AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
		.AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
		.ComparisonFunc = D3D11_COMPARISON_NEVER,
		.MinLOD = 0,
		.MaxLOD = D3D11_FLOAT32_MAX
	};

	ID3D11Device* pDevice = g_pGraphicMgr->GetDevice();

    FAILED_CHECK_RETURN(pDevice->CreateSamplerState(&sampDesc, &_pSampler), L"message");

	return S_OK;
}

void Engine::Sampler::Free()
{
	SafeRelease(_pSampler);
}
