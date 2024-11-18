#include "GraphicManager.h"
#include <dxgi1_4.h>
#pragma comment(lib,"dxgi.lib")

using namespace Engine;

bool Engine::GraphicManager::SetUpGraphic(HWND hWnd, unsigned int width, unsigned int height, bool isFullScreen)
{
	// �����.
	HRESULT hr = S_OK;

	// ����ü�� �Ӽ� ���� ����ü ����.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;							// ����ü�� ����� â �ڵ� ��.
	swapDesc.Windowed = !isFullScreen;						// â ��� ���� ����.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �����(�ؽ�ó)�� ����/���� ũ�� ����.
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;

	// ȭ�� �ֻ��� ����.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ���ø� ���� ����.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 1. ��ġ ����.   2.����ü�� ����. 3.��ġ ���ؽ�Ʈ ����.
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0,
		D3D11_SDK_VERSION, &swapDesc, &_pSwapChain, &_pDevice, nullptr, &_pDeviceContext);

	if (FAILED(hr))
		return false;

	// 4. ����Ÿ�ٺ� ����.  (����۸� �̿��ϴ� ����Ÿ�ٺ�)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
	if (FAILED(hr))
		return false;

	hr = _pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &_pRenderTargetView);  // �ؽ�ó�� ���� ���� ����
	if (FAILED(hr))
		return false;

	SafeRelease(pBackBufferTexture);	//�ܺ� ���� ī��Ʈ�� ���ҽ�Ų��.	

	// 5. ����Ʈ ����.	
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// ����Ʈ ����.
	_pDeviceContext->RSSetViewports(1, &viewport);

	//6. �X��&���ٽ� �� ����
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	hr = _pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil);
	if (FAILED(hr))
		return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = _pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &_pDepthStencilView);
	if (FAILED(hr))
		return false;

	SafeRelease(textureDepthStencil);	

	// 7. ����
	D3D11_BLEND_DESC blendDesc
	{
		.RenderTarget
		{
			{
				.BlendEnable = TRUE,
				.SrcBlend = D3D11_BLEND_SRC_ALPHA,
				.DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
				.BlendOp = D3D11_BLEND_OP_ADD,
				.SrcBlendAlpha = D3D11_BLEND_ONE,
				.DestBlendAlpha = D3D11_BLEND_ZERO,
				.BlendOpAlpha = D3D11_BLEND_OP_ADD,
				.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
			}
		}
	};

	ID3D11BlendState* pBlendState = nullptr;
	FAILED_CHECK_THROW(_pDevice->CreateBlendState(&blendDesc, &pBlendState), L"message");

	float blendFactor[4]{ 0.f, 0.f, 0.f };
	_pDeviceContext->OMSetBlendState(pBlendState, blendFactor, 0xFFFFFFFF);

	SafeRelease(pBlendState);

	// ��Ⱦ�� ����
	_aspectRatio = (float)width / (float)height;


	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&_pDXGIFactory)))
		return false;

	if (FAILED(_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&_pDXGIAdapter))))
		return false;

	return true;
}

void Engine::GraphicManager::BeginDraw()
{
	float color[4]{ 0.5f, 0.5f, 0.5f, 1.f };	
	_pDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pDeviceContext->ClearRenderTargetView(_pRenderTargetView, color);
	_pDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Engine::GraphicManager::EndDraw()
{
	_pSwapChain->Present(0, 0);
}

std::pair<size_t, size_t> Engine::GraphicManager::GetUsedVRAM()
{
	DXGI_QUERY_VIDEO_MEMORY_INFO local, nonLocal;
	_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &local);
	_pDXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &nonLocal);

	return { local.CurrentUsage / 1024 / 1024, nonLocal.CurrentUsage / 1024 / 1024 };
}

void Engine::GraphicManager::Free()
{	
	SafeRelease(_pDepthStencilView);
	SafeRelease(_pRenderTargetView);
	SafeRelease(_pSwapChain);
	SafeRelease(_pDeviceContext);
	SafeRelease(_pDevice);
}
