// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

UCLASS()
class TREES_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanet();

	// Radius of the planet in Unreal coordinates.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planetary Data")
	float radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planetary Data")
	UStaticMesh* planetaryMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// default planet mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* planetMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
