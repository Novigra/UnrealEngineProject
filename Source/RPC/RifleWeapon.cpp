// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Components/ArrowComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ARifleWeapon::ARifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Damage = 25;
	Bullets = 25;
	Mag = 25;
	FireRate = 0.3f;
}

void ARifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer->OnPlayerDestination.AddDynamic(this, &ARifleWeapon::SetupWeapon);

	MyPlayer->OnStartShooting.AddDynamic(this, &ARifleWeapon::StartShooting);
	MyPlayer->OnStopShooting.AddDynamic(this, &ARifleWeapon::StopShooting);

	CurrentLevel = GetWorld();
}

void ARifleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ARifleWeapon::SetupWeapon()
{
	Super::SetupWeapon();

	if (MyPlayer)
	{
		if (PlayerWeapon == EPlayerWeapon::EPW_Rifle)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player should equip the weapon"));

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

void ARifleWeapon::StartShooting()
{
	Super::StartShooting();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARifleWeapon::Shoot, FireRate, true, 0.0f);
}

void ARifleWeapon::StopShooting()
{
	Super::StopShooting();

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ARifleWeapon::Shoot()
{
	if (SpawnBullet && PlayerWeapon == EPlayerWeapon::EPW_Rifle)
	{
		if (Bullets != 0)
		{
			PlayerCameraManager = MyPlayer->CameraManager;

			if (CurrentLevel)
			{
				FHitResult HitResult;

				FVector Start = PlayerCameraManager->GetCameraLocation();
				FVector End = Start + (PlayerCameraManager->GetActorForwardVector() * BulletEndDistance);

				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(MyPlayer);

				CurrentLevel->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Camera, QueryParams);

				if (bToggleDebugLine)
					DrawDebugLine(CurrentLevel, Start, End, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 2.0f);

				FVector TraceEnd = HitResult.TraceEnd;
				FVector BulletSpawnVector = BulletSpawnTransform->GetComponentLocation();
				FVector BulletDirection = UKismetMathLibrary::GetDirectionUnitVector(BulletSpawnVector, TraceEnd);
				FVector EndLocation = (BulletDirection * BulletSpeed);

				ASpawnBullet* SpawnedBullet = CurrentLevel->SpawnActor<ASpawnBullet>(SpawnBullet, BulletSpawnVector, FRotator(0.0f));

				SpawnedBullet->GetProjectileMovement()->SetVelocityInLocalSpace(EndLocation);

				Bullets--;

				if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
				{
					UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *HitResult.GetActor()->GetName());
				}
				else 
				{
					UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
				}
			}
		}
		else
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnBullet is invalid"));
	}

	
}