// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AShotgunWeapon::AShotgunWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BulletDir1 = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletDirection1"));
	BulletDir1->SetupAttachment(BulletSpawnTransform);

	Damage = 45;
	Bullets = 6;
	Mag = 6;
	FireRate = 0.3f;
}

void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer->OnPlayerDestination.AddDynamic(this, &AShotgunWeapon::SetupWeapon);

	MyPlayer->OnStartShooting.AddDynamic(this, &AShotgunWeapon::StartShooting);
	MyPlayer->OnStopShooting.AddDynamic(this, &AShotgunWeapon::StopShooting);

	CurrentLevel = GetWorld();
	SpawnBulletArr.Init(SpawnBullet, 4);
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

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AShotgunWeapon::Shoot, FireRate, true, 0.0f);
}

void AShotgunWeapon::StopShooting()
{
	Super::StopShooting();

	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AShotgunWeapon::Shoot()
{
	if (SpawnBullet && PlayerWeapon == EPlayerWeapon::EPW_Shotgun)
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

				for (auto B = SpawnBulletArr.CreateIterator(); B; ++B)
				{
					UE_LOG(LogTemp, Log, TEXT("Bullets pew pew"));

					ASpawnBullet* SpawnedBullet = CurrentLevel->SpawnActor<ASpawnBullet>(SpawnBullet, BulletSpawnVector, FRotator(0.0f));
					SpawnedBullet->GetProjectileMovement()->SetVelocityInLocalSpace(EndLocation);

					Bullets--;
				}

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