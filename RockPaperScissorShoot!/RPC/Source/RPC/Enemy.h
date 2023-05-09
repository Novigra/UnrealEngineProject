// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

// Enemy Status depends on whether the Enemy is in the (Rock, Paper, Scissors) match or in the fight.
UENUM(BlueprintType)
enum class EEnemyStatus : uint8
{
	EES_Match		UMETA(DisplayName = "Match"),
	EES_Pending		UMETA(DisplayName = "Pending"),
	EES_Fight		UMETA(DisplayName = "Fight")
};

UENUM(BlueprintType)
enum class EEnemyChoice : uint8
{
	EEC_Rock		UMETA(DisplayName = "Rock"),
	EEC_Paper		UMETA(DisplayName = "Paper"),
	EEC_Scissors	UMETA(DisplayName = "Scissors"),

	EEC_NONE		UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EEnemyResult : uint8
{
	EER_Winner	UMETA(DisplayName = "Winner"),
	EER_Loser	UMETA(DisplayName = "Loser"),

	EER_NONE	UMETA(DisplayName = "NONE")
};

UCLASS()
class RPC_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EEnemyStatus EnemyStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EEnemyChoice EnemyChoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EEnemyResult EnemyResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Match)
	class AMyPlayer* MyPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent* EnemyCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Match)
	bool bCanEnemyChoose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	int32 RoundEnemyWins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	AEnemy* testEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interpolation)
	bool bEnemyPushBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	class AEnemyPlacement* EnemyPlacement;

	bool bTie;
	bool bCanChooseWinner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EnemyPlay();
	void ComparePlay();
	void NextRound();
	void ChooseWinner();
	void SwitchModes();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
