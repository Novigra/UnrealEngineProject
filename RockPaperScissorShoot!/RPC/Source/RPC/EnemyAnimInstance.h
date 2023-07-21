// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPC_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Motion)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Motion)
	class AEnemy* Enemy;
};
