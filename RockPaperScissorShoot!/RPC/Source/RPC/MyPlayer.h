// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"

// Player Status depends on whether the player is in the (Rock, Paper, Scissors) match or in the fight.
UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	EPS_Match		UMETA(DisplayName = "Match"),
	EPS_Pending		UMETA(DisplayName = "Pending"),
	EPS_Fight		UMETA(DisplayName = "Fight")
};

UENUM(BlueprintType)
enum class EPlayerChoice : uint8
{
	EPC_Rock		UMETA(DisplayName = "Rock"),
	EPC_Paper		UMETA(DisplayName = "Paper"),
	EPC_Scissors	UMETA(DisplayName = "Scissors"),

	EPC_NONE		UMETA(DisplayName = "NONE")
};

// Result Status depends on whether the player won the match or not.
UENUM(BlueprintType)
enum class EPlayerResult : uint8
{
	EPR_Winner	UMETA(DisplayName = "Winner"),
	EPR_Loser	UMETA(DisplayName = "Loser"),

	EPR_NONE	UMETA(DisplayName = "NONE")
};

UENUM(BlueprintType)
enum class EPlayerAnimTrans : uint8
{
	EPAT_Choosing	UMETA(DisplayName = "Choosing"),
	EPAT_Playing	UMETA(DisplayName = "Playing"),

	EPAT_NONE	UMETA(DisplayName = "NONE")
};

UCLASS()
class RPC_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EPlayerStatus PlayerStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EPlayerChoice PlayerChoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EPlayerResult PlayerResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EPlayerAnimTrans PlayerAnimTrans;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float SpeedNormal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float SpeedFast;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	int32 RoundPlayerWins;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	float MatchRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	float MatchTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match)
	float MatchTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match)
	bool bCanplayerchoose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Match)
	bool bStopTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interpolation)
	bool bPushBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interpolation)
	class APlacement* Placement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class ARifleWeapon* RifleWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AShotgunWeapon* ShotgunWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AWeapon* EquippedWeapon;

	bool bToggleLog;
	bool bToggleEquip;
	FVector InitialCamera;
	FVector InitialMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Forward / Backward Movement
	void FBMovement(float value);

	// Right / Left Movement
	void RLMovement(float value);

	void CameraPitchRotation(float value);
	void CameraYawRotation(float value);

	// Jump / Stop Jumping
	void Jump();
	void StopJumping();
	
	// Sprint Movement
	void Sprint();
	void StopSprinting();

	void PlayerRock();
	void PlayerPaper();
	void PlayerScissors();

	void Shoot();
	void Aim_Pressed();
	void Aim_Released();

	void LoadActors();

	// Functions (FOR DEBUGGING PURPOSES)
	void ChangeStatus(); // Change Player Status
	void RestartTime(); // RestartTimer
	void RandomNumber();

	FORCEINLINE bool GetTimeCondition() { return bStopTimer; }
	FORCEINLINE EPlayerStatus GetPlayerStatus() { return PlayerStatus; }

	FORCEINLINE void SetEquippedWeapon(AWeapon* MyWeapon) { EquippedWeapon = MyWeapon; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
};
