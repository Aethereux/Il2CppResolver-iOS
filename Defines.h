//
//  Defines.h
//  Asura
//
//  Created by Eux on 1/21/26.
//

#pragma once

#include "Utils/Memory.h"
#include "VMTHook.h"

#include <string>
#include <codecvt>
#include <locale>


#define IL2CPP_INIT_EXPORT                                "il2cpp_init"
#define IL2CPP_CLASS_FROM_NAME_EXPORT                    "il2cpp_class_from_name"
#define IL2CPP_CLASS_GET_FIELDS                            "il2cpp_class_get_fields"
#define IL2CPP_CLASS_GET_FIELD_FROM_NAME_EXPORT            "il2cpp_class_get_field_from_name"
#define IL2CPP_CLASS_GET_METHODS                        "il2cpp_class_get_methods"
#define IL2CPP_CLASS_GET_METHOD_FROM_NAME_EXPORT        "il2cpp_class_get_method_from_name"
#define IL2CPP_CLASS_GET_PROPERTY_FROM_NAME_EXPORT        "il2cpp_class_get_property_from_name"
#define IL2CPP_CLASS_GET_TYPE_EXPORT                    "il2cpp_class_get_type"
#define IL2CPP_DOMAIN_GET_EXPORT                        "il2cpp_domain_get"
#define IL2CPP_DOMAIN_GET_ASSEMBLIES_EXPORT                "il2cpp_domain_get_assemblies"
#define IL2CPP_FREE_EXPORT                                "il2cpp_free"
#define IL2CPP_IMAGE_GET_CLASS_EXPORT                    "il2cpp_image_get_class"
#define IL2CPP_IMAGE_GET_CLASS_COUNT_EXPORT                "il2cpp_image_get_class_count"
#define IL2CPP_RESOLVE_FUNC_EXPORT                        "il2cpp_resolve_icall"
#define IL2CPP_STRING_NEW_EXPORT                        "il2cpp_string_new"
#define IL2CPP_THREAD_ATTACH_EXPORT                        "il2cpp_thread_attach"
#define IL2CPP_THREAD_DETACH_EXPORT                        "il2cpp_thread_detach"
#define IL2CPP_TYPE_GET_OBJECT_EXPORT                    "il2cpp_type_get_object"
#define IL2CPP_OBJECT_NEW                                "il2cpp_object_new"
#define IL2CPP_METHOD_GET_PARAM_NAME                    "il2cpp_method_get_param_name"
#define IL2CPP_METHOD_GET_PARAM                            "il2cpp_method_get_param"
#define IL2CPP_CLASS_FROM_IL2CPP_TYPE                    "il2cpp_class_from_il2cpp_type"
#define IL2CPP_FIELD_STATIC_GET_VALUE                    "il2cpp_field_static_get_value"
#define IL2CPP_FIELD_STATIC_SET_VALUE                    "il2cpp_field_static_set_value"

#define IL2CPP_ASSEMBLY_GET_IMAGE                        "il2cpp_assembly_get_image"
#define IL2CPP_IMAGE_GET_NAME                            "il2cpp_image_get_name"

// Camera
#define UNITY_CAMERA_CLASS                                          "UnityEngine.Camera"
#define UNITY_CAMERA_GETCURRENT                                     UNITY_CAMERA_CLASS"::get_current"
#define UNITY_CAMERA_GETMAIN                                        UNITY_CAMERA_CLASS"::get_main"
#define UNITY_CAMERA_GETDEPTH                                       UNITY_CAMERA_CLASS"::get_depth"
#define UNITY_CAMERA_SETDEPTH                                       UNITY_CAMERA_CLASS"::set_depth"
#define UNITY_CAMERA_GETFIELDOFVIEW                                 UNITY_CAMERA_CLASS"::get_fieldOfView"
#define UNITY_CAMERA_SETFIELDOFVIEW                                 UNITY_CAMERA_CLASS"::set_fieldOfView"
#define UNITY_CAMERA_WORLDTOSCREEN                                  UNITY_CAMERA_CLASS"::WorldToScreenPoint_Injected"
#define UNITY_CAMERA_GETPROJECTIONMATRIX                            UNITY_CAMERA_CLASS"::get_projectionMatrix_Injected"
#define UNITY_CAMERA_WORLDTOCAMERAMATRIX                            UNITY_CAMERA_CLASS"::get_worldToCameraMatrix_Injected"
#define UNITY_CAMERA_SCREENTOWORLDPOINT                             UNITY_CAMERA_CLASS"::ScreenToWorldPoint_Injected")
#define UNITY_CAMERA_CAMERATOWORLDMATRIX                            UNITY_CAMERA_CLASS"::get_cameraToWorldMatrix_Injected"
#define UNITY_CAMERA_WORLDTOVIEWPORT                                UNITY_CAMERA_CLASS"::WorldToViewportPoint_Injected"
// Component
#define UNITY_COMPONENT_CLASS                                        "UnityEngine.Component"
#define UNITY_COMPONENT_GETGAMEOBJECT                                UNITY_COMPONENT_CLASS"::get_gameObject"
#define UNITY_COMPONENT_GETTRANSFORM                                UNITY_COMPONENT_CLASS"::get_transform"

// GameObject
#define UNITY_GAMEOBJECT_CLASS                                        "UnityEngine.GameObject"
#define UNITY_GAMEOBJECT_ADDCOMPONENT                                UNITY_GAMEOBJECT_CLASS"::Internal_AddComponentWithType"
#define UNITY_GAMEOBJECT_CREATEPRIMITIVE                             UNITY_GAMEOBJECT_CLASS"::CreatePrimitive"
#define UNITY_GAMEOBJECT_FIND                                        UNITY_GAMEOBJECT_CLASS"::Find"
#define UNITY_GAMEOBJECT_GETCOMPONENT                                UNITY_GAMEOBJECT_CLASS"::GetComponentByName"
#define UNITY_GAMEOBJECT_GETCOMPONENTS                               UNITY_GAMEOBJECT_CLASS"::GetComponentsInternal"
#define UNITY_GAMEOBJECT_GETCOMPONENTINCHILDREN                      UNITY_GAMEOBJECT_CLASS"::GetComponentInChildren"
#define UNITY_GAMEOBJECT_GETACTIVE                                   UNITY_GAMEOBJECT_CLASS"::get_active"
#define UNITY_GAMEOBJECT_GETLAYER                                    UNITY_GAMEOBJECT_CLASS"::get_layer"
#define UNITY_GAMEOBJECT_GETTRANSFORM                                UNITY_GAMEOBJECT_CLASS"::get_transform"
#define UNITY_GAMEOBJECT_SETACTIVE                                   UNITY_GAMEOBJECT_CLASS"::set_active"
#define UNITY_GAMEOBJECT_SETLAYER                                    UNITY_GAMEOBJECT_CLASS"::set_layer"
#define UNITY_GAMEOBJECT_FINDGAMEOBJECTWITHTAG                       UNITY_GAMEOBJECT_CLASS"::FindGameObjectsWithTag"

// LayerMask
#define UNITY_LAYERMASK_CLASS                                        "UnityEngine.LayerMask"
#define UNITY_LAYERMASK_LAYERTONAME                                 UNITY_LAYERMASK_CLASS"::LayerToName"
#define UNITY_LAYERMASK_NAMETOLAYER                                 UNITY_LAYERMASK_CLASS"::NameToLayer"

// MonoBehaviour
#define UNITY_MONOBEHAVIOUR_CLASS                                   "UnityEngine.MonoBehaviour"

// Object
#define UNITY_OBJECT_CLASS                                            "UnityEngine.Object"
#define UNITY_OBJECT_DESTROY                                        UNITY_OBJECT_CLASS"::Destroy"
#define UNITY_OBJECT_FINDOBJECTSOFTYPE                              UNITY_OBJECT_CLASS"::FindObjectsOfType(System.Type,System.Boolean)"
#define UNITY_OBJECT_GETNAME                                        UNITY_OBJECT_CLASS"::GetName"

// Rigidbody
#define UNITY_RIGIDBODY_CLASS                                        "UnityEngine.Rigidbody"
#define UNITY_RIGIDBODY_GETDETECTCOLLISIONS                         UNITY_RIGIDBODY_CLASS"::get_detectCollisions"
#define UNITY_RIGIDBODY_GETVELOCITY                                 UNITY_RIGIDBODY_CLASS"::get_velocity_Injected"
#define UNITY_RIGIDBODY_SETDETECTCOLLISIONS                         UNITY_RIGIDBODY_CLASS"::set_detectCollisions"
#define UNITY_RIGIDBODY_SETVELOCITY                                 UNITY_RIGIDBODY_CLASS"::set_velocity_Injected"

// Transform
#define UNITY_TRANSFORM_CLASS                                        "UnityEngine.Transform"
#define UNITY_TRANSFORM_GETPARENT                                   UNITY_TRANSFORM_CLASS"::GetParent"
#define UNITY_TRANSFORM_GETROOT                                     UNITY_TRANSFORM_CLASS"::GetRoot"
#define UNITY_TRANSFORM_GETCHILD                                    UNITY_TRANSFORM_CLASS"::GetChild"
#define UNITY_TRANSFORM_GETCHILDCOUNT                               UNITY_TRANSFORM_CLASS"::get_childCount"
#define UNITY_TRANSFORM_FINDCHILD                                   UNITY_TRANSFORM_CLASS"::FindRelativeTransformWithPath"
#define UNITY_TRANSFORM_GETPOSITION                                 UNITY_TRANSFORM_CLASS"::get_position_Injected"
#define UNITY_TRANSFORM_GETROTATION                                 UNITY_TRANSFORM_CLASS"::get_rotation_Injected"
#define UNITY_TRANSFORM_GETLOCALPOSITION                            UNITY_TRANSFORM_CLASS"::get_localPosition_Injected"
#define UNITY_TRANSFORM_GETLOCALSCALE                               UNITY_TRANSFORM_CLASS"::get_localScale_Injected"
#define UNITY_TRANSFORM_SETPOSITION                                 UNITY_TRANSFORM_CLASS"::set_position_Injected"
#define UNITY_TRANSFORM_SETROTATION                                 UNITY_TRANSFORM_CLASS"::set_rotation_Injected"
#define UNITY_TRANSFORM_GETFORWARD                                  UNITY_TRANSFORM_CLASS"::get_forward"
#define UNITY_TRANSFORM_SETFORWARD                                  UNITY_TRANSFORM_CLASS"::set_forward"
#define UNITY_TRANSFORM_SETLOCALPOSITION                            UNITY_TRANSFORM_CLASS"::set_localPosition_Injected"
#define UNITY_TRANSFORM_SETLOCALSCALE                               UNITY_TRANSFORM_CLASS"::set_localScale_Injected"

// Texture
#define UNITY_TEXTURE_CLASS                                         "UnityEngine.Texture"
#define UNITY_TEXTURE_GETNATIVETEXTUREPTR                           UNITY_TEXTURE_CLASS"::GetNativeTexturePtr"


namespace Unity
{
    enum m_eTypeEnum
    {
        Type_Void = 1,
        Type_Boolean = 2,
        Type_Character = 3,
        Type_Integer = 8,
        Type_Float = 12,
        Type_String = 14,
        Type_Pointer = 15,
        Type_ValueType = 17,
        Type_Class = 18,
        Type_Variable = 19,
        Type_Array = 20,
        Type_Enum = 85,
    };
    
    enum m_eFieldAttribute
    {
        FieldAttribute_Compiler,
        FieldAttribute_Private,
        FieldAttribute_FamilyAndAssembly,
        FieldAttribute_Assembly,
        FieldAttribute_Family,
        FieldAttribute_FamilyOrAssembly,
        FieldAttribute_Public,
        FieldAttribute_AccessMask,
        FieldAttribute_Static = 16,
        FieldAttribute_InitOnly = 32,
        FieldAttribute_Literal = 64,
        FieldAttribute_NotSerialized = 128,
        FieldAttribute_HasRVA = 256,
        FieldAttribute_SpecialName = 512,
        FieldAttribute_RTSpecialName = 1024,
        FieldAttribute_HasMarshal = 4096,
        FieldAttribute_InvokeImpl = 8192,
        FieldAttribute_Default = 32768,
        FieldAttribute_Reserved = 38144,
    };
}
