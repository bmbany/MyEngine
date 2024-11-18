#include "Material.h"
#include "ResourceManager.h"

#include "Texture.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Engine;

Engine::Material::Material()
{
	_textureMaps.resize((size_t)TextureType::End);
}

void Engine::Material::SetParameters()
{
	for (size_t i = 0; i < _textureMaps.size(); i++)
	{
		ID3D11ShaderResourceView* pTexture = nullptr; 
		
		if (_textureMaps[i].get())
			pTexture = _textureMaps[i]->Get();

		_pixelShader->SetShaderResource((TextureType)i, pTexture);
	}

	_vertexShader->SetVertexShader();
	_pixelShader->SetPixelShader();
}

void Engine::Material::BindVertexShader(const wchar_t* vertexShader)
{
	_vertexShader = g_pResourceMgr->LoadResource<VertexShader>(vertexShader);
	assert(_vertexShader.get());
}

void Engine::Material::BindPixelShader(const wchar_t* pixelShader)
{
	_pixelShader = g_pResourceMgr->LoadResource<PixelShader>(pixelShader);
	assert(_pixelShader.get());
}

void Engine::Material::BindTextureMap(TextureType type, const std::shared_ptr<Texture>& texture)
{
	_textureMaps[(int)type] = texture;
}

void Engine::Material::Free()
{
}