// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"

#include "Kismet/GameplayStatics.h"

#include "SceneManagement.h"

#include <random>
#include <vector>
#include <chrono>


// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// mesh component (root)
	planetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet Base"));
	planetMeshComponent->SetupAttachment(RootComponent);

	// camera spring arm
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->SetupAttachment(planetMeshComponent);

	// camera
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComp->SetupAttachment(springArm, USpringArmComponent::SocketName);

	// update the mesh
	planetMeshComponent->SetStaticMesh(planetaryMesh);
}

void APlanet::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// update the mesh
	planetMeshComponent->SetStaticMesh(planetaryMesh);

	// update spring arm length
	springArm->TargetArmLength = radius * 2.0;
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 3.0f;
	springArm->bEnableCameraRotationLag = true;
	springArm->CameraRotationLagSpeed = 3.0f;

	// reset the scale of the mesh
	// default radius is 50 of the mesh so scale by that
	double scale = radius * (1.0 / 50.0);
	planetMeshComponent->SetWorldScale3D(FVector(scale));

}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();


	// get reference to player controller.
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	playerController->SetViewTarget(this);
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// determine if culling features are enabled
	if (useCullingFeatures) {

		// loop through all of the trees
		for (auto tree : trees) {

			//don't do anything if the tree was removed by right-click
			if (!tree) continue;

			// determine if tree should be visible
			FVector v1 = cameraComp->GetComponentLocation().GetSafeNormal();
			FVector v2 = tree->GetActorLocation().GetSafeNormal();
			double prod = FVector::DotProduct(v1, v2);

			if (prod < 0.0) {
				tree->SetActorHiddenInGame(true);
			} else {
				tree->SetActorHiddenInGame(false);
			}

		}

	}
	else {

		// enable all trees
		for (auto tree : trees) {
			if (!tree) continue;

			tree->SetActorHiddenInGame(false);
		}
	}

}

// Regenerate trees
void APlanet::regenerateTrees(int num, float minHeight, float maxHeight, float minRadius, float maxRadius) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, TEXT("Trees Regenerated. Num: " + FString::FromInt(num)));

	// clear the current trees
	for (auto tree : trees) {
		if(tree) tree->Destroy();
	}
	trees.Empty();

	for (int i = 0; i < num; ++i) {

		// generate random location, radius and height of the tree
		float treeRadius	= FMath::RandRange(minRadius, maxRadius);
		float height		= FMath::RandRange(minHeight, maxHeight);
		float lat			= FMath::RandRange(-0.5*PI, 0.5*PI);
		float lon			= FMath::RandRange(0.0, 2.0 * PI);

		// Compute the location where this tree will be placed
		float x = radius * cos(lat) * cos(lon);
		float y = radius * cos(lat) * sin(lon);
		float z = radius * sin(lat);
		FVector loc(x, y, z);

		// compute the rotation. 
		FRotator rot(loc.Rotation());
		rot = rot.Add(-90, 0, 0);

		// spawn the new tree into the world.
		auto newTree = GetWorld()->SpawnActor(treeType, &loc, &rot);

		// default tree size is 100x100x100, so we need to scale appropriately
		// default radius is 50
		FVector scale(treeRadius / 100.0 * radius, treeRadius / 100.0 * radius, height / 100.0 * radius);
		newTree->SetActorScale3D(scale);

		// add to the array
		trees.Add(newTree);

	}

}

