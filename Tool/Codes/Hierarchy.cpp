#include "Hierarchy.h"
#include "../../Engine/Headers/Engine.h"

// imgui
#undef new
#include "../../Reference/inc/imgui.h"

Hierarchy::Hierarchy()
{
    _selectedObject = _objects.end();
}

void Hierarchy::DrawGUI()
{
    for (auto iter = _objects.begin(); iter != _objects.end(); iter++)
    {
        auto& [isSelected, gameObject] = iter->second;
        std::filesystem::path name = gameObject->GetName();

        if (ImGui::Selectable(name.string().c_str(), &isSelected))
        {
            if (_selectedObject != _objects.end())
                _selectedObject->second.first = false;

            _selectedObject = iter;
        }
    }

    if (!_isRename && ImGui::IsKeyPressed(ImGuiKey_F2))
    {
        if (_selectedObject != _objects.end())
        {
            memcpy(_renameBuffer, _selectedObject->first.c_str(), _selectedObject->first.length());
            _isRename = true;
        }
    }

    if (_isRename)
    {
        Rename();
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("HierarchyContentContextMenu");
    }

    // 팝업 컨텍스트 메뉴
    if (ImGui::BeginPopup("HierarchyContentContextMenu"))
    {
        if (ImGui::MenuItem("Create GameObject"))
        {
            AddObject();
        }

        if (ImGui::BeginMenu("Light"))
        {
            if (ImGui::MenuItem("Directional Light"))
            {
            }

            if (ImGui::MenuItem("Point Light"))
            {
            }

            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
}

void Hierarchy::Update()
{
    /*if (_selectedObject != _objects.end())
    {
        auto& [isSelected, gameObject] = _selectedObject->second;

        if (isSelected)
        {
            const std::string& name = _selectedObject->first;

        }
    }*/
}

void Hierarchy::AddObject()
{
    static unsigned int ID = 0;
    std::filesystem::path tag = L"GameObject (" + std::to_wstring(ID) + L")";

    auto iter = _objects.find(tag.string());
    if (iter == _objects.end())
    {
        Engine::GameObject* pGameObject = new Engine::GameObject(tag.c_str());

        _objects.emplace(tag.string().c_str(), std::make_pair(false, pGameObject));
        Engine::AddObjectInLayer(0, pGameObject);

        ID++;
    }
}

void Hierarchy::Rename()
{
    // 이름 변경 모드 활성화
    ImGui::OpenPopup("Rename");

    // 이름 변경 팝업
    auto SetRename = [this]()
        {
            if (_objects.end() != _objects.find(_renameBuffer))
            {
                _isRename = false;
                ImGui::CloseCurrentPopup();
                return;
            }

            auto& [isSelected, gameObject] = _selectedObject->second;

            gameObject->SetName(std::filesystem::path(_renameBuffer).c_str());
            _objects.emplace(_renameBuffer, std::make_pair(false, gameObject));
            _isRename = false;

            _objects.erase(_selectedObject);
            _selectedObject = _objects.end();

            ImGui::CloseCurrentPopup();
        };

    if (ImGui::BeginPopupModal("Rename", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::InputText("##Rename", _renameBuffer, MAXSIZE, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            // 엔터 입력 시 이름 변경 완료
            SetRename();
        }

        if (ImGui::Button("OK"))
        {
            SetRename();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            _isRename = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Hierarchy::Free()
{
}
