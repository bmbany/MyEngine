#pragma once
#include "Base.h"

namespace Engine
{
	class ImGUIManager : public Base, public Singleton<ImGUIManager>
	{
		friend class Singleton;
	private:
		explicit ImGUIManager() = default;
		virtual ~ImGUIManager() = default;

	public:
		void Initialize(HWND hWnd);
		void AddFunction(const std::function<void()>& function) { _functions.emplace_back(function); }
		void Render();

	private:
		// Base을(를) 통해 상속됨
		void Free() override;

	private:
		std::vector<std::function<void()>> _functions;
	};
}