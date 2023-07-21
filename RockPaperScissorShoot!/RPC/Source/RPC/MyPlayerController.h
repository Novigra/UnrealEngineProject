// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPC_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDOverlay;

	class AMyPlayer* MyPlayer;
	bool bMouseToggle;

	APlayerController* PlayerController;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
};
