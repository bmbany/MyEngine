#include "BasicPixelShader.h"

Engine::BasicPixelShader::BasicPixelShader(const wchar_t* name)
	: Shader(name)
{
	SetName(name);
}

void Engine::BasicPixelShader::Awake()
{
	HRESULT hr = __super::CreatePixelShader(L"../Shaders/BasicPixelShader.hlsl", "main", "ps_4_0", &_pShader);
	FAILED_CHECK_THROW(hr, L"CreatePixelShader Failed");
}

void Engine::BasicPixelShader::Free()
{
	SafeRelease(_pShader);
}
