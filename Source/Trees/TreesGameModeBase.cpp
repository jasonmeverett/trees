// Copyright Epic Games, Inc. All Rights Reserved.


#include "TreesGameModeBase.h"

void ATreesGameModeBase::BeginPlay() {
	Super::BeginPlay();


	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
	CurrentWidget->AddToViewport();



}