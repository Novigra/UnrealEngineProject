// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AShotgunWeapon::AShotgunWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Damage = 45;
	Bullets = 6;
	Mag = 6;
	FireRate = 1;

	bToggleLog = true;
	bTestEquip = true;
}

void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyPlayer)
	{
		/*if (Bullets == 0)
		{
			MyPlayer->bCanPlayerShoot = false;
		}*/
	}

	/*if ((PlayerWeapon == EPlayerWeapon::EPW_Shotgun) && (MyPlayer->PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		if (bToggleLog)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Shotgun"));
			bToggleLog = false;
		}
		
		if (bTestEquip)
		{
			SetWeapon(this);
			SetupWeapon();
			bTestEquip = false;
		}
		
	}
	else if (!(PlayerWeapon == EPlayerWeapon::EPW_Shotgun) && (MyPlayer->PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		if (bTestEquip)
		{
			SetupWeapon();
			bTestEquip = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("YAAAAAAAAAA"));
		}
	}*/
}

void AShotgunWeapon::SetupWeapon()
{
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalMesh->SetSimulatePhysics(false);

	if (MyPlayer || Enemy)
	{
		if ((PlayerWeapon == EPlayerWeapon::EPW_Shotgun) && (MyPlayer->PlayerStatus == EPlayerStatus::EPS_Fight))
		{
			SetWeapon(this);
			
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
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("YOOOOOOOOOOOOOO"));
			}
		}
	}
}

void AShotgunWeapon::Shoot()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Yup, that's a child class! (Shotgun)"));
}