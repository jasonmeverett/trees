// Copyright Epic Games, Inc. All Rights Reserved.


#include "TreesGameModeBase.h"

void ATreesGameModeBase::BeginPlay() {
	Super::BeginPlay();

	// Add our UI to the viewport on start.
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
	CurrentWidget->AddToViewport();



}