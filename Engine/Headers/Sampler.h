#pragma once
#include "Resource.h"

namespace Engine
{
	class Sampler : public Resource
	{
	public:
		explicit Sampler() = default;
		virtual ~Sampler();

	public:
		ID3D11SamplerState* Get() const { return _pSampler; }

	public:
		// Resource을(를) 통해 상속됨
		HRESULT LoadResource(const std::filesystem::path& filePath) override;

	private:
		void Free() override;

	private:
		ID3D11SamplerState* _pSampler = nullptr;
	};
}