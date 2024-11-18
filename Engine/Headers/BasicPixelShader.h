#pragma once
#include "Shader.h"

namespace Engine
{
	class BasicPixelShader : public Shader
	{
	public:
		explicit BasicPixelShader(const wchar_t* name);
	private:
		virtual ~BasicPixelShader() = default;

	public:
		void Awake() override;

	private:
		// Shader��(��) ���� ��ӵ�
		void Free() override;

	private:
		ID3D11PixelShader* _pShader = nullptr;

	};
}