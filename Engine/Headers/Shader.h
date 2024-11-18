#pragma once
#include "Resource.h"

namespace Engine
{
	class Shader : public Resource
	{
	protected:
		explicit Shader() = default;
		virtual ~Shader() = default;

	protected:
		void CompileShader(std::vector<uint8_t>& output, const std::filesystem::path& filePath, const char* entry, const char* model);
	};
}