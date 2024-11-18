#pragma once
#include "Base.h"

namespace Engine
{
	class GameObject;
	class Renderer : public Base
	{
	public:
		struct RenderGroupInfo
		{
			ID3D11DeviceContext* pDeviceContext;
			IDXGISwapChain* pSwapChain;
			int size;
			int width;
			int height;
		};
	private:
		using RenderGroup = std::vector<std::vector<GameObject*>>;
		using SortInfo = std::vector<bool(*)(GameObject*, GameObject*)>;

	private:
		explicit Renderer() = default;
		virtual ~Renderer() = default;

	public:
		void Render_GameObject();
		bool SetUpRenderer(const RenderGroupInfo& info);
		void SetSortGroup(int sortGroup, bool(*sortFunc)(GameObject*, GameObject*));
		bool AddRenderGroup(int group, GameObject* pObject);
		void ResetRenderGroup();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	public:
		static Renderer* Create();

	private:
		RenderGroupInfo		_info;
		RenderGroup			_renderGroup;
		std::vector<int>	_renderGroupIndex;
		SortInfo			_sortInfo;
		bool				_isSetUp{ false };
	};
}

