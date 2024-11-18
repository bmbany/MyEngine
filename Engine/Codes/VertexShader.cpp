#include "VertexShader.h"
#include "GraphicManager.h"
#include "ConstantBuffer.h"

Engine::VertexShader::~VertexShader()
{
	Free();
}

void Engine::VertexShader::SetVertexShader()
{	
	_pDeviceContext->IASetInputLayout(_pInputLayout);
	_pDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);	

	for (auto& slot : _inputBindSlot)
	{
		g_pConstantBuffer->SetConstantBuffer(slot.name.c_str(), ShaderType::Vertex, slot.start, slot.count);
	}
}

HRESULT Engine::VertexShader::LoadResource(const std::filesystem::path& filePath)
{
	ID3D11Device* pDevice = g_pGraphicMgr->GetDevice();
	_pDeviceContext = g_pGraphicMgr->GetDeviceContext();

	// 컴파일된 cso 파일로부터 쉐이더 정보를 불러옴
	std::vector<uint8_t> vertexShaderData = ReadData(filePath.c_str());
	
	if (vertexShaderData.empty())
	{
		CompileShader(vertexShaderData, filePath, "main", "vs_5_0");
	}

	// 로드한 쉐이더 파일의 input semantic 을 읽어서 인풋 레이아웃 자동 생성
	ID3D11ShaderReflection* pReflector = nullptr;
	D3DReflect(vertexShaderData.data(), vertexShaderData.size(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	for (unsigned int i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputBindDesc{};
		pReflector->GetResourceBindingDesc(i, &inputBindDesc);

		if (D3D_SIT_CBUFFER == inputBindDesc.Type)
		{
			ID3D11ShaderReflectionConstantBuffer* pConstantBuffer = pReflector->GetConstantBufferByName(inputBindDesc.Name);
			D3D11_SHADER_BUFFER_DESC bufferDesc{};
			pConstantBuffer->GetDesc(&bufferDesc);
			g_pConstantBuffer->AddConstantBuffer(std::filesystem::path(bufferDesc.Name).c_str(), bufferDesc.Size);
			_inputBindSlot.emplace_back(std::filesystem::path(inputBindDesc.Name).c_str(), inputBindDesc.BindPoint, inputBindDesc.BindCount);
		}
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	inputLayoutDesc.resize(shaderDesc.InputParameters);

	for (unsigned int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pReflector->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc
		{
			.SemanticName = paramDesc.SemanticName,
			.SemanticIndex = paramDesc.SemanticIndex,
			.InputSlot = 0,
			.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
			.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
			.InstanceDataStepRate = 0
		};

		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
		{
			if (paramDesc.Mask == 1) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			else if (paramDesc.Mask <= 3) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			else if (paramDesc.Mask <= 7) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			else if (paramDesc.Mask <= 15) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
		{
			if (paramDesc.Mask == 1) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.Mask <= 3) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.Mask <= 7) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.Mask <= 15) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		}
		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
		{
			if (paramDesc.Mask == 1) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.Mask <= 3) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.Mask <= 7) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.Mask <= 15) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}

		inputLayoutDesc[i] = elementDesc;
	}

	FAILED_CHECK_RETURN(pDevice->CreateVertexShader(
		vertexShaderData.data(),
		vertexShaderData.size(),
		nullptr,
		&_pVertexShader),
		L"message");

	FAILED_CHECK_RETURN(pDevice->CreateInputLayout(
		inputLayoutDesc.data(),
		(unsigned int)inputLayoutDesc.size(),
		vertexShaderData.data(),
		vertexShaderData.size(),
		&_pInputLayout)
		, L"message");

	SafeRelease(pReflector);

	return S_OK;
}

void Engine::VertexShader::Free()
{
	SafeRelease(_pVertexShader);
	SafeRelease(_pInputLayout);
}