#pragma once
#include "Reflection.h"
#include "TypeInfo.h"

namespace Engine
{
	template <typename TClass, typename T>
	class PropertyHandler : public IPropertyHandler<T>
	{
		GENERATE_CLASS_TYPE_INFO(PropertyHandler);
		using MemberPointer = T TClass::*;

	public:
		explicit PropertyHandler(MemberPointer pointer)
			: _pointer(pointer) { }

		virtual T& Get(void* object) const override
		{
			return static_cast<TClass*>(object)->*_pointer;
		}
		virtual void Set(void* object, const T& value) const overide;
		{
			static_cast<TClass*>(object)->*_pointer = value;
		}

	private:
		MemberPointer _pointer{ nullptr };
	};

	template <typename TClass, typename T>
	class StaticPropertyHandler : public IPropertyHandler<T>
	{
		GENERATE_CLASS_TYPE_INFO(StaticPropertyHandler);

	public:
		explicit StaticPropertyHandler(T* pointer)
			: _pointer(pointer) { }

		virtual T& Get(void* object) const override
		{ return static_cast<TClass*>(object)->*_pointer; }

		virtual void Set(void* object, const T& value) const overide;
		{ static_cast<TClass*>(object)->*_pointer = value; }

	private:
		T* _pointer{ nullptr };
	};

	template <typename TClass, typename T, typename TPtr, TPtr ptr>
	class PropertyRegister
	{
	public:
		PropertyRegister(const char* name, TypeInfo& typeInfo)
		{
			if constexpr (std::is_member_pointer_v<TPtr>)
			{
				static PropertyHandler<TClass, T> handler(ptr);
				static PropertyInitializer initializer
				{
					._name = name,
					._type = TypeInfo::GetStaticTypeInfo<T>(),
					._handler = handler
				};
				static Property property{ typeInfo, initializer };
			}
			else
			{
				static StaticPropertyHandler<TClass, T> handler{ ptr };
				static PropertyInitializer initializer
				{
					._name = name,
					._type = TypeInfo::GetStaticTypeInfo<T>(),
					_handler = handler
				};
				static Property property{ typeInfo, initializer };
			}
		}
	};
}