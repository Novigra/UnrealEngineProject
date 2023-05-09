// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class RPC_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interpolation | Location")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interpolation | Location")
	float InterpSpeedLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interpolation | Rotation")
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interpolation | Rotation")
	float InterpSpeedRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	class AMyPlayer* MyPlayer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
