// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBullet.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Placement.h"
#include "EnemyPlacement.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnBullet::ASpawnBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BulletCollision"));
	BulletCollision->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BulletCollision);

	BulletSpeed = 10.0f;
}

// Called when the game starts or when spawned
void ASpawnBullet::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();
	
	//SetupPlacement();

	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnBullet::OnOverlapBegin);
	BulletCollision->OnComponentEndOverlap.AddDynamic(this, &ASpawnBullet::OnOverlapEnd);
}

// Called every frame
void ASpawnBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ASpawnBullet::SetupPlacement()
//{
//	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();
//
//	AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(this, AEnemy::StaticClass());
//	Enemy = Cast<AEnemy>(FoundEnemy);
//
//	if (MyPlayer)
//	{
//		MyPlayer->OnPlayerDestination.AddDynamic(this, &ASpawnBullet::BindPlayerWeapon);
//	}
//
//	if (Enemy)
//	{
//		Enemy->OnEnemyDestination.AddDynamic(this, &ASpawnBullet::BindEnemyWeapon);
//	}
//}

void ASpawnBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->IsA(AMyPlayer::StaticClass()))
		{
			AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(this, AEnemy::StaticClass());
			Enemy = Cast<AEnemy>(FoundEnemy);
			EnemyWeapon = Enemy->GetEnemyWeapon();

			if (EnemyWeapon)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rifle Damage: %d"), EnemyWeapon->Damage));

				DamagePlayer();
			}
		}
		else if(OtherActor->IsA(AEnemy::StaticClass()))
		{
			//AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(this, AEnemy::StaticClass());
			Enemy = Cast<AEnemy>(OtherActor);

			if (OtherComp == Enemy->GetCapsuleComponent())
			{
				PlayerWeapon = MyPlayer->GetEquippedWeapon();

				if (PlayerWeapon)
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Rifle Damage: %d"), PlayerWeapon->Damage));

					DamageEnemy();
				}
			}
		}
	}
}

void ASpawnBullet::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ASpawnBullet::BindPlayerWeapon()
{
	PlayerWeapon = MyPlayer->GetEquippedWeapon();
}

void ASpawnBullet::BindEnemyWeapon()
{
	EnemyWeapon = Enemy->GetEnemyWeapon();
}

void ASpawnBullet::DamagePlayer()
{
	MyPlayer->Health -= EnemyWeapon->Damage;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Health: %d"), int32(MyPlayer->Health)));
}

void ASpawnBullet::DamageEnemy()
{
	Enemy->Health -= PlayerWeapon->Damage;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Enemy Health: %d"), int32(Enemy->Health)));
}