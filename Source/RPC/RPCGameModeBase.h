// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPCGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPC_API ARPCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARPCGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
