// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPCGameModeBase.h"


ARPCGameModeBase::ARPCGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARPCGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARPCGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}