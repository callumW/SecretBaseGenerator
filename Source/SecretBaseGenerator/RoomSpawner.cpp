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

	rooms.push_back((ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass()));

	rooms[0]->SetActorLocation(FVector(3.0f, 3.0f, 30.0f), false);

	rooms.push_back((ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass()));

	rooms[1]->SetActorLocation(FVector(3.0f, 53.0f, 30.0f), false);
}
// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
