// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GeoTechTest/GeoTechTestGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGeoTechTestGameModeBase() {}
// Cross Module References
	GEOTECHTEST_API UClass* Z_Construct_UClass_AGeoTechTestGameModeBase_NoRegister();
	GEOTECHTEST_API UClass* Z_Construct_UClass_AGeoTechTestGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_GeoTechTest();
// End Cross Module References
	void AGeoTechTestGameModeBase::StaticRegisterNativesAGeoTechTestGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AGeoTechTestGameModeBase);
	UClass* Z_Construct_UClass_AGeoTechTestGameModeBase_NoRegister()
	{
		return AGeoTechTestGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AGeoTechTestGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_GeoTechTest,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "GeoTechTestGameModeBase.h" },
		{ "ModuleRelativePath", "GeoTechTestGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGeoTechTestGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::ClassParams = {
		&AGeoTechTestGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGeoTechTestGameModeBase()
	{
		if (!Z_Registration_Info_UClass_AGeoTechTestGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AGeoTechTestGameModeBase.OuterSingleton, Z_Construct_UClass_AGeoTechTestGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AGeoTechTestGameModeBase.OuterSingleton;
	}
	template<> GEOTECHTEST_API UClass* StaticClass<AGeoTechTestGameModeBase>()
	{
		return AGeoTechTestGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGeoTechTestGameModeBase);
	struct Z_CompiledInDeferFile_FID_GeoTechTest_Source_GeoTechTest_GeoTechTestGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_GeoTechTest_Source_GeoTechTest_GeoTechTestGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AGeoTechTestGameModeBase, AGeoTechTestGameModeBase::StaticClass, TEXT("AGeoTechTestGameModeBase"), &Z_Registration_Info_UClass_AGeoTechTestGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AGeoTechTestGameModeBase), 810154693U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_GeoTechTest_Source_GeoTechTest_GeoTechTestGameModeBase_h_939946873(TEXT("/Script/GeoTechTest"),
		Z_CompiledInDeferFile_FID_GeoTechTest_Source_GeoTechTest_GeoTechTestGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_GeoTechTest_Source_GeoTechTest_GeoTechTestGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
