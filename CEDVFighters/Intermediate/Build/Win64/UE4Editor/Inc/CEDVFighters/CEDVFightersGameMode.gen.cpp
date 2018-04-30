// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "CEDVFightersGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCEDVFightersGameMode() {}
// Cross Module References
	CEDVFIGHTERS_API UClass* Z_Construct_UClass_ACEDVFightersGameMode_NoRegister();
	CEDVFIGHTERS_API UClass* Z_Construct_UClass_ACEDVFightersGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_CEDVFighters();
// End Cross Module References
	void ACEDVFightersGameMode::StaticRegisterNativesACEDVFightersGameMode()
	{
	}
	UClass* Z_Construct_UClass_ACEDVFightersGameMode_NoRegister()
	{
		return ACEDVFightersGameMode::StaticClass();
	}
	UClass* Z_Construct_UClass_ACEDVFightersGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_AGameModeBase,
				(UObject* (*)())Z_Construct_UPackage__Script_CEDVFighters,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
				{ "IncludePath", "CEDVFightersGameMode.h" },
				{ "ModuleRelativePath", "CEDVFightersGameMode.h" },
				{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
			};
#endif
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<ACEDVFightersGameMode>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&ACEDVFightersGameMode::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00880288u,
				nullptr, 0,
				nullptr, 0,
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACEDVFightersGameMode, 1725489414);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACEDVFightersGameMode(Z_Construct_UClass_ACEDVFightersGameMode, &ACEDVFightersGameMode::StaticClass, TEXT("/Script/CEDVFighters"), TEXT("ACEDVFightersGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACEDVFightersGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
