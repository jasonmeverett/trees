// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

	// default tree type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree Data")
	TSubclassOf<AActor> treeType;

	UFUNCTION(BlueprintCallable)
	void regenerateTrees(int num, float minHeight, float maxHeight, float minRadius, float maxRadius);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tree Data")
	bool useCullingFeatures;

	// planetary material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planetary Data")
	UMaterial* planetaryMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// default planet mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* planetMeshComponent;

	// Spring-arm and camera
	UPROPERTY(EditAnywhere) 
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere) 
	UCameraComponent* cameraComp;

	// list of all current trees
	TArray<AActor*> trees;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
