// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"

// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Room Spawner online!"));
	}

	float xOffset = 1000.0f;
	float yOffset = 1000.0f;

	for (uint8 x = 0; x < width; x++) {
		for (uint8 y = 0; y < height; y++) {
			FVector location(((float)x) * xOffset, ((float)y) * yOffset, 0.0f);
			AActor* tmp = GetWorld()->SpawnActor(ARoom::StaticClass());
			tmp->SetActorLocation(location);
			rooms.Add((ARoom*) tmp);
		}
	}

	// rooms.Add((ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass()));
	//
	// rooms.Add((ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass()));
	//
	// rooms[1]->SetActorLocation(FVector(0.0f, 1000.0f, 0.0f));
}
// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
