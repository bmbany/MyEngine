#include "Renderer.h"
#include "GameObject.h"
#include "GraphicManager.h"
#include "ImGUIManager.h"

using namespace Engine;

void Engine::Renderer::Render_GameObject()
{
    if (!_isSetUp)
        return;
 
    g_pGraphicMgr->BeginDraw();

    for (int i = 0; i < _info.size; i++)
    {
        /*if (nullptr != _sortInfo[i])
            _renderGroup[i].sort(_sortInfo[i]);*/

        for (int j = 0; j < _renderGroupIndex[i]; j++)
        {
            _renderGroup[i][j]->Render();
        }

        _renderGroupIndex[i] = 0;
    }

    g_pImGUIMgr->Render();
    g_pGraphicMgr->EndDraw();
}

bool Engine::Renderer::SetUpRenderer(const RenderGroupInfo& info)
{
    if (_isSetUp)
        return false;

    _info = info;
    _renderGroup.resize(_info.size);
    _sortInfo.resize(_info.size);
    _renderGroupIndex.resize(_info.size);

    for (auto& group : _renderGroup)
        group.resize(2000);

    _isSetUp = true;

    return true;
}

void Engine::Renderer::SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*))
{
    _sortInfo[sortGroup] = sortFunc;
}

bool Engine::Renderer::AddRenderGroup(int group, GameObject* pObject)
{    
    if (nullptr == pObject || pObject->IsDead())
        return false;

    _renderGroup[group][_renderGroupIndex[group]++] = pObject;

    return true;
}

void Engine::Renderer::ResetRenderGroup()
{
    for (int i = 0; i < _info.size; i++)
        _renderGroup[i].clear();
}

void Engine::Renderer::Free()
{
}

Renderer* Engine::Renderer::Create()
{
    return new Renderer;
}
