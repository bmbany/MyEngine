#pragma once

#ifdef _DEBUG
#include "Engine_Define.h"
#include "Reflection.h"

namespace Engine
{
    class Component;
    class GameObject;
    class ComponentFactory
    {
    public:
        using ComponentCreator = std::function<Component*(GameObject*)>;
        using ComponentMap = std::unordered_map<std::wstring, ComponentCreator>;    

        static const bool RegisterComponent(const wchar_t* type, ComponentCreator creator)
        {
            ComponentMap& factorys = GetFactory();
            factorys[type] = creator;

            return true;
        }

        static Component* CreateComponent(const wchar_t* type, GameObject* pOwner)
        {
            ComponentMap& factorys = GetFactory();

            if (factorys.find(type) != factorys.end())
            {
                return factorys[type](pOwner);
            }

            return nullptr;
        }

    private:
        static ComponentMap& GetFactory()
        {
            static ComponentMap factorys;
            return factorys;
        }
    };
}
#endif