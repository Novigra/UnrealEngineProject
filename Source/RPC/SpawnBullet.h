// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnBullet.generated.h"

UCLASS()
class RPC_API ASpawnBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnBullet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectileMovement)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow)
	class UArrowComponent* BulletForwardMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class UCapsuleComponent* BulletCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMeshComponent* EmptyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class AMyPlayer* MyPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	class AWeapon* PlayerWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	class AEnemy* Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Weapon")
	class AWeapon* EnemyWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement|PlayerPlacement")
	class APlacement* PlayerPlacement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placement|EnemyPlacement")
	class AEnemyPlacement* EnemyPlacement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BulletProperties)
	float BulletSpeed;

	float BulletDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BulletProperties)
	AActor* BulletOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void BindPlayerWeapon();

	UFUNCTION()
		void BindEnemyWeapon();

	UFUNCTION()
		void DamagePlayer();

	UFUNCTION()
		void DamageEnemy();

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }
};
