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

static bool contains(std::vector<std::pair<float, float>> const& vec, std::pair<float, float> const& needle)
{
	for (auto const& ele : vec) {
		if (ele.first == needle.first && ele.second == needle.second) {
			return true;
		}
	}
	return false;
}

ESet<std::pair<float, float>>  ARoomSpawner::GetRoomLocations()
{
	ESet<std::pair<float, float>> room_set;
	ESet<std::pair<float, float>> adjacent_room_set{(unsigned) seed};
	room_set.insert(std::make_pair<float, float>(0.0f, 0.0f));	// add origin so player doesn't fall!

	adjacent_room_set.insert(std::make_pair<float, float>(1.0f, 0.0f));
	adjacent_room_set.insert(std::make_pair<float, float>(0.0f, 1.0f));

	uint8 count = 1;
	while (count <= num_rooms) {
		// pick random room
		auto current_room = adjacent_room_set.get_random();
		adjacent_room_set.erase(current_room);

		room_set.insert(current_room);

		// calculate adjacent rooms
		std::vector<std::pair<float,float>> adjacents = {
			{current_room.first, current_room.second-1},
			{current_room.first-1, current_room.second},
			{current_room.first+1, current_room.second},
			{current_room.first, current_room.second+1},
		};

		// add to adjacent room list
		for (auto & r : adjacents) {
			if (!room_set.contains(r)) {
				adjacent_room_set.insert(r);
			}
		}

		room_set.insert(current_room);
		UE_LOG(LogTemp, Warning, TEXT("Added room: %f, %f to create mesh asset"), current_room.first, current_room.second);
		count++;
	}

	return room_set;
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
