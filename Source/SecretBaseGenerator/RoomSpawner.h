// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "Room.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSpawner.generated.h"

UCLASS()
class SECRETBASEGENERATOR_API ARoomSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	std::vector<ARoom*> rooms;
};
