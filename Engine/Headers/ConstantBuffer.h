#pragma once
#include "Base.h"

namespace Engine
{
    class ConstantBuffer : public Base, public Singleton<ConstantBuffer>
    {
        friend class Singleton;

        struct Slot
        {
            ID3D11Buffer* pBuffer;
            unsigned int size;
        };
    private:
        explicit ConstantBuffer() = default;
        virtual ~ConstantBuffer() = default;

    public:
        void Intialize();
        void AddConstantBuffer(const std::filesystem::path& name, const unsigned int byteWidth);
        void SetConstantBuffer(const wchar_t* name, ShaderType type, unsigned int start = 0, unsigned int numBuffers = 1);
        void UpdateConstantBuffer(const wchar_t* name, const void* data);

    private:
        // DXBase을(를) 통해 상속됨
        void Free() override;

    private:
        std::unordered_map<std::wstring, Slot>  _constantBuffers;
        ID3D11DeviceContext*    _pDeviceContext = nullptr;
        ID3D11Device*           _pDevice = nullptr;
    };
}