// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

#include "Components/PointLightComponent.h"

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

	// Initialize(RoomBlock(0.0f, 0.0f));
}

void ARoom::LoadWall(FVector loc, FRotator rotation, FName name, WALL_TYPE const type)
{
	auto wall_mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), name);
	meshes.Add(wall_mesh);

	TCHAR const * mesh_name = nullptr;

	switch(type) {
		case WALL_TYPE::WALL:
			mesh_name = TEXT("StaticMesh'/Game/Models/wall.wall'");
			break;
		case WALL_TYPE::DOOR:
			mesh_name = TEXT("StaticMesh'/Game/Models/wall_door.wall_door'");
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("unknown wall type"));
			mesh_name = TEXT("StaticMesh'/Game/Models/wall_door.wall_door'");
	}

	//ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(mesh_name);
	UStaticMesh* mesh_asset = (UStaticMesh*) StaticLoadObject(UStaticMesh::StaticClass(), nullptr, mesh_name);


	if (mesh_asset) {
		wall_mesh->SetStaticMesh(mesh_asset);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed to create mesh asset"));
	}

	wall_mesh->RegisterComponent();

	if (RootComponent) {
		wall_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not attaching wall"));
	}
	wall_mesh->SetRelativeLocation(loc, false);

	wall_mesh->AddLocalRotation(rotation, false);
}

void ARoom::LoadCeiling()
{
	auto ceiling_mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("ceiling"));
	meshes.Add(ceiling_mesh);

	UStaticMesh* mesh_asset = (UStaticMesh*) StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Models/ceiling.ceiling'"));

	if (mesh_asset) {
		ceiling_mesh->SetStaticMesh(mesh_asset);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed to create mesh asset"));
	}

	ceiling_mesh->RegisterComponent();

	if (RootComponent) {
		ceiling_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not attaching wall"));
	}
	ceiling_mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 275.0f), false);

	ceiling_mesh->AddLocalRotation(FRotator(180.0f, 0.0, 0.0f), false);
}

void ARoom::LoadLight()
{
	auto light = NewObject<UPointLightComponent>(this, UPointLightComponent::StaticClass(), TEXT("room light"));
	meshes.Add(light);

	light->RegisterComponent();

	if (RootComponent) {
		light->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	light->SetRelativeLocation(FVector(0.0f, 0.0f, 235.0f), false);
}

void ARoom::Initialize(RoomBlock const& block)
{
	LoadWall(FVector(495.0f, 0.0f, 137.5f), FRotator(0.0f, 180.0f, 0.0f), "right wall", block.walls[1]);
	LoadWall(FVector(0.0f, 495.0f, 137.5f), FRotator(0.0f, -90.0f, 0.0f), "front wall", block.walls[0]);
	LoadWall(FVector(-495.0f, 0.0f, 137.5f), FRotator(0.0f, 0.0f, 0.0f), "left wall", block.walls[3]);
	LoadWall(FVector(0.0f, -495.0f, 137.5f), FRotator(0.0f, 90.0f, 0.0f), "back wall", block.walls[2]);

	LoadCeiling();
	LoadLight();
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
