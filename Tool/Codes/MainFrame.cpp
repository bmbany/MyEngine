#include "MainFrame.h"
#include "../../Engine/Headers/Engine.h"
#include "../../Engine/Headers/GraphicManager.h"

#include "MainCamera.h"
#include "Inspector.h"
#include "Assets.h"
#include "Hierarchy.h"

// imgui
#undef new
#include "../../Reference/inc/imgui.h"

constexpr float InspectorWidth = 500.f;

int MainFrame::Update(const float deltaTime)
{
    return 0;
}

int MainFrame::LateUpdate(const float deltaTime)
{
    return 0;
}

void MainFrame::Game()
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        // CameraMove
        _pMainCamera->SetCanMove(true);
    }
    else
    {
        _pMainCamera->SetCanMove(false);
    }
}

bool MainFrame::Initialize()
{
    _pMainCamera = new MainCamera;
    Camera::AddCamera(L"MainCamera", _pMainCamera);
    Camera::SetCurrentCamera(L"MainCamera");
    
    _pInpector = new Inspector;
    _pAssets = new Assets;
    _pHierarchy = new Hierarchy;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

    RECT rect;
    GetClientRect(Engine::GetWindow(), &rect);

    g_pImGUIMgr->AddFunction([this, rect]()
        {
            ImGui::SetNextWindowPos(ImVec2((float)rect.left, (float)rect.top));
            ImGui::SetNextWindowSize(ImVec2(float(rect.right - rect.left), float(rect.bottom - rect.top)));
            
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("MainDockSpace", nullptr, ImGuiWindowFlags_NoTitleBar |
                                                   ImGuiWindowFlags_NoCollapse |
                                                   ImGuiWindowFlags_NoResize |
                                                   ImGuiWindowFlags_NoMove | 
                                                   ImGuiWindowFlags_NoBringToFrontOnFocus);

                // DockSpace »ý¼º
                ImGui::DockSpace(ImGui::GetID("MainDockSpace"));
            ImGui::End();

            ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((unsigned long long)g_pGraphicMgr->GetRenderTargetTexture(), ImVec2(1920.f, 1080.f));
                Game();
            ImGui::End();
            ImGui::PopStyleVar();
            
            ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);           
                _pInpector->DrawGUI();
            ImGui::End();

            ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);
                _pHierarchy->DrawGUI();
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 10));
            ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoCollapse);
                _pAssets->DrawGUI();
            ImGui::End();
            ImGui::PopStyleVar();
        });

    return true;
}

void MainFrame::Free()
{
    Engine::SafeRelease(_pInpector);
    Engine::SafeRelease(_pHierarchy);
    Engine::SafeRelease(_pAssets);
}