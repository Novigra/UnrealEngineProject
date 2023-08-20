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
}

void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer->OnPlayerDestination.AddDynamic(this, &AShotgunWeapon::SetupWeapon);

	MyPlayer->OnStartShooting.AddDynamic(this, &AShotgunWeapon::StartShooting);
	MyPlayer->OnStopShooting.AddDynamic(this, &AShotgunWeapon::StopShooting);
}

// Called every frame
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShotgunWeapon::SetupWeapon()
{
	Super::SetupWeapon();

	if (MyPlayer)
	{
		if (PlayerWeapon == EPlayerWeapon::EPW_Shotgun)
		{
			MyPlayer->SetEquippedWeapon(this);

			const USkeletalMeshSocket* LeftHandSocket = MyPlayer->GetMesh()->GetSocketByName("L_palmSocket");
			if (LeftHandSocket)
			{
				LeftHandSocket->AttachActor(this, MyPlayer->GetMesh());
			}
		}
		else
		{
			Enemy->SetEnemyWeapon(this);

			const USkeletalMeshSocket* LeftHandSocket = Enemy->GetMesh()->GetSocketByName("LeftHandSocket");
			if (LeftHandSocket)
			{
				LeftHandSocket->AttachActor(this, Enemy->GetMesh());
			}
		}
	}

	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SkeletalMesh->SetSimulatePhysics(false);
}

void AShotgunWeapon::StartShooting()
{
	Super::StartShooting();


}

void AShotgunWeapon::StopShooting()
{
	Super::StopShooting();


}

void AShotgunWeapon::Shoot()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Yup, that's a child class! (Shotgun)"));
}