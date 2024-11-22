#include "ImGUIManager.h"
#include "GraphicManager.h"

#ifdef _DEBUG
#undef new
#endif
#include "../../Reference/inc/imgui.h"
#include "../../Reference/inc/imgui_impl_win32.h"
#include "../../Reference/inc/imgui_impl_dx11.h"

void Engine::ImGUIManager::Initialize(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(g_pGraphicMgr->GetDevice(), g_pGraphicMgr->GetDeviceContext());	
}

void Engine::ImGUIManager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& function : _functions)
		function();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// ¸ÖÆ¼ ºäÆ÷Æ® ·»´õ¸µ
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Engine::ImGUIManager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}