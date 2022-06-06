// Fill out your copyright notice in the Description page of Project Settings.


#include "TreesPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Planet.h"


// constructor
ATreesPlayerController::ATreesPlayerController() {

	this->bShowMouseCursor = true;
	this->bEnableClickEvents = true;
	this->bEnableMouseOverEvents = true;

	rotateSpeed = 10.f;

}

void ATreesPlayerController::BeginPlay() {
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Player Controller Initialized"));

	// Get reference to the planet in the scene.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanet::StaticClass(), FoundActors);

	// There should only be one default planet in the scene
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, TEXT("Planets in Scene: " + FString::FromInt(FoundActors.Num())));

	// Add a reference to this planet.
	currentPlanet = Cast<APlanet>(FoundActors[0]);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, TEXT("Planet Radius: " + FString::SanitizeFloat(currentPlanet->radius) ));

	// Get spring arm component
	springArm = Cast<USpringArmComponent>(currentPlanet->GetComponentByClass(USpringArmComponent::StaticClass()));

	// set default camera dist
	springArm->TargetArmLength = 5.0 * currentPlanet->radius;

}

void ATreesPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Input mapped"));

	InputComponent->BindAxis("RotateLon", this, &ATreesPlayerController::RotateLon);
	InputComponent->BindAxis("RotateLat", this, &ATreesPlayerController::RotateLat);
	InputComponent->BindAxis("Zoom", this, &ATreesPlayerController::Zoom);


}


void ATreesPlayerController::Zoom(float d) {
	
	float speed = -0.05;

	// compute new distance
	float distNew = springArm->TargetArmLength * (1.0 + speed*d);

	// clamp value
	distNew = FMath::Clamp(distNew, 2.0 * currentPlanet->radius, 20.0 * currentPlanet->radius);

	// set value
	springArm->TargetArmLength = distNew;

}

void ATreesPlayerController::RotateLon(float dx) {
	yaw += rotateSpeed*dx;
	RotateSpring();

}
void ATreesPlayerController::RotateLat(float dy) {
	pitch += rotateSpeed*dy;
	RotateSpring();

}

void ATreesPlayerController::RotateSpring() {
	springArm->SetRelativeRotation(FQuat(FRotator(pitch, yaw, 0.f)));
}