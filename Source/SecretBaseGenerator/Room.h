// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"


enum class WALL_TYPE
{
	WALL,
	DOOR
};

typedef struct RoomBlock
{
	float x = 0.0f;
	float y = 0.0f;
	WALL_TYPE walls[4] = {WALL_TYPE::WALL};

	RoomBlock(float x_val, float y_val)
	{
		x = x_val;
		y = y_val;
	}

	bool operator==(RoomBlock const& right) const
	{
		return (x == right.x && y == right.y);
	}

} RoomBlock;

struct RoomBlockSetLessThan
{
	bool operator()(RoomBlock const& left, RoomBlock const& right) const
	{
		return (left.x < right.x || left.y < right.y);
	}
};


UCLASS()
class SECRETBASEGENERATOR_API ARoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoom();

	void Initialize(RoomBlock const& block);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void LoadWall(FVector loc, FRotator rotation, FName name, WALL_TYPE const type);
	void LoadCeiling();
	void LoadLight();

	TArray<USceneComponent*> meshes;
};
