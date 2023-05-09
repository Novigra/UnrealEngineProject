// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "MyPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPlacement.h"

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
	bCanEnemyChoose = false;
	bTie = false;
	bCanChooseWinner = true;
	RoundEnemyWins = 0;
	InterpSpeed = 800.0f;
	bEnemyPushBack = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();

	EnemyCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	EnemyCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (MyPlayer)
	{
		if (MyPlayer->MatchRound <= 3)
		{
			bCanEnemyChoose = MyPlayer->GetTimeCondition();
		}

		if (bCanEnemyChoose)
		{
			EnemyPlay();
			ComparePlay();

			if (MyPlayer->MatchRound <= 3)
			{
				NextRound();
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

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor)
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		if (MyPlayer)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player insight"));
		}
	}*/
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player outsight"));*/
}

void AEnemy::EnemyPlay()
{
	int RandomNumber = (int)FMath::FRandRange(1.0, 4.0);

	if (RandomNumber == 1)
	{
		EnemyChoice = EEnemyChoice::EEC_Rock;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Rock"));
	}
	else if (RandomNumber == 2)
	{
		EnemyChoice = EEnemyChoice::EEC_Paper;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Paper"));
	}
	else
	{
		EnemyChoice = EEnemyChoice::EEC_Scissors;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Chose Scissors"));
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
	MyPlayer->MatchTimer = 1;
	MyPlayer->bCanplayerchoose = true;
	MyPlayer->bStopTimer = false;
	if (!(bTie))
	{
		MyPlayer->MatchRound++;
	}
	bTie = false;
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