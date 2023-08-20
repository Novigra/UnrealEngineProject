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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDestination);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartShooting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopShooting);

UCLASS()
class RPC_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDestination OnPlayerDestination;

	UPROPERTY(BlueprintAssignable)
	FOnStartShooting OnStartShooting;

	UPROPERTY(BlueprintAssignable)
	FOnStopShooting OnStopShooting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class APlayerCameraManager* CameraManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	class APlayerController* PlayerControllerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh)
	class UStaticMeshComponent* StaticRootMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FVector InitialCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float InitialTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float ZoomedValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float ZoomRate;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	FVector RpsCamera;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	FVector InitialMeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	FVector RpsMeshLocation;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interpolation)
	bool bPushBack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interpolation)
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interpolation)
	class APlacement* Placement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class ASpawnBullet* SpawnBullet;

	/*
	* Combat related variables
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperties")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties|Speed")
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperties|Speed")
	float WinnerSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperties|Speed")
	float LoserSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperties|Speed")
	float DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties|Speed|Dash")
	FVector PlayerDashDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerProperties|Speed|Dash")
	float PlayerDashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerProperties|Speed|Dash")
	bool bCanDash;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerProperties")
	bool bCanPlayerShoot;

	bool bToggleLog;
	bool bToggleMeshLoc;

	float Zoom;
	float InitialZoom;
	bool bTriggerZooming;
	bool bTriggerZoomingOut;

	bool bIsPressed;

	/*
	* Functions implementation is written in the same order to avoid confusion
	*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	// Set Character Speed
	void SetCharacterSpeed();
	void SetCharacterDash();
	
	// Movement Speed
	void Dash();

	void PlayerRock();
	void PlayerPaper();
	void PlayerScissors();

	void StartShoot();
	void StopShoot();

	void Reload();

	void Aim_Pressed();
	void Aim_Released();

	void ZoomIn(float DeltaTime);
	void ZoomOut(float DeltaTime);

	void LoadActors();
	void MeshModification();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HealthMechanics();

	// Functions (FOR DEBUGGING PURPOSES)
	void ChangeStatus(); // Change Player Status
	void RestartTime(); // RestartTimer
	void RandomNumber();
	void CheckShooting();
	void CheckHealth();

	FORCEINLINE bool GetTimeCondition() { return bStopTimer; }
	FORCEINLINE EPlayerStatus GetPlayerStatus() { return PlayerStatus; }

	FORCEINLINE void SetEquippedWeapon(AWeapon* MyWeapon) { EquippedWeapon = MyWeapon; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
};
