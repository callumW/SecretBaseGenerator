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

	float const xOffset = 1000.0f;
	float const yOffset = 1000.0f;

	auto room_locations = GetRoomLocations();

	for (auto & loc : room_locations) {
		FVector location(loc.first * xOffset, loc.second * yOffset, 0.0f);
		AActor* tmp = GetWorld()->SpawnActor(ARoom::StaticClass());
		rooms.Add((ARoom*) tmp);
	}
}

std::vector<std::pair<float, float>> ARoomSpawner::GetRoomLocations()
{
	std::vector<std::pair<float, float>> ret;
	for (uint8 x = 0; x < width; x++) {
		for (uint8 y = 0; y < height; y++) {
			ret.push_back(std::make_pair<float, float>((float) x, (float) y));
		}
	}

	return ret;
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
