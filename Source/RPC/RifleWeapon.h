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

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Bullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Mag;*/

	bool bToggleLog;
	bool bTestEquip;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupWeapon();

	virtual void Shoot() override;

	FORCEINLINE void SetWeapon(ARifleWeapon* MyWeapon) { Weapon = MyWeapon; }
	FORCEINLINE ARifleWeapon* GetWeapon() { return Weapon; }
};
