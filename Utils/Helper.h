//
//  Helper.h
//  Asura
//
//  Created by Euclid Jan Guillermo on 1/22/26.
//

#pragma once
#include "../Unity/GameObject.h"
#include "../Unity/Object.h"
#include "../Unity/Component.h"

static CComponent *GetMonoBehaviour()
{
    Il2CppArray<CGameObject *> *m_Objects = Object::FindObjectsOfType<CGameObject>(UNITY_GAMEOBJECT_CLASS);
    for (uintptr_t u = 0U; m_Objects->m_uMaxLength > u; ++u)
    {
        CGameObject *m_Object = m_Objects->At(static_cast<unsigned int>(u));
        if (!m_Object)
            continue;

        CComponent *m_MonoBehaviour = m_Object->GetComponentByIndex(UNITY_MONOBEHAVIOUR_CLASS);
        if (m_MonoBehaviour)
            return m_MonoBehaviour;
    }

    return nullptr;
}
