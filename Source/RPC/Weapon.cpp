// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(SkeletalMesh);

	BulletSpawnTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletSpawnTransform"));
	BulletSpawnTransform->SetupAttachment(SkeletalMesh);

	bMyTest = true;
	PlayerWeapon = EPlayerWeapon::EPW_NONE;

	Damage = 1;
	Bullets = 1;
	Mag = 1;
	FireRate = 1.0;
	FireTime = 0.0;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	MyPlayer = UGameplayStatics::GetPlayerController(this,0)->GetPawn<AMyPlayer>();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(this, AEnemy::StaticClass());
	Enemy = Cast<AEnemy>(FoundActor);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//UE_LOG(LogTemp, Warning, TEXT("FireTime = %f"), FireTime);

	CurrentSpawnerLocation = BulletSpawnTransform->GetRelativeLocation();
	
	if (MyPlayer)
	{
		if (MyPlayer->PlayerResult == EPlayerResult::EPR_Winner)
		{
			PlayerWeapon = EPlayerWeapon::EPW_Shotgun;
			
		}
		else if (MyPlayer->PlayerResult == EPlayerResult::EPR_Loser)
		{
			PlayerWeapon = EPlayerWeapon::EPW_Rifle;
		}

		if (MyPlayer->bIsPressed)
		{
			FireTime += (FireRate * DeltaTime);
			UE_LOG(LogTemp, Warning, TEXT("FireTime = %f"), FireTime);

			if (FireTime > 10)
			{
				FireTime = 0.0f;
				if (Bullets != 0)
				{
					MyPlayer->bCanPlayerShoot = true;
				}
			}

			if (MyPlayer->bCanPlayerShoot)
			{
				if (Bullets != 0)
				{
					Bullets--;
				}
			}
		}
	}
}