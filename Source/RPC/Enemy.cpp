// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayer.h"
#include "SpawnBullet.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPlacement.h"
#include "RifleWeapon.h"
#include "ShotgunWeapon.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyCollision"));
	EnemyCollision->SetupAttachment(GetRootComponent());

	EnemyStatus = EEnemyStatus::EES_Match;
	EnemyChoice = EEnemyChoice::EEC_NONE;
	EnemyResult = EEnemyResult::EER_NONE;

	RandomNumber = 0;

	bCanEnemyChoose = false;
	bTie = false;
	bCanChooseWinner = true;
	RoundEnemyWins = 0;
	InterpSpeed = 800.0f;
	bEnemyPushBack = false;
	bToggleEquip = true;

	bTempA = true;
	bTempB = false;

	PlayTime = 2.0f;
	PlayTimer = 0.0f;
	PlayTimerRate = 1.0f;

	Health = 100.0f;
	MaxHealth = 100.0f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();

	EnemyCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroOnOverlapBegin);
	EnemyCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroOnOverlapEnd);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::EnemyOnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemy::EnemyOnOverlapEnd);

	LoadActors();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (MyPlayer)
	{
		// DON'T FORGET TO CHANGE EPS TO EES
		if (EnemyStatus == EEnemyStatus::EES_Match)
		{
			if (MyPlayer->MatchRound <= 3)
			{
				bCanEnemyChoose = MyPlayer->GetTimeCondition();
			}

			if (bCanEnemyChoose)
			{
				if (bTempA)
				{
					EnemyPlay();
					ComparePlay();
					bTempA = false;
				}


				if (MyPlayer->MatchRound <= 3)
				{
					PlayTimer += (PlayTimerRate * DeltaTime);

					if (PlayTimer >= PlayTime)
					{
						NextRound();
					}
				}
			}

			if (MyPlayer->MatchRound == 4)
			{
				if (bCanChooseWinner)
				{
					ChooseWinner();
					SwitchModes();
				}
				bCanChooseWinner = false;
			}
		}
		

		/*if (EnemyStatus == EEnemyStatus::EES_Fight)
		{
			if (ShotgunWeapon)
			{
				if ((MyPlayer->GetEquippedWeapon() == ShotgunWeapon) && bToggleEquip)
				{
					SetEnemyWeapon(RifleWeapon);
					bToggleEquip = false;
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy has rifle"));
				}
			}

			if (RifleWeapon)
			{
				if ((MyPlayer->GetEquippedWeapon() == RifleWeapon) && bToggleEquip)
				{
					SetEnemyWeapon(ShotgunWeapon);
					bToggleEquip = false;
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy has shotgun"));
				}
			}
		}*/
	}

	if (EnemyPlacement)
	{
		if (bEnemyPushBack)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector TargetLocation = EnemyPlacement->GetEnemyPlacementLocation();
			FVector InterpLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
			SetActorLocation(InterpLocation);
		}
	}

	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->IsA(AEnemyPlacement::StaticClass()) && (EnemyStatus == EEnemyStatus::EES_Pending))
		{
			OnEnemyDestination.Broadcast();

			if ((EnemyResult == EEnemyResult::EER_Winner) && (EnemyStatus == EEnemyStatus::EES_Pending))
			{
				ShotgunWeapon->SetupWeapon();
			}
			else if((EnemyResult == EEnemyResult::EER_Loser) && (EnemyStatus == EEnemyStatus::EES_Pending))
			{
				RifleWeapon->SetupWeapon();
			}

			if (EnemyStatus == EEnemyStatus::EES_Fight)
			{
				if (ShotgunWeapon)
				{
					if ((MyPlayer->GetEquippedWeapon() == ShotgunWeapon) && bToggleEquip)
					{
						SetEnemyWeapon(RifleWeapon);
						bToggleEquip = false;
						if (GEngine)
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy has rifle"));
					}
				}

				if (RifleWeapon)
				{
					if ((MyPlayer->GetEquippedWeapon() == RifleWeapon) && bToggleEquip)
					{
						SetEnemyWeapon(ShotgunWeapon);
						bToggleEquip = false;
						if (GEngine)
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy has shotgun"));
					}
				}
			}
		}
	}
}

void AEnemy::AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AEnemy::EnemyOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		SpawnBullet = Cast<ASpawnBullet>(OtherActor);
		if (SpawnBullet)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Yup, That's Your Enemy"));
			SpawnBullet->Destroy();
		}
	}
}

void AEnemy::EnemyOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::EnemyPlay()
{
	RandomNumber = (int)FMath::FRandRange(1.0, 4.0);

	if (RandomNumber == 1)
	{
		EnemyChoice = EEnemyChoice::EEC_Rock;

		if (EnemyChoice == EEnemyChoice::EEC_Rock)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Rock"));
		}
	}
	else if (RandomNumber == 2)
	{
		EnemyChoice = EEnemyChoice::EEC_Paper;

		if (EnemyChoice == EEnemyChoice::EEC_Paper)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Paper"));
		}
	}
	else
	{
		EnemyChoice = EEnemyChoice::EEC_Scissors;

		if (EnemyChoice == EEnemyChoice::EEC_Scissors)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Scissors"));
		}
	}

	bCanEnemyChoose = false;
}

void AEnemy::ComparePlay()
{
	if (MyPlayer->PlayerChoice == EPlayerChoice::EPC_NONE)
	{
		RoundEnemyWins++;
	}
	else if (MyPlayer->PlayerChoice == EPlayerChoice::EPC_Rock)
	{
		if (EnemyChoice == EEnemyChoice::EEC_Rock)
		{
			bTie = true;
		}
		else if (EnemyChoice == EEnemyChoice::EEC_Paper)
		{
			RoundEnemyWins++;
		}
		else
		{
			MyPlayer->RoundPlayerWins++;
		}
	}
	else if (MyPlayer->PlayerChoice == EPlayerChoice::EPC_Paper)
	{
		if (EnemyChoice == EEnemyChoice::EEC_Rock)
		{
			MyPlayer->RoundPlayerWins++;
		}
		else if (EnemyChoice == EEnemyChoice::EEC_Paper)
		{
			bTie = true;
		}
		else
		{
			RoundEnemyWins++;
		}
	}
	else  // If PlayerChoice = Scissors
	{
		if (EnemyChoice == EEnemyChoice::EEC_Rock)
		{
			RoundEnemyWins++;
		}
		else if (EnemyChoice == EEnemyChoice::EEC_Paper)
		{
			MyPlayer->RoundPlayerWins++;
		}
		else
		{
			bTie = true;
		}
	}
}

void AEnemy::NextRound()
{
	EnemyChoice = EEnemyChoice::EEC_NONE;

	MyPlayer->MatchTimer = 1;
	MyPlayer->bCanplayerchoose = true;
	MyPlayer->bStopTimer = false;
	MyPlayer->PlayerChoice = EPlayerChoice::EPC_NONE;
	MyPlayer->PlayerAnimTrans = EPlayerAnimTrans::EPAT_Choosing;
	if (!(bTie))
	{
		MyPlayer->MatchRound++;
	}
	bTie = false;

	bTempA = true;
	PlayTimer = 0.0f;

	RandomNumber = -1;
}

void AEnemy::ChooseWinner()
{
	if (MyPlayer->RoundPlayerWins > RoundEnemyWins)
	{
		MyPlayer->PlayerResult = EPlayerResult::EPR_Winner;
		EnemyResult = EEnemyResult::EER_Loser;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("PLAYER IS THE WINNER"));
	}
	else
	{
		MyPlayer->PlayerResult = EPlayerResult::EPR_Loser;
		EnemyResult = EEnemyResult::EER_Winner;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("ENEMY IS THE WINNER"));
	}
}

void AEnemy::SwitchModes()
{
	MyPlayer->PlayerStatus = EPlayerStatus::EPS_Pending;
	EnemyStatus = EEnemyStatus::EES_Pending;

	MyPlayer->bPushBack = true;
	bEnemyPushBack = true;
}

void AEnemy::LoadActors()
{
	AActor* FoundWeapon = UGameplayStatics::GetActorOfClass(this, ARifleWeapon::StaticClass());
	RifleWeapon = Cast<ARifleWeapon>(FoundWeapon);

	FoundWeapon = UGameplayStatics::GetActorOfClass(this, AShotgunWeapon::StaticClass());
	ShotgunWeapon = Cast<AShotgunWeapon>(FoundWeapon);
}