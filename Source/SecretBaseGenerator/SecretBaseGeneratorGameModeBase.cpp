// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SecretBaseGeneratorGameModeBase.h"
#include "PlayerCharacter.h"

ASecretBaseGeneratorGameModeBase::ASecretBaseGeneratorGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    DefaultPawnClass = APlayerCharacter::StaticClass();
}

void ASecretBaseGeneratorGameModeBase::StartPlay()
{
    Super::StartPlay();

    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello!"));
    }
}
