#pragma once
#include "Component.h"

namespace Engine
{
	class BitFlag : public Component
	{
	public:
		explicit BitFlag(const wchar_t* name);
	private:
		virtual ~BitFlag() = default;

	public:
		inline void OnFlag(const _ullong& flag) { _flag |= flag; }
		inline void OffFlag(const _ullong& flag) { _flag &= ~flag; }
		inline void ToggleFlag(const _ullong& flag) { _flag ^= flag; }
		inline _ullong CheckFlag(const _ullong& flag) const { return _flag & flag; }
		inline void ResetFlag() { _flag = 0; }
		inline void SetFlag(const _ullong& flag) { _flag = flag; }
		inline _ullong GetCurrentFlag() const { return _flag; }

	private:
		// Component을(를) 통해 상속됨
		void Free() override;

	private:
		_ullong _flag = 0;
	};
}