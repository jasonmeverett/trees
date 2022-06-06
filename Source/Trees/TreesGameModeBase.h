// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TreesGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TREES_API ATreesGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Trees")
	int maxTrees;

};
