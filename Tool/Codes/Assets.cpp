#include "Assets.h"
#include <commdlg.h>

// imgui
#undef new
#include "../../Reference/inc/imgui.h"

void Assets::DrawGUI()
{
    int itemsPerRow = 5;
    int itemIndex = 0;
    
    for (const auto& item : _items)
    {
        // �׸��� ���̾ƿ� ����
        if (itemIndex % itemsPerRow != 0)
            ImGui::SameLine();

        // �����ܰ� �ؽ�Ʈ ǥ��
        
        ImGui::BeginGroup();
        ID3D11ShaderResourceView* pTexture = nullptr;
        if (ImGui::ImageButton(item.c_str(), (unsigned long long)pTexture, ImVec2(64, 64)))
        {
            std::cout << "Clicked on: " << std::endl;
        }
        ImGui::TextWrapped("%s", item.c_str());
        ImGui::EndGroup();
        itemIndex++;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("AssetsContentContextMenu");
    }

    // �˾� ���ؽ�Ʈ �޴�
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 5.f));
    if (ImGui::BeginPopup("AssetsContentContextMenu"))
    {
        if (ImGui::MenuItem("Import New Asset..."))
        {
            std::filesystem::path filePath = OpenFileDialog();
            if (!filePath.empty())
            {
                AddItem(filePath.filename().string());
            }
        }
        
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void Assets::Update()
{
    
}

std::wstring Assets::OpenFileDialog()
{
    wchar_t filename[MAX_PATH] = L"";

    OPENFILENAME ofenFileName
    {
        .lStructSize = sizeof(OPENFILENAME),
        .hwndOwner = nullptr,
        .lpstrFilter = L"All Files\0*.*\0fbx Files\0*.fbx\0",
        .lpstrFile = filename,
        .nMaxFile = MAX_PATH,
        .Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
        .lpstrDefExt = L""
    };

    if (GetOpenFileName(&ofenFileName))
    {
        return std::wstring(filename);
    }

    return L""; // ����ڰ� ����� ��� �� ���ڿ� ��ȯ
}

void Assets::AddItem(const std::string& fileName)
{
    auto findItem = std::find_if(_items.begin(), _items.end(), 
        [&fileName](const auto& name)
        { 
            if (name == fileName)
                return true;

            return false;
        });

    if (_items.end() == findItem)
    {
        _items.push_back(fileName);
    }
}

void Assets::Free()
{
}
