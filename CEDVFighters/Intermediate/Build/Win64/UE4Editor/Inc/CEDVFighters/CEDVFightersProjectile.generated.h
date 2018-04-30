// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FVector;
struct FHitResult;
#ifdef CEDVFIGHTERS_CEDVFightersProjectile_generated_h
#error "CEDVFightersProjectile.generated.h already included, missing '#pragma once' in CEDVFightersProjectile.h"
#endif
#define CEDVFIGHTERS_CEDVFightersProjectile_generated_h

#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnHit) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_HitComp); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_STRUCT(FVector,Z_Param_NormalImpulse); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_Hit); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->OnHit(Z_Param_HitComp,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_NormalImpulse,Z_Param_Out_Hit); \
		P_NATIVE_END; \
	}


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnHit) \
	{ \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_HitComp); \
		P_GET_OBJECT(AActor,Z_Param_OtherActor); \
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_OtherComp); \
		P_GET_STRUCT(FVector,Z_Param_NormalImpulse); \
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_Hit); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		P_THIS->OnHit(Z_Param_HitComp,Z_Param_OtherActor,Z_Param_OtherComp,Z_Param_NormalImpulse,Z_Param_Out_Hit); \
		P_NATIVE_END; \
	}


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesACEDVFightersProjectile(); \
	friend CEDVFIGHTERS_API class UClass* Z_Construct_UClass_ACEDVFightersProjectile(); \
public: \
	DECLARE_CLASS(ACEDVFightersProjectile, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/CEDVFighters"), NO_API) \
	DECLARE_SERIALIZER(ACEDVFightersProjectile) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_INCLASS \
private: \
	static void StaticRegisterNativesACEDVFightersProjectile(); \
	friend CEDVFIGHTERS_API class UClass* Z_Construct_UClass_ACEDVFightersProjectile(); \
public: \
	DECLARE_CLASS(ACEDVFightersProjectile, AActor, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/CEDVFighters"), NO_API) \
	DECLARE_SERIALIZER(ACEDVFightersProjectile) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ACEDVFightersProjectile(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ACEDVFightersProjectile) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACEDVFightersProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACEDVFightersProjectile); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACEDVFightersProjectile(ACEDVFightersProjectile&&); \
	NO_API ACEDVFightersProjectile(const ACEDVFightersProjectile&); \
public:


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACEDVFightersProjectile(ACEDVFightersProjectile&&); \
	NO_API ACEDVFightersProjectile(const ACEDVFightersProjectile&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACEDVFightersProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACEDVFightersProjectile); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ACEDVFightersProjectile)


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__ProjectileMesh() { return STRUCT_OFFSET(ACEDVFightersProjectile, ProjectileMesh); } \
	FORCEINLINE static uint32 __PPO__ProjectileMovement() { return STRUCT_OFFSET(ACEDVFightersProjectile, ProjectileMovement); }


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_12_PROLOG
#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_RPC_WRAPPERS \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_INCLASS \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_INCLASS_NO_PURE_DECLS \
	CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID CEDVFighters_Source_CEDVFighters_CEDVFightersProjectile_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
