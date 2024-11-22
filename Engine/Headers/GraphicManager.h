#pragma once
#include "Base.h"

struct IDXGIFactory4;
struct IDXGIAdapter3;

namespace Engine
{
	class Factorys;
	class Shader;
	class GraphicManager : public Base, public Singleton<GraphicManager>
	{
		friend class Singleton;
	private:
		explicit GraphicManager() = default;
		virtual ~GraphicManager() = default;

	public:
		ID3D11DeviceContext* GetDeviceContext() const { return _pDeviceContext; }
		ID3D11Device* GetDevice() const { return _pDevice; }
		ID3D11ShaderResourceView* GetRenderTargetTexture() const { return _pRenderTargetTexture; }
		float GetAspectRatio() const { return _aspectRatio; }

		HRESULT SetUpGraphic(HWND hWnd, unsigned int width, unsigned int height, bool isFullScreen);
		void SetRenderTargetView(unsigned int index);

	public:
		void BeginDraw();
		void EndDraw();
		std::pair<size_t, size_t> GetUsedVRAM();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		ID3D11Device*				_pDevice{ nullptr };
		ID3D11DeviceContext*		_pDeviceContext{ nullptr };
		IDXGISwapChain*				_pSwapChain{ nullptr };
		ID3D11RenderTargetView*		_renderTargetViews[8]{ nullptr, };
		ID3D11DepthStencilView*		_pDepthStencilView{ nullptr };
		ID3D11ShaderResourceView*	_pRenderTargetTexture{ nullptr };
		IDWriteFactory*				_pWriteFactory{ nullptr };
		
		IDXGIFactory4*				_pDXGIFactory{ nullptr };
		IDXGIAdapter3*				_pDXGIAdapter{ nullptr };
		float						_aspectRatio{ 0.f };
	};
}