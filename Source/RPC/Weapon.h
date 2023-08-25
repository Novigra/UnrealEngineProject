// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBullet.h"
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
	EPlayerWeapon PlayerWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Bullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Mag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	float BulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	TSubclassOf<ASpawnBullet> SpawnBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	TArray<TSubclassOf<ASpawnBullet>> SpawnBulletArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	TArray<FRotator> BulletDirectionRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponProperties)
	bool bShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	float BulletEndDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	bool bToggleDebugLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class APlayerCameraManager* PlayerCameraManager;

	FVector CurrentSpawnerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AMeshRotation)
	FRotator SeeRotation;

	UWorld* CurrentLevel;

	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupWeapon();

	UFUNCTION()
	virtual void StartShooting();

	UFUNCTION()
	virtual void StopShooting();

	UFUNCTION()
	virtual void Shoot();

	EPlayerWeapon GetPlayerWeapon() { return PlayerWeapon; }
};
