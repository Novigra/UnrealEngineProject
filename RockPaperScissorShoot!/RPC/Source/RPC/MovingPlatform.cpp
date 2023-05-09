// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	TargetLocation = FVector(GetActorLocation());
	TargetRotation = FRotator(GetActorRotation());
	InterpSpeedLocation = 10.0f;
	InterpSpeedRotation = 10.0f;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyPlayer)
	{
		if ((MyPlayer->GetPlayerStatus() == EPlayerStatus::EPS_Pending) || (MyPlayer->GetPlayerStatus() == EPlayerStatus::EPS_Fight))
		{
			FVector CurrentLocation = GetActorLocation();
			FVector InterpLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeedLocation);
			SetActorLocation(InterpLocation);

			FRotator CurrentRotation = GetActorRotation();
			FRotator InterpRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeedRotation);
			SetActorRotation(InterpRotation);
		}
	}
}

