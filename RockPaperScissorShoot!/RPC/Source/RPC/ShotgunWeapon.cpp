// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "MyPlayer.h"
#include "Engine/SkeletalMeshSocket.h"

AShotgunWeapon::AShotgunWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Damage = 45;
	Bullets = 6;
	Mag = 6;

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

	if (PlayerWeapon == EPlayerWeapon::EPW_Shotgun)
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
}

void AShotgunWeapon::SetupWeapon()
{
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalMesh->SetSimulatePhysics(false);

	if (PlayerWeapon == EPlayerWeapon::EPW_Shotgun)
	{
		const USkeletalMeshSocket* LeftHandSocket = MyPlayer->GetMesh()->GetSocketByName("L_palmSocket");
		if (LeftHandSocket)
		{
			LeftHandSocket->AttachActor(this, MyPlayer->GetMesh());
		}
	}
	
}