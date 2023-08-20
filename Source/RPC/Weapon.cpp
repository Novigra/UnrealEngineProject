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

	bShoot = false;

	BulletEndDistance = 10000.0f;
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

	CurrentSpawnerLocation = BulletSpawnTransform->GetRelativeLocation();
	
}

void AWeapon::SetupWeapon()
{
	if (MyPlayer)
	{
		if (MyPlayer->PlayerResult == EPlayerResult::EPR_Winner)
		{
			PlayerWeapon = EPlayerWeapon::EPW_Shotgun;

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Is A Winner And The Weapon Is A Shotgun"));
		}
		else if (MyPlayer->PlayerResult == EPlayerResult::EPR_Loser)
		{
			PlayerWeapon = EPlayerWeapon::EPW_Rifle;

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Is A Loser And The Weapon Is A Rifle"));
		}
	}
}

void AWeapon::StartShooting()
{
	bShoot = true;
}

void AWeapon::StopShooting()
{
	bShoot = false;
}

void AWeapon::Shoot(){}