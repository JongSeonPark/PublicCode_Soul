// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSoul, Log, All);

// 로그 위치 알고 싶을 때, 아래와 교체
// #define SOUL_LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
// #define SOUL_LOG_S(Verbosity) UE_LOG(LogSoul, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
// #define SOUL_LOG(Verbosity, Format, ...) UE_LOG(LogSoul, Verbosity, TEXT("%s %s"), *SOUL_LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define SOUL_LOG(Verbosity, Format, ...) UE_LOG(LogSoul, Verbosity, Format, ##__VA_ARGS__)

// PhysicalMaterial
#define SURFACE_SNOW EPhysicalSurface::SurfaceType1
#define SURFACE_WATER EPhysicalSurface::SurfaceType2
#define SURFACE_SOIL EPhysicalSurface::SurfaceType3
#define SURFACE_DESERT EPhysicalSurface::SurfaceType4
#define SURFACE_LEAF EPhysicalSurface::SurfaceType5
#define SURFACE_DEBRIS EPhysicalSurface::SurfaceType6
#define SURFACE_MUD EPhysicalSurface::SurfaceType7
