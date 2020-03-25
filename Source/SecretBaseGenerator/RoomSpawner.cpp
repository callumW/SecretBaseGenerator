// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomSpawner.h"

#include "LevelGenerator.h"

// Sets default values
ARoomSpawner::ARoomSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_rand_engine.seed(seed);
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
	float zOffset = 0.0f;
	float const z_increment = 600.0f;


	LevelGeneration::LevelGenerator genie;

	for (int floor_count = 0; floor_count < num_of_levels; floor_count++) {
		auto nodes = genie.GenerateLevel(num_of_nodes, num_rooms_per_level, m_rand_engine());

		for (auto & node : nodes) {
			// if (node.type == LevelGeneration::NODE_TYPE::ROOM) {
				FVector location(node.x * xOffset, node.y * yOffset, zOffset);
				ARoom* tmp = (ARoom*) GetWorld()->SpawnActor(ARoom::StaticClass());
				tmp->Initialize(node);
				tmp->SetActorLocation(location);
				rooms.Add(tmp);
			// }
			// else if (node.type == LevelGeneration::NODE_TYPE::STAIR_WELL) {
			//
			// }
		}

		zOffset += z_increment;
	}
}

// Called every frame
void ARoomSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
