// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

// Sets default values
ARoom::ARoom()
	:AActor()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("room mesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh_asset(TEXT("StaticMesh'/Game/Models/room.room'"));

	if (mesh_asset.Succeeded()) {
		mesh->SetStaticMesh(mesh_asset.Object);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed to create mesh asset"));
	}
	RootComponent = mesh;
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
