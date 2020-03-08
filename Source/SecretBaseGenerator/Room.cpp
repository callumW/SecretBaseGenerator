// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

// Sets default values
ARoom::ARoom()
	:AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floor mesh"));

	if (floor) {
		RootComponent = floor;

		ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(TEXT("StaticMesh'/Game/Models/floor.floor'"));

		if (mesh_asset.Succeeded()) {
			floor->SetStaticMesh(mesh_asset.Object);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("failed to create mesh asset"));
		}
	}

	LoadWall(FVector(0.0f, -495.0f, 137.5f), FRotator(0.0f, 90.0f, 0.0f), "left wall");
	LoadWall(FVector(0.0f, 495.0f, 137.5f), FRotator(0.0f, -90.0f, 0.0f), "right wall");
	LoadWall(FVector(495.0f, 0.0f, 137.5f), FRotator(0.0f, 180.0f, 0.0f), "front wall");
	LoadWall(FVector(-495.0f, 0.0f, 137.5f), FRotator(0.0f, 0.0f, 0.0f), "back wall");
}

void ARoom::LoadFloor()
{

}

void ARoom::LoadWall(FVector loc, FRotator rotation, FName name)
{
	auto wall_mesh = CreateDefaultSubobject<UStaticMeshComponent>(name);
	meshes.Add(wall_mesh);

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(TEXT("StaticMesh'/Game/Models/wall_door.wall_door'"));

	if (mesh_asset.Succeeded()) {
		wall_mesh->SetStaticMesh(mesh_asset.Object);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed to create mesh asset"));
	}

	if (RootComponent) {
		wall_mesh->SetupAttachment(RootComponent);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not attaching wall"));
	}
	wall_mesh->SetRelativeLocation(loc, false);

	wall_mesh->AddLocalRotation(rotation, false);
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
