#pragma once

#ifdef _DEBUG
	#define REGISTER_COMPONENT(Type) \
	struct Dummy_##Type { inline static const bool dummy = ComponentFactory::RegisterComponent(L#Type, \
	[](GameObject* pGameObject) { return (Component*)pGameObject->AddComponent<Type>(L#Type); }); };
#else
	#define REGISTER_COMPOENT(Type) (void(0))
	#define REGISTER_PROPERTY(Type) (void(0))
#endif

#define PROPERTY(Name) \
inline static struct RegistPropertyExecutor_##Name \
{ \
	RegistPropertyExecutor_##Name() \
	{ \
		static PropertyRegister<ThisType, decltype(_##Name), decltype(&ThisType::_##Name), &ThisType::_##Name> property_register_##Name \
		{ #Name, ThisType::StaticTypeInfo() };\
	} \
} \