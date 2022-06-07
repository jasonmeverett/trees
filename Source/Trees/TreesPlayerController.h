// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"


#include "Planet.h"

#include "TreesPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class TREES_API ATreesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Constructor
	ATreesPlayerController();




protected:
	// begin play
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// camera rotation and zoom functions
	void RotateLon(float dx);
	void RotateLat(float dy);
	void Zoom(float d);
	void RotateSpring();

	// reference to planet
	APlanet* currentPlanet;
	USpringArmComponent* springArm; // reference to planet spring arm

	// camera functionality values
	float pitch, yaw;
	float rotateSpeed;
	float dist;

	// get current planet link
	UFUNCTION(BlueprintCallable)
	APlanet* getCurrentPlanet() { return currentPlanet; }

	
};
