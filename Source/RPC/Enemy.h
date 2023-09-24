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

// Enemy life at the end of the two matches
UENUM(BlueprintType)
enum class EEnemyLife : uint8
{
	EEL_Alive	UMETA(DisplayName = "Alive"),
	EEL_Dead	UMETA(DisplayName = "Dead"),

	EEL_NONE	UMETA(DisplayName = "NONE")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDestination); 

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EEnemyLife EnemyLife;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Match)
	class AMyPlayer* MyPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent* EnemyCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match)
	class ASpawnBullet* SpawnBullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Match)
	bool bCanEnemyChoose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	int32 RoundEnemyWins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	AEnemy* testEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	int RandomNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Match)
	float PlayTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	float PlayTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match)
	float PlayTimerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interpolation)
	bool bEnemyPushBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	float InterpSpeed;

	/*
	* Combat related
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties|Health")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyProperties|Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	class AEnemyPlacement* EnemyPlacement;

	UPROPERTY(BlueprintAssignable, Category = Interpolation)
	FOnEnemyDestination OnEnemyDestination;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AWeapon* EquippedWeapon;

	/*
	* Animation related
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TempTrans)
	bool bTempA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TempTrans)
	bool bTempB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TempTrans)
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TempTrans)
	bool bIsPunching;

	bool bTie;
	bool bCanChooseWinner;
	bool bToggleEquip;
	bool bCanGetPushed;

	FTimerHandle PlayHandle;

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
	void PunchDamage();

	UFUNCTION()
	virtual void AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void EnemyOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EnemyOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE void SetEnemyWeapon(AWeapon* MyWeapon) { EquippedWeapon = MyWeapon; }
	FORCEINLINE AWeapon* GetEnemyWeapon() { return EquippedWeapon; }

	FORCEINLINE void SetEnemyChoice(EEnemyChoice myEnemyChoice) { EnemyChoice = myEnemyChoice; }
	FORCEINLINE EEnemyChoice GetEnemyChoice() { return EnemyChoice; }
};
