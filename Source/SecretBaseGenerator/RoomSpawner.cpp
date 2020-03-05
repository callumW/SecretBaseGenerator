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
		tmp->SetActorLocation(location);
		rooms.Add((ARoom*) tmp);
	}
}

std::set<std::pair<float, float>> ARoomSpawner::GetRoomLocations()
{
	std::set<std::pair<float, float>> ret;
	ret.insert(std::make_pair<float, float>(0.0f, 0.0f));	// add origin so player doesn't fall!

	uint8 count = 1;
	while (count <= num_rooms) {
		float randomX, randomY;
		randomX = FMath::RandRange(0, width - 1);
		randomY = FMath::RandRange(0, height - 1);

		auto it = ret.insert(std::make_pair<float, float>((float) randomX, (float) randomY));
		if (it.second) {	// successfully added location
			UE_LOG(LogTemp, Warning, TEXT("Added room: %f, %f to create mesh asset"), (float) randomX, (float) randomY);
			count++;
		}
	}

	return ret;
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
