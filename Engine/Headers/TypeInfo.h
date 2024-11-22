#pragma once;
#include <concepts>
#include <string>

namespace Engine
{
	template <typename T, typename U = void>
	struct SuperClassTypeDeduction
	{ using Type = void; };

	template <typename T>
	struct SuperClassTypeDeduction<T, std::void_t<typename T::ThisType>>
	{ using Type = T::ThisType; };

	template <typename T>
	concept HasSuper = requires
	{ typename T::Super; } && !std::same_as<typename T::Super, void>;

	template <typename T>
	struct TypeInfoInitializer
	{
		TypeInfoInitializer(const char* name)
			: _name(name)
		{
			if constexpr (HasSuper<T>)
			{
				_pSuper = &(typename T::Super::StaticTypeInfo());
			}
		}

		const char* _name{ nullptr };
		const TypeInfo* _pSuper{ nullptr };
	};

	class TypeInfo
	{
	public:
		template <typename T>
		explicit TypeInfo(const TypeInfoInitializer<T>& initializer)
			: _typeHash(typeid(T).hash_code())
			, _fullName(typeid(T).name())
			, _name(initializer._name)
			, _pSuper(initializer._pSuper)
		{
		}

	private:
		bool IsA(const TypeInfo& other) const
		{
			auto hashcode = typeid(int).hash_code();
		}

	private:
		std::string		_fullName;
		size_t			_typeHash{ 0 };
		const char*		_name{ nullptr };
		const TypeInfo*	_pSuper{ nullptr };
	};
}