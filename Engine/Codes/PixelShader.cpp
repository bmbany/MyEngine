#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "GraphicManager.h"
#include "ResourceManager.h"
#include "Sampler.h"

using namespace Engine;

Engine::PixelShader::~PixelShader()
{
	Free();
}

void Engine::PixelShader::SetPixelShader()
{
	_pDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);

	for (auto& slot : _sharedBindSlot)
	{
		switch (slot.type)
		{
		case Type::CBuffer:
			g_pConstantBuffer->SetConstantBuffer(slot.name.c_str(), ShaderType::Pixel, slot.slot.start, slot.slot.count);
			break;
		case Type::Sampler:
			ID3D11SamplerState* pSampler = _sampler->Get();
			_pDeviceContext->PSSetSamplers(slot.slot.start, slot.slot.count, &pSampler);
			break;
		}
	}
}

void Engine::PixelShader::SetShaderResource(TextureType type, ID3D11ShaderResourceView* pShaderResourece)
{
	const auto& slot = _inputBindSlot[(int)type];
	_pDeviceContext->PSSetShaderResources(slot.start, slot.count, &pShaderResourece);
}

HRESULT Engine::PixelShader::LoadResource(const std::filesystem::path& filePath)
{
	ID3D11Device* pDevice = g_pGraphicMgr->GetDevice();
	_pDeviceContext = g_pGraphicMgr->GetDeviceContext();

	// 컴파일된 cso 파일로부터 쉐이더 정보를 불러옴
	std::vector<uint8_t> pixelShaderData = ReadData(filePath.c_str());

	if (pixelShaderData.empty())
	{
		CompileShader(pixelShaderData, filePath, "main", "ps_5_0");
	}

	ID3D11ShaderReflection* pReflector{ nullptr };
	D3DReflect(pixelShaderData.data(), pixelShaderData.size(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc{};
	pReflector->GetDesc(&shaderDesc);

	_inputBindSlot.resize((int)TextureType::End);

	for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputBindDesc{};
		pReflector->GetResourceBindingDesc(i, &inputBindDesc);

		std::string resourceName = inputBindDesc.Name;

		std::transform(resourceName.begin(), resourceName.end(), resourceName.begin(),
			[](unsigned char c) { return std::tolower(c); });

		switch (inputBindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			if (resourceName.find("diff") != std::string::npos)
			{
				_inputBindSlot[(int)TextureType::Diffuse] = { inputBindDesc.BindPoint, inputBindDesc.BindCount};
			}
			else if (resourceName.find("norm") != std::string::npos)
			{
				_inputBindSlot[(int)TextureType::Normal] = { inputBindDesc.BindPoint, inputBindDesc.BindCount };
			}
			else if (resourceName.find("spec") != std::string::npos)
			{
				_inputBindSlot[(int)TextureType::Specular] = { inputBindDesc.BindPoint, inputBindDesc.BindCount };
			}
			else if (resourceName.find("emis") != std::string::npos)
			{
				_inputBindSlot[(int)TextureType::Emission] = { inputBindDesc.BindPoint, inputBindDesc.BindCount };
			}
			else if (resourceName.find("light") != std::string::npos)
			{
				_inputBindSlot[(int)TextureType::LightMap] = { inputBindDesc.BindPoint, inputBindDesc.BindCount };
			}
			break;
		case D3D_SIT_SAMPLER:
			if (resourceName.find("linear") != std::string::npos)
			{
				std::filesystem::path name(inputBindDesc.Name);
				_sampler = g_pResourceMgr->LoadResource<Sampler>(name);
				_sharedBindSlot.push_back({ name.c_str(), {inputBindDesc.BindPoint, inputBindDesc.BindCount}, Type::Sampler });
			}
			break;
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* pConstantBuffer = pReflector->GetConstantBufferByName(inputBindDesc.Name);
			D3D11_SHADER_BUFFER_DESC bufferDesc{};
			pConstantBuffer->GetDesc(&bufferDesc);
			std::filesystem::path name(inputBindDesc.Name);
			g_pConstantBuffer->AddConstantBuffer(name.c_str(), bufferDesc.Size);
			_sharedBindSlot.push_back({ name.c_str(), { inputBindDesc.BindPoint, inputBindDesc.BindCount }, Type::CBuffer });
		}
			break;
		}
	}

	FAILED_CHECK_RETURN(pDevice->CreatePixelShader(
		pixelShaderData.data(),
		pixelShaderData.size(),
		nullptr,
		&_pPixelShader),
		L"message");

	SafeRelease(pReflector);

	return S_OK;
}

void Engine::PixelShader::Free()
{
	SafeRelease(_pPixelShader);
}