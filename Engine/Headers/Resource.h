#pragma once
#include "Base.h"

namespace Engine
{
	class Resource abstract : public Base
	{
	protected:
		explicit Resource() = default;
		virtual ~Resource() = default;

	public:
		virtual HRESULT LoadResource(const std::filesystem::path& filePath) = 0;
		virtual void Free() = 0;
	};
}