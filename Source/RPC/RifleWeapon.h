// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPC_API ARifleWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:

	ARifleWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	ARifleWeapon* Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupWeapon() override;

	virtual void StartShooting() override;

	virtual void StopShooting() override;

	virtual void Shoot() override;

	FORCEINLINE void SetWeapon(ARifleWeapon* MyWeapon) { Weapon = MyWeapon; }
	FORCEINLINE ARifleWeapon* GetWeapon() { return Weapon; }
};
