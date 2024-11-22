#include "../../Engine/Headers/Engine.h"
#include "MainFrame.h"

#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

void CheckMemoryLeaks()
{
	_CrtDumpMemoryLeaks();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Engine::GameManager* pGameManager = Engine::GameManager::GetInstance();
	
	Engine::GameManager::GameDefaultSetting info
	{
		.hInstance = hInstance,
		.appName = L"MyD3D",
		.renderGroupSize = 1,
		.maxSoundGroup = 1,
		.layerSize = 1,
		.width = 1920,
		.height = 1080,
		.fiexedCount = 50,
		.isFullScreen = false
	};

	if (pGameManager->Initialize(info))
	{
		pGameManager->ChangeScene(new MainFrame);
		pGameManager->Run();
	}

	Engine::SafeRelease(pGameManager);

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

	return EXIT_SUCCESS;
}