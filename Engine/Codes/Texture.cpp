#include "Texture.h"
#include "GraphicManager.h"
#include <DirectXTex.h>

using namespace Engine;

Engine::Texture::~Texture()
{
    Free();
}

HRESULT Engine::Texture::LoadResource(const std::filesystem::path& filePath)
{
    ID3D11Device* pDevice = g_pGraphicMgr->GetDevice();
    std::filesystem::path file = filePath;

    std::wstring extension = file.extension().wstring();
    DirectX::ScratchImage image;
    DirectX::TexMetadata metadata;

    if (L".dds" == extension)
    {
        FAILED_CHECK_RETURN(DirectX::LoadFromDDSFile(filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, image), L"message");
    }
    else if (L".tga" == extension)
    {
        FAILED_CHECK_RETURN(DirectX::LoadFromTGAFile(filePath.c_str(), &metadata, image), L"message");
    }
    else
    {
        FAILED_CHECK_RETURN(DirectX::LoadFromWICFile(filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image), L"message");
    }

    FAILED_CHECK_RETURN(DirectX::CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &_pTexture), L"message");

    return S_OK;
}

void Engine::Texture::Free()
{
    SafeRelease(_pTexture);
}
