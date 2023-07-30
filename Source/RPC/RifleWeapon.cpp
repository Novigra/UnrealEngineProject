// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"


ARifleWeapon::ARifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Damage = 25;
	Bullets = 25;
	Mag = 25;
	FireRate = 10;

	bToggleLog = true;
	bTestEquip = true;
}

void ARifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyPlayer)
	{
		/*if (Bullets < 0)
		{
			MyPlayer->bCanPlayerShoot = false;
		}*/	
	}

	if ((PlayerWeapon == EPlayerWeapon::EPW_Rifle) && (MyPlayer->PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		if (bToggleLog)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Rifle"));
			bToggleLog = false;
		}

		if (bTestEquip)
		{
			SetWeapon(this);
			SetupWeapon();
			bTestEquip = false;
		}

	}
	else if (!(PlayerWeapon == EPlayerWeapon::EPW_Rifle) && (MyPlayer->PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		if (bTestEquip)
		{
			SetupWeapon();
			bTestEquip = false;
		}
	}

}

void ARifleWeapon::SetupWeapon()
{
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalMesh->SetSimulatePhysics(false);

	if (PlayerWeapon == EPlayerWeapon::EPW_Rifle)
	{
		const USkeletalMeshSocket* LeftHandSocket = MyPlayer->GetMesh()->GetSocketByName("L_palmSocket");
		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(this, MyPlayer->GetMesh());
		}
	}
	else
	{
		const USkeletalMeshSocket* LeftHandSocket = Enemy->GetMesh()->GetSocketByName("LeftHandSocket");
		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(this, Enemy->GetMesh());
		}
	}

}