// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
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
	InitialCamera = GetMesh()->GetRelativeLocation();
	//RpsCamera = InitialCamera;

	StaticRootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticRootMesh->SetupAttachment(GetRootComponent());

	// Attach Mesh to Camera Boom so it follows the camera Movement
	GetMesh()->SetupAttachment(Camera);
	RpsMeshLocation = InitialMeshLocation;

	// Set Capsule Size
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 40.0f);

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
	bToggleMeshLoc = true;

	// Character Properties
	MaxHealth = 100.0f;
	Health = 100.0f;

	CurrentSpeed = 0.0f;

	WinnerSpeed = 800.0f;
	LoserSpeed = 400.0f;
	DashSpeed = 1000.0f;

	DashTime = 0.5f;
	DashRemainingTime = 0.0f;
	DashTimeRate = 1.0f;

	bCanDash = false;
	bSpeedSwitch = true;
	bDashSwitch = false;
	bCanPlayerShoot = false;

	Zoom = Camera->FieldOfView;
	InitialZoom = Zoom;
	ZoomRate = 1.0f;
	bTriggerZooming = false;
	bTriggerZoomingOut = false;

	bIsPressed = false;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitialMeshLocation = GetMesh()->GetRelativeLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnOverlapEnd);

	LoadActors();
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTriggerZooming)
	{
		ZoomIn(DeltaTime);
	}

	if (bTriggerZoomingOut)
	{
		ZoomOut(DeltaTime);
	}

	// Modify Mesh in runtime
	MeshModification();
	
	if (GetPlayerStatus() == EPlayerStatus::EPS_Match)
	{
		// Match Timer
		if (!(bStopTimer))
		{
			MatchTimer += (MatchTimerRate * DeltaTime);
			//UE_LOG(LogTemp, Warning, TEXT("Match Timer = %f"), MatchTimer);
		}

		if (MatchTimer > 6)
		{
			bCanplayerchoose = false;
			bStopTimer = true;

			PlayerAnimTrans = EPlayerAnimTrans::EPAT_Playing;
		}
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

	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		// Equip Weapon
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

		if (bSpeedSwitch)
		{
			SetCharacterSpeed();
			SetCharacterDash();

			bSpeedSwitch = false;
		}

		if (bCanDash)
		{
			if (bDashSwitch)
			{
				if (DashTime > DashRemainingTime)
				{
					GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
					DashRemainingTime += (DashTimeRate * DeltaTime);
				}
				else
				{
					GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
					DashRemainingTime = 0.0f;
					bDashSwitch = false;
				}
			}
		}

	}
}

void AMyPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (Placement)
		{
			OnPlayerDestination.Broadcast();
		}
	}
}

void AMyPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

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

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMyPlayer::DashState);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMyPlayer::NormalState);

	PlayerInputComponent->BindAction("PlayerRock", IE_Pressed, this, &AMyPlayer::PlayerRock);
	PlayerInputComponent->BindAction("PlayerPaper", IE_Pressed, this, &AMyPlayer::PlayerPaper);
	PlayerInputComponent->BindAction("PlayerScissors", IE_Pressed, this, &AMyPlayer::PlayerScissors);

	PlayerInputComponent->BindAction("TestInput", IE_Pressed, this, &AMyPlayer::ChangeStatus);
	PlayerInputComponent->BindAction("RestartTimer", IE_Pressed, this, &AMyPlayer::RestartTime);
	PlayerInputComponent->BindAction("Random", IE_Pressed, this, &AMyPlayer::RandomNumber);
	PlayerInputComponent->BindAction("TestShooting", IE_Pressed, this, &AMyPlayer::CheckShooting);
	PlayerInputComponent->BindAction("TestHealth", IE_Pressed, this, &AMyPlayer::CheckHealth);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMyPlayer::StartShoot);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMyPlayer::StopShoot);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyPlayer::Reload);
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

void AMyPlayer::SetCharacterSpeed()
{
	if (PlayerResult == EPlayerResult::EPR_Winner)
	{
		CurrentSpeed = WinnerSpeed;
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
	else
	{
		CurrentSpeed = LoserSpeed;
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
}

void AMyPlayer::SetCharacterDash()
{
	if (PlayerResult == EPlayerResult::EPR_Winner)
	{
		bCanDash = false;
	}
	else
	{
		bCanDash = true;
	}
}

void AMyPlayer::DashState()
{
	bDashSwitch = true;
}

void AMyPlayer::NormalState()
{

}

void AMyPlayer::PlayerRock()
{
	if ((PlayerStatus == EPlayerStatus::EPS_Match) && (bCanplayerchoose))
	{
		PlayerChoice = EPlayerChoice::EPC_Rock;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("PLAYER CHOSE : ROCK"));
		
		bCanplayerchoose = false;
		PlayerAnimTrans = EPlayerAnimTrans::EPAT_Choosing;
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
		PlayerAnimTrans = EPlayerAnimTrans::EPAT_Choosing;
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
		PlayerAnimTrans = EPlayerAnimTrans::EPAT_Choosing;
	}
}

void AMyPlayer::StartShoot()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		bIsPressed = true;
		if (EquippedWeapon->Bullets != 0)
		{
			bCanPlayerShoot = true;
		}
	}
}

//void AMyPlayer::StartShoot()
//{
//	if (PlayerStatus == EPlayerStatus::EPS_Fight)
//	{
//		if ( (EquippedWeapon->FireTime == 0.0f))
//		{
//			bCanPlayerShoot = true;
//
//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Bullets = %d"),EquippedWeapon->Bullets));
//		}
//		/*else
//		{
//			bCanPlayerShoot = false;
//		}*/
//	}
//}

void AMyPlayer::StopShoot()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		bIsPressed = false;
		bCanPlayerShoot = false;
		EquippedWeapon->FireTime = 0.0f;
	}
}

void AMyPlayer::Reload()
{
	EquippedWeapon->Bullets = EquippedWeapon->Mag;
}

void AMyPlayer::Aim_Pressed()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		bTriggerZooming = true;
		bTriggerZoomingOut = false;
	}
}

void AMyPlayer::Aim_Released()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		bTriggerZooming = false;
		bTriggerZoomingOut = true;
	}
}

void AMyPlayer::ZoomIn(float DeltaTime)
{
	Zoom -= (ZoomRate * DeltaTime);
	Camera->FieldOfView = Zoom;
	UE_LOG(LogTemp, Warning, TEXT("Zoom = %f"), Zoom);

	if (Zoom <= ZoomedValue)
	{
		bTriggerZooming = false;
	}
}

void AMyPlayer::ZoomOut(float DeltaTime)
{
	Zoom += (ZoomRate * DeltaTime);
	Camera->FieldOfView = Zoom;
	UE_LOG(LogTemp, Warning, TEXT("Zoom = %f"), Zoom);

	if (Zoom >= InitialZoom)
	{
		bTriggerZoomingOut = false;
	}
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

void AMyPlayer::MeshModification()
{
	if (PlayerStatus == EPlayerStatus::EPS_Match && bToggleMeshLoc)
	{
		GetMesh()->SetRelativeLocation(RpsMeshLocation);
		bToggleMeshLoc = false;
	}
	else if (PlayerStatus == EPlayerStatus::EPS_Fight && !(bToggleMeshLoc))
	{
		GetMesh()->SetRelativeLocation(InitialMeshLocation);
		bToggleMeshLoc = true;
	}
}

void AMyPlayer::HealthMechanics_Implementation()
{
	Health -= 5;
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

void AMyPlayer::CheckShooting()
{
	if (bCanPlayerShoot)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Can Player Shoot Is True"));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Can Player Shoot Is False"));
	}
}

void AMyPlayer::CheckHealth()
{
	HealthMechanics();
}