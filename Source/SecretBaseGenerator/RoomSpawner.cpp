// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"

// Sets default values
ARoomSpawner::ARoomSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	try {
		auto room_locations = GetRoomLocations();

		for (auto & loc : room_locations) {
			FVector location(loc.x * xOffset, loc.y * yOffset, height);
			ARoom* tmp = (ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass());
			tmp->Initialize(loc);
			tmp->SetActorLocation(location);
			rooms.Add(tmp);
		}
	}
	catch(std::exception const& e) {
		UE_LOG(LogTemp, Warning, TEXT("Caught exception: %s"), e.what());
	}
}

ESet<RoomBlock>  ARoomSpawner::GetRoomLocations()
{
	ESet<RoomBlock> room_set;
	ESet<RoomBlock> adjacent_room_set{(unsigned) seed};
	room_set.insert(RoomBlock(0.0f, 0.0f));	// add origin so player doesn't fall!

	adjacent_room_set.insert(RoomBlock(1.0f, 0.0f));
	adjacent_room_set.insert(RoomBlock(0.0f, 1.0f));

	uint8 count = 1;
	while (count <= num_rooms) {
		// pick random room
		auto current_room = adjacent_room_set.get_random();
		adjacent_room_set.erase(current_room);

		room_set.insert(current_room);

		// calculate adjacent rooms
		std::vector<RoomBlock> adjacents = {
			{current_room.x, current_room.y-1},
			{current_room.x-1, current_room.y},
			{current_room.x+1, current_room.y},
			{current_room.x, current_room.y+1},
		};

		// add to adjacent room list
		for (auto & r : adjacents) {
			if (!room_set.contains(r)) {
				adjacent_room_set.insert(r);
			}
		}

		room_set.insert(current_room);
		count++;
	}

	// Setup open walls
	for (auto & room : room_set) {
		RoomBlock bottom{room.x, room.y-1};
		RoomBlock left{room.x-1, room.y};
		RoomBlock right{room.x+1, room.y};
		RoomBlock top{room.x, room.y+1};


		if (room_set.contains(bottom)) {
			room.walls[2] = WALL_TYPE::DOOR;
		}
		if (room_set.contains(left)) {
			room.walls[3] = WALL_TYPE::DOOR;
		}
		if (room_set.contains(right)) {
			room.walls[1] = WALL_TYPE::DOOR;
		}
		if (room_set.contains(top)) {
			room.walls[0] = WALL_TYPE::DOOR;
		}
	}

	return room_set;
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
