// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();

	bMouseToggle = false;
}

void AMyPlayerController::Tick(float DeltaTime)
{
	if (!bMouseToggle)
	{
		if (MyPlayer)
		{
			if (MyPlayer->GetPlayerStatus() == EPlayerStatus::EPS_Pending)
			{
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
				
				bMouseToggle = true;
			}
		}
	}
}