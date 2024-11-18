#include "BasicVertexShader.h"

using namespace Engine;

Engine::BasicVertexShader::BasicVertexShader(const wchar_t* name)
	: Shader(name)
{
	SetName(name);
}

void Engine::BasicVertexShader::Awake()
{
	HRESULT hr = __super::CreateVertexShader(L"../Shaders/BasicVertexShader.hlsl", "main", "vs_4_0", &_pShader);
	FAILED_CHECK_THROW(hr, L"CreateVertexShader Failed");

}

void Engine::BasicVertexShader::Free()
{
	SafeRelease(_pShader);
}
