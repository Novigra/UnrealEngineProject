// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
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

	bMyTest = true;
	PlayerWeapon = EPlayerWeapon::EPW_NONE;
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
	
	if (MyPlayer)
	{
		if (MyPlayer->PlayerResult == EPlayerResult::EPR_Loser)
		{
			PlayerWeapon = EPlayerWeapon::EPW_Shotgun;
			
		}
	}
}