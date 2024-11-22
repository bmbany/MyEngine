#include "GameManager.h"
#include "Camera.h"
#include "EventInvoker.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Scene.h"
#include "WinApp.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

// Manager
#include "GraphicManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "TimeManager.h"
#include "CameraManager.h"
#include "ImGUIManager.h"

using namespace Engine;

GraphicManager* g_pGraphicMgr{ nullptr };
ResourceManager* g_pResourceMgr{ nullptr };
ConstantBuffer* g_pConstantBuffer{ nullptr };
ImGUIManager* g_pImGUIMgr{ nullptr };

Engine::GameManager::GameManager()
{
    g_pGraphicMgr = GraphicManager::GetInstance();
    g_pResourceMgr = ResourceManager::GetInstance();
    g_pConstantBuffer = ConstantBuffer::GetInstance();
    g_pImGUIMgr = ImGUIManager::GetInstance();

    _pInputMgr = InputManager::GetInstance();
    _pTimeMgr = TimeManager::GetInstance();
    _pSoundMgr = SoundManager::GetInstance();
    _pCameraMgr = CameraManager::GetInstance();
    _pSceneMgr = SceneManager::Create();
    _pRenderer = Renderer::Create();
    _pEventInvoker = EventInvoker::Create();
}

void Engine::GameManager::Run()
{
    MSG msg;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;
            else
            {
                DispatchMessage(&msg);
            }
        }
        else
        {
            StartGame();
            FixedUpdateGame();
            UpdateGame();
            LateUpdateGame();
            RenderGame();
        }
    }
}

void Engine::GameManager::StartGame()
{
    _pSceneMgr->Start();
}

void Engine::GameManager::FixedUpdateGame()
{
    _elapsed += _pTimeMgr->GetDeltaTime();
    
    if (_elapsed >= _fixed)
    {
        _pSceneMgr->FixedUpdate();
        _pCameraMgr->FixedUpdate();
        _elapsed -= _fixed;
    }
}

int Engine::GameManager::UpdateGame()
{
    _pTimeMgr->Update();
    float deltaTime = _pTimeMgr->GetDeltaTime();
    _pInputMgr->Update(deltaTime);
    _pEventInvoker->Update(deltaTime);
    _pSceneMgr->Update(deltaTime);
    _pCameraMgr->Update(deltaTime);

    return 0;
}

int Engine::GameManager::LateUpdateGame()
{    
    float deltaTime = _pTimeMgr->GetDeltaTime();    
    _pSceneMgr->LateUpdate(deltaTime);
    _pCameraMgr->LateUpdate(deltaTime);
    _pSoundMgr->Update(deltaTime);
    _pSceneMgr->AddRenderGroup();

    return 0;
}

void Engine::GameManager::RenderGame()
{
    _pRenderer->Render_GameObject();
}

bool Engine::GameManager::Initialize(const GameDefaultSetting& info)
{
    _pWinApp = WinApp::Create(info.hInstance, info.appName, info.width, info.height, info.isFullScreen);

    if (nullptr == _pWinApp)
        return false;
   
    if (E_FAIL == g_pGraphicMgr->SetUpGraphic(_pWinApp->GetWindow(), info.width, info.height, info.isFullScreen))
        return false;

    if (false == _pInputMgr->SetUpInputDevice(info.hInstance, _pWinApp->GetWindow()))
        return false;

    if (false == _pSoundMgr->SetUpSound(info.maxSoundGroup))
        return false;

    Renderer::RenderGroupInfo renderInfo
    {
        .pDeviceContext = g_pGraphicMgr->GetDeviceContext(),
        .pSwapChain = nullptr,
        .size = info.renderGroupSize,
        .width = info.width,
        .height = info.height
    };

    if (false == _pRenderer->SetUpRenderer(renderInfo))
        return false;

    _pSceneMgr->SetUpLayer(info.layerSize);
    g_pImGUIMgr->Initialize(_pWinApp->GetWindow());

    _fixed = 1.f / info.fiexedCount;
    g_pConstantBuffer->Intialize();

    return true;
}

HWND Engine::GameManager::GetWindow() const
{
    return _pWinApp->GetWindow();
}

void Engine::GameManager::SetSlowTime(float rate)
{
    _pTimeMgr->SetSlowTime(rate);
}

void Engine::GameManager::ChangeScene(Scene* pScene)
{
    _pSceneMgr->ChangeScene(pScene);
}

std::list<GameObject*>* Engine::GameManager::FindObjectList(const int layerGroup)
{
    return _pSceneMgr->FindObjectList(layerGroup);
}

GameObject* Engine::GameManager::FindObject(const int layerGroup, const wchar_t* objectTag)
{
    return _pSceneMgr->FindObject(layerGroup, objectTag);
}

void Engine::GameManager::Free()
{
    SafeRelease(_pWinApp);
    SafeRelease(_pEventInvoker);
    SafeRelease(_pRenderer);
    SafeRelease(_pSceneMgr);
    SafeRelease(_pInputMgr);
    SafeRelease(_pTimeMgr);
    SafeRelease(_pSoundMgr);
    SafeRelease(_pCameraMgr);
    SafeRelease(g_pImGUIMgr);
    SafeRelease(g_pResourceMgr);
    SafeRelease(g_pConstantBuffer);
    SafeRelease(g_pGraphicMgr);
}

bool Engine::GameManager::AddRenderGroup(const int renderGroup, GameObject* pObject)
{
    return _pRenderer->AddRenderGroup(renderGroup, pObject);
}

void Engine::GameManager::SetSortGroup(const int sortGroup, bool(*sortFunc)(GameObject*, GameObject*))
{
    _pRenderer->SetSortGroup(sortGroup, sortFunc);
}

EventInvoker* Engine::GameManager::GetEventInvoker() const
{
    return _pEventInvoker;
}

void Engine::GameManager::LoadSound(const char* filePath)
{
    _pSoundMgr->LoadSound(filePath);
}

void Engine::GameManager::ClearObjectList(const int layerGroup)
{
    _pSceneMgr->ClearObjectList(layerGroup);
    _pRenderer->ResetRenderGroup();
}

void Engine::GameManager::ClearLayer(const int layerGroup)
{
    _pSceneMgr->ClearLayer(layerGroup);
    _pRenderer->ResetRenderGroup();
}

void Engine::GameManager::AddObjectInLayer(const int layerGroup, GameObject* pObject)
{
    _pSceneMgr->AddObjectInLayer(layerGroup, pObject);
}
