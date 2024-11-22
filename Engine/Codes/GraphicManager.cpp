#include "GraphicManager.h"
#include <dxgi1_4.h>
#pragma comment(lib,"dxgi.lib")

using namespace Engine;

HRESULT Engine::GraphicManager::SetUpGraphic(HWND hWnd, unsigned int width, unsigned int height, bool isFullScreen)
{
	// 스왑체인 속성 설정 구조체 생성.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;							// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = !isFullScreen;						// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;

	// 화면 주사율 설정.
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 샘플링 관련 설정.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
	FAILED_CHECK_RETURN(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, nullptr, 0,
													  D3D11_SDK_VERSION, &swapDesc, &_pSwapChain, &_pDevice, nullptr, &_pDeviceContext)
													  , L"message");

	// 4. 렌더타겟뷰 생성.  (백버퍼를 이용하는 렌더타겟뷰)	
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	FAILED_CHECK_RETURN(_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture), L"message");
	FAILED_CHECK_RETURN(_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &_renderTargetViews[0]), L"message");

	ID3D11Texture2D* pRenderTargetTexture = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc
	{
		.Width = width, // 렌더링 영역 너비
		.Height = height, // 렌더링 영역 높이
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM, // RGBA 포맷
		.SampleDesc {.Count = 1 },
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	};

	FAILED_CHECK_RETURN(_pDevice->CreateTexture2D(&textureDesc, nullptr, &pRenderTargetTexture), L"message");
	FAILED_CHECK_RETURN(_pDevice->CreateRenderTargetView(pRenderTargetTexture, nullptr, &_renderTargetViews[1]), L"message");
	FAILED_CHECK_RETURN(_pDevice->CreateShaderResourceView(pRenderTargetTexture, nullptr, &_pRenderTargetTexture), L"message");

	SafeRelease(pBackBufferTexture);	//외부 참조 카운트를 감소시킨다.
	SafeRelease(pRenderTargetTexture);

	// 5. 뷰포트 설정.	
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// 뷰포트 설정.
	_pDeviceContext->RSSetViewports(1, &viewport);

	//6. 뎊스&스텐실 뷰 생성
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
	FAILED_CHECK_RETURN(_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil), L"message");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	FAILED_CHECK_RETURN(_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, &_pDepthStencilView), L"message");

	SafeRelease(textureDepthStencil);	

	// 7. 블렌딩
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

	// 종횡비 설정
	_aspectRatio = (float)width / (float)height;

	FAILED_CHECK_RETURN(CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&_pDXGIFactory), L"message");
	FAILED_CHECK_RETURN(_pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&_pDXGIAdapter)), L"message");

	return S_OK;
}

void Engine::GraphicManager::SetRenderTargetView(unsigned int index)
{
	float color[4]{ 0.5f, 0.5f, 0.5f, 1.f };
	_pDeviceContext->OMSetRenderTargets(1, &_renderTargetViews[index], _pDepthStencilView);
	_pDeviceContext->ClearRenderTargetView(_renderTargetViews[index], color);
	_pDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Engine::GraphicManager::BeginDraw()
{
	float color[4]{ 0.5f, 0.5f, 0.5f, 1.f };
	_pDeviceContext->OMSetRenderTargets(1, &_renderTargetViews[0], _pDepthStencilView);
	_pDeviceContext->ClearRenderTargetView(_renderTargetViews[0], color);
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
	for (auto& renderTargetView : _renderTargetViews)
		SafeRelease(renderTargetView);

	SafeRelease(_pDepthStencilView);
	SafeRelease(_pSwapChain);
	SafeRelease(_pRenderTargetTexture);
	SafeRelease(_pDeviceContext);
	SafeRelease(_pDevice);
}
