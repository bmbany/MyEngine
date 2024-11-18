#pragma once
#include "Base.h"

namespace Engine
{
	class VertexShader;
	class PixelShader;
	class Texture;
	class Sampler;
	class Material : public Base
	{
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;

	public:
		explicit Material();
		virtual ~Material() = default;

	public:
		void SetParameters();

	public:
		void BindVertexShader(const wchar_t* vertexShader);
		void BindPixelShader(const wchar_t* pixelShader);
		void BindTextureMap(TextureType type, const std::shared_ptr<Texture>& texture);

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		std::vector<std::shared_ptr<Texture>>	_textureMaps;
		std::shared_ptr<VertexShader>			_vertexShader;
		std::shared_ptr<PixelShader>			_pixelShader;
	};
}