#pragma once
#include "Resource.h"

namespace Engine
{
    class Texture : public Resource
    {
    public:
        explicit Texture() = default;
        virtual ~Texture();

    public:
        HRESULT LoadResource(const std::filesystem::path& filePath) override;
        ID3D11ShaderResourceView* Get() const { return _pTexture; }

    private:
        // Resource��(��) ���� ��ӵ�
        void Free() override;

    private:
        ID3D11ShaderResourceView* _pTexture = nullptr;
    };
}