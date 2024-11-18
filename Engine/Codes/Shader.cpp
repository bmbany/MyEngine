#include "Shader.h"

using namespace Engine;

class IncludeHandler : public ID3DInclude
{
public:
	// ���̴� ������ �����ϴ� �⺻ ���丮
	std::wstring shaderDir;

	IncludeHandler(const std::wstring& dir) : shaderDir(dir) {}

	HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override
	{
		// Include ���� ��� ����
		std::wstring fullPath = shaderDir + std::wstring(pFileName, pFileName + strlen(pFileName));

		// ������ ���̳ʸ� ���� ����
		std::ifstream file(fullPath, std::ios::binary);
		if (!file.is_open())
		{
			return E_FAIL; // ������ ã�� �� ���� ���
		}

		// ���� ������ �޸𸮷� �о���̱�
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		char* data = new char[size];
		file.read(data, size);
		file.close();

		*ppData = data;      // ������ ������ ����
		*pBytes = (UINT)size; // ���� ũ�� ����

		return S_OK;
	}

	HRESULT __stdcall Close(LPCVOID pData) override
	{
		delete[] reinterpret_cast<const char*>(pData); // �޸� ����
		return S_OK;
	}
};

void Engine::Shader::CompileShader(std::vector<uint8_t>& output, const std::filesystem::path& filePath, const char* entry, const char* model)
{
	std::vector<std::string> buffers;
	
	wchar_t path[256] = L"";
	lstrcat(path, L"../Engine/Shaders/");
	lstrcat(path, filePath.filename().c_str());

	int first_index = 0;
	int length = lstrlen(path);

	while ('_' != path[first_index++])
	{
		if (first_index >= length)
			break;
	}

	std::vector<D3D_SHADER_MACRO> macros;
	int pathIndex = first_index;

	if (first_index < length)
	{
		while ('.' != path[pathIndex])
		{
			char buffer[256] = "";
			int macroIndex = 0;
			while ('_' != path[pathIndex] && '.' != path[pathIndex])
			{
				buffer[macroIndex] = path[pathIndex];
				macroIndex++;
				pathIndex++;

				if ('_' == path[pathIndex])
				{
					pathIndex++;
					break;
				}
			}

			buffers.push_back(buffer);
		}

		for (auto& buffer : buffers)
		{
			macros.push_back({ buffer.c_str(), "" });
		}

		macros.emplace_back(nullptr, nullptr);
	}
	else
	{
		pathIndex = first_index;
		while ('.' != path[pathIndex--]) {}
		first_index = pathIndex + 2;
	}

	lstrcpy(&path[first_index - 1], L".hlsl");

	ID3DBlob* pShaderBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	
	IncludeHandler includeHandler(L"../Engine/Shaders/");

	D3DCompileFromFile(path, macros.data(), &includeHandler, entry, model, D3DCOMPILE_ENABLE_STRICTNESS, 0, &pShaderBlob, &pErrorBlob);

	output.resize(pShaderBlob->GetBufferSize());
	memcpy(output.data(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize());

	std::ofstream fout;
	fout.open(filePath, std::ios::out | std::ios::binary);

	if (fout.is_open())
	{
		fout.write((const char*)output.data(), output.size());
		fout.close();
	}

	SafeRelease(pShaderBlob);
	SafeRelease(pErrorBlob);
}
