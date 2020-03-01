// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SecretBaseGeneratorGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SECRETBASEGENERATOR_API ASecretBaseGeneratorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ASecretBaseGeneratorGameModeBase();

	virtual void StartPlay() override;

};
