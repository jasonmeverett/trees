// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"

#include "Kismet/GameplayStatics.h"

#include <random>
#include <vector>
#include <chrono>


// Sets default values
APlanet::APlanet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planetMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planet Base"));
	//baseRotator = CreateDefaultSubobject<USceneComponent>(TEXT("Base Rotator"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));


	//planetMeshComponent->SetStaticMesh(planetaryMesh);
	//baseRotator->SetupAttachment(planetMeshComponent);
	springArm->SetupAttachment(planetMeshComponent);
	cameraComp->SetupAttachment(springArm, USpringArmComponent::SocketName);
}

void APlanet::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	// update the mesh
	//planetaryMesh->SetMaterial(0, planetaryMaterial);
	planetMeshComponent->SetStaticMesh(planetaryMesh);



	// update spring arm length
	springArm->TargetArmLength = radius * 2.0;
	springArm->bEnableCameraLag = true;
	springArm->CameraLagSpeed = 3.0f;

	// reset the scale of the mesh
	// default radius is 50 of the mesh so scale by that
	double scale = radius * (1.0 / 50.0);
	planetMeshComponent->SetWorldScale3D(FVector(scale));

}

// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();

	// update the mesh
	//planetMeshComponent->SetStaticMesh(planetaryMesh);

	// get reference to player controller.
	auto playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	playerController->SetViewTarget(this);
}

// Called every frame
void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (useCullingFeatures) {

		// loop through all of the trees
		for (auto tree : trees) {
			if (!tree) continue;

			// get the spring vector
			FVector v1 = cameraComp->GetComponentLocation().GetSafeNormal();
			FVector v2 = tree->GetActorLocation().GetSafeNormal();

			double prod = FVector::DotProduct(v1, v2);
			if (prod < 0.0) {
				tree->SetActorHiddenInGame(true);
			}
			else
			{
				tree->SetActorHiddenInGame(false);
			}

		}

	}
	else {
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

	// create new trees
	// first, come up with all random heights, radii, etc
	// assume random lat/lon
	//std::default_random_engine generator;
	//generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	//std::uniform_real_distribution<float> radiusDistribution(minRadius, maxRadius);
	//std::uniform_real_distribution<float> heightDistribution(minHeight, maxHeight);
	//std::uniform_real_distribution<float> latDistribution(-PI, PI);
	//std::uniform_real_distribution<float> lonDistribution(0, 2.0*PI);


	for (size_t i = 0; i < num; ++i) {

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
		auto newTree = GetWorld()->SpawnActor(treeType, &loc, &rot);
		

		// default tree size is 100x100x100, so we need to scale appropriately
		// default radius is 50
		FVector scale(treeRadius / 100.0 * radius, treeRadius / 100.0 * radius, height / 100.0 * radius);
		newTree->SetActorScale3D(scale);

		// add to the array
		trees.Add(newTree);

		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, TEXT("Position: " + FString::SanitizeFloat()));

	}



}

