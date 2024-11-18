#pragma once
#include "Shader.h"

namespace Engine
{
	class BasicVertexShader : public Shader
	{
	public:
		explicit BasicVertexShader(const wchar_t* name);
	private:
		virtual ~BasicVertexShader() = default;

	public:
		void Awake() override;

	private:
		// Shader을(를) 통해 상속됨
		void Free() override;

	private:
		ID3D11VertexShader* _pShader = nullptr;
	};
}
