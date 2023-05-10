// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GenericPlatform/ICursor.h"
#include "Enemy.h"
#include "Placement.h"
#include "Weapon.h"
#include "RifleWeapon.h"
#include "ShotgunWeapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 10.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	InitialCamera = Camera->GetRelativeLocation();

	// Attach Mesh to Camera Boom so it follows the camera Movement
	GetMesh()->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	InitialMesh = GetMesh()->GetRelativeLocation();

	// Set Capsule Size
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 40.0f);

	// Character Movement
	SpeedNormal = 0.0f;			// Set Speed Value (In Normal State)
	SpeedFast = 0.0f;			// Set Speed Value (In Sprint State)

	// Player initial status
	PlayerStatus = EPlayerStatus::EPS_Match;
	PlayerChoice = EPlayerChoice::EPC_NONE;
	PlayerResult = EPlayerResult::EPR_NONE;
	PlayerAnimTrans = EPlayerAnimTrans::EPAT_NONE;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Player Initial Status Is Match"));

	RoundPlayerWins = 0;
	MatchRound = 1;
	MatchTimer = 1;
	MatchTimerRate = 1;
	
	bCanplayerchoose = true;
	bStopTimer = false;
	
	bPushBack = false;
	InterpSpeed = 100.0f;

	bToggleLog = true;
	bToggleEquip = true;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	LoadActors();
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Match Timer
	if (!(bStopTimer))
	{
		MatchTimer += (MatchTimerRate * DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Match Timer = %f"), MatchTimer);
	}
	
	if (MatchTimer > 6)
	{
		bCanplayerchoose = false;
		bStopTimer = true;

		/*if (PlayerChoice == EPlayerChoice::EPC_NONE && bToggleLog)
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYER CHOSE : NONE"));
			bToggleLog = false;
		}
		else if (!(PlayerChoice == EPlayerChoice::EPC_NONE) && (bToggleLog))
		{
			UE_LOG(LogTemp, Warning, TEXT("PLAYER CHOSE : (Action)"));
			bToggleLog = false;
		}*/

	}

	// Push Player to the actor after the match
	if (Placement)
	{
		if (bPushBack)
		{
			FVector CurrentLocation = GetActorLocation();
			FVector TargetLocation = Placement->GetPlacementLocation();
			FVector InterpLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
			SetActorLocation(InterpLocation);
		}
	}

	// Equip Weapon
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		if (ShotgunWeapon)
		{
			if (ShotgunWeapon->Weapon && bToggleEquip)
			{
				SetEquippedWeapon(ShotgunWeapon);
				bToggleEquip = false;
			}
		}

		if (RifleWeapon)
		{
			if (RifleWeapon->Weapon && bToggleEquip)
			{
				SetEquippedWeapon(RifleWeapon);
				bToggleEquip = false;
			}
		}
	}


}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward/BackwardMovement", this, &AMyPlayer::FBMovement);
	PlayerInputComponent->BindAxis("Right/LeftMovement", this, &AMyPlayer::RLMovement);

	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::CameraPitchRotation);
	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::CameraYawRotation);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPlayer::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPlayer::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyPlayer::StopSprinting);

	PlayerInputComponent->BindAction("PlayerRock", IE_Pressed, this, &AMyPlayer::PlayerRock);
	PlayerInputComponent->BindAction("PlayerPaper", IE_Pressed, this, &AMyPlayer::PlayerPaper);
	PlayerInputComponent->BindAction("PlayerScissors", IE_Pressed, this, &AMyPlayer::PlayerScissors);

	PlayerInputComponent->BindAction("TestInput", IE_Pressed, this, &AMyPlayer::ChangeStatus);
	PlayerInputComponent->BindAction("RestartTimer", IE_Pressed, this, &AMyPlayer::RestartTime);
	PlayerInputComponent->BindAction("Random", IE_Pressed, this, &AMyPlayer::RandomNumber);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMyPlayer::Shoot);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &AMyPlayer::Aim_Pressed);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &AMyPlayer::Aim_Released);
}

// Forward / BackWard Movement
void AMyPlayer::FBMovement(float value)
{
	if (Controller != nullptr && value != 0.0f && (PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

// Right / Left Movement
void AMyPlayer::RLMovement(float value)
{
	if (Controller != nullptr && value != 0.0f && (PlayerStatus == EPlayerStatus::EPS_Fight))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

// Set Camera Rotation
void AMyPlayer::CameraPitchRotation(float value)
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		APawn::AddControllerPitchInput(value);
	}
}

void AMyPlayer::CameraYawRotation(float value)
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		APawn::AddControllerYawInput(value);
	}
}

void AMyPlayer::Jump()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		ACharacter::Jump();
	}
}

void AMyPlayer::StopJumping()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		ACharacter::StopJumping();
	}
}


// Sprint State
void AMyPlayer::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SpeedFast;
}

// Normal State
void AMyPlayer::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = SpeedNormal;
}

void AMyPlayer::PlayerRock()
{
	if ((PlayerStatus == EPlayerStatus::EPS_Match) && (bCanplayerchoose))
	{
		PlayerChoice = EPlayerChoice::EPC_Rock;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("PLAYER CHOSE : ROCK"));
		
		bCanplayerchoose = false;
	}
}

void AMyPlayer::PlayerPaper()
{
	if ((PlayerStatus == EPlayerStatus::EPS_Match) && (bCanplayerchoose))
	{
		PlayerChoice = EPlayerChoice::EPC_Paper;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("PLAYER CHOSE : PAPER"));

		bCanplayerchoose = false;
	}
}

void AMyPlayer::PlayerScissors()
{
	if ((PlayerStatus == EPlayerStatus::EPS_Match) && (bCanplayerchoose))
	{
		PlayerChoice = EPlayerChoice::EPC_Scissors;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("PLAYER CHOSE : SCISSORS"));

		bCanplayerchoose = false;
	}
}

void AMyPlayer::Shoot()
{

}

void AMyPlayer::Aim_Pressed()
{

}

void AMyPlayer::Aim_Released()
{

}

void AMyPlayer::LoadActors()
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(this, APlacement::StaticClass());
	Placement = Cast<APlacement>(FoundActor);

	AActor* FoundWeapon = UGameplayStatics::GetActorOfClass(this, ARifleWeapon::StaticClass());
	RifleWeapon = Cast<ARifleWeapon>(FoundWeapon);

	FoundWeapon = UGameplayStatics::GetActorOfClass(this, AShotgunWeapon::StaticClass());
	ShotgunWeapon = Cast<AShotgunWeapon>(FoundWeapon);
}

void AMyPlayer::ChangeStatus()
{
	if (PlayerStatus == EPlayerStatus::EPS_Match)
	{
		PlayerStatus = EPlayerStatus::EPS_Fight;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Player Status Is Fight"));
	}
	else
	{
		PlayerStatus = EPlayerStatus::EPS_Match;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Player Status Is Match"));
	}
}

void AMyPlayer::RestartTime()
{
	bCanplayerchoose = true;
	MatchTimer = 1;
}

void AMyPlayer::RandomNumber()
{
	int myRandomNumber = (int)FMath::FRandRange(1.0, 4.0);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Random Number = %d"), myRandomNumber));
}