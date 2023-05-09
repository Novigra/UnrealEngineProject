// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RPC_API AShotgunWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:

	AShotgunWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	AShotgunWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Bullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponProperties)
	int32 Mag;

	bool bToggleLog;
	bool bTestEquip;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupWeapon();

	FORCEINLINE void SetWeapon(AShotgunWeapon* MyWeapon) { Weapon = MyWeapon; }
	FORCEINLINE AShotgunWeapon* GetWeapon() { return Weapon; }

};
