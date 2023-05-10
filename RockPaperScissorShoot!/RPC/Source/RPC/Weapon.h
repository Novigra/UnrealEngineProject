// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EPlayerWeapon : uint8
{
	EPW_Shotgun		UMETA(DisplayName = "Shotgun"),
	EPW_Rifle		UMETA(DisplayName = "Rifle"),
	
	EPW_NONE		UMETA(DisplayName = "NONE")
};

UCLASS()
class RPC_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow)
	class UArrowComponent* BulletSpawnTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	class AMyPlayer* MyPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	class AEnemy* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	class UCapsuleComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	bool bMyTest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	EPlayerWeapon PlayerWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EPlayerWeapon GetPlayerWeapon() { return PlayerWeapon; }
};
