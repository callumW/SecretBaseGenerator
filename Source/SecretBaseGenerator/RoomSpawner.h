// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <utility>
#include <set>

#include "Room.h"
#include "ESet.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 width = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 height = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 num_rooms = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 seed = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	ESet<std::pair<float, float>>  GetRoomLocations();

	TArray<ARoom*> rooms;
};
