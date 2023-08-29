// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericPlatform/ICursor.h"
#include "Enemy.h"
#include "Placement.h"
#include "Weapon.h"
#include "Wall.h"
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

	//Create Hand Collision
	HandCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HandCollision"));
	HandCollision->SetupAttachment(GetMesh(), "R_wristSocket");

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
	bToggleMeshLoc = true;

	// Character Properties
	MaxHealth = 100.0f;
	Health = 100.0f;

	CurrentSpeed = 0.0f;

	WinnerSpeed = 800.0f;
	LoserSpeed = 400.0f;
	DashSpeed = 1000.0f;

	bCanPunch = false;

	bCanDash = false;
	bCanPlayerShoot = false;

	Zoom = Camera->FieldOfView;
	InitialZoom = Zoom;
	ZoomRate = 1.0f;
	bTriggerZooming = false;
	bTriggerZoomingOut = false;

	bIsPressed = false;
	bStartPunching = false;

	PlayerDashDirection = FVector(0.0f);
	PlayerDashSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitialMeshLocation = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(RpsMeshLocation);

	PlayerControllerRef = UGameplayStatics::GetPlayerController(this, 0);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyPlayer::OnOverlapEnd);
	HandCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::PunchOnOverlapBegin);

	LoadActors();

	if (GetWorld())
	{
		CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	}
	if (CameraManager)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("WE GOT THE CAMERA!!!"));
	}
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
	//MeshModification();
	
	if (GetPlayerStatus() == EPlayerStatus::EPS_Match)
	{
		// Match Timer
		if (!(bStopTimer))
		{
			MatchTimer += (MatchTimerRate * DeltaTime);
			//UE_LOG(LogTemp, Log, TEXT("Match Timer = %f"), MatchTimer);
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
}

void AMyPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->IsA(APlacement::StaticClass()))
		{
			OnPlayerDestination.Broadcast();

			GetMesh()->SetRelativeLocation(InitialMeshLocation);

			if (PlayerStatus == EPlayerStatus::EPS_Pending)
			{
				SetCharacterHealth();
				SetCharacterSpeed();
				SetCharacterDash();
			}
		}
	}

	
}

void AMyPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMyPlayer::PunchOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && bCanPunch)
	{
		if (!(OtherActor->IsA(AWeapon::StaticClass())))
		{
			if (OtherActor->IsA(AEnemy::StaticClass()))
			{
				Enemy = Cast<AEnemy>(OtherActor);
				Enemy->bCanGetPushed = true;

				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Super Punch : %s"), *OtherActor->GetName()));
			}
			else if (OtherActor->IsA(AWall::StaticClass()))
			{
				FVector WallLocation = OtherActor->GetActorLocation();
				FVector PlayerLocation = GetActorLocation();

				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Super Punch : %s"), *OtherActor->GetName()));
				FVector PushBack = UKismetMathLibrary::GetDirectionUnitVector(WallLocation, PlayerLocation) * PushBackScale;
				LaunchCharacter(PushBack, false, false);
			}
		}
		bCanPunch = false;
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

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMyPlayer::Dash);

	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AMyPlayer::StartPunch);
	PlayerInputComponent->BindAction("Punch", IE_Released, this, &AMyPlayer::StopPunch);

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

void AMyPlayer::SetCharacterHealth()
{
	if (PlayerResult == EPlayerResult::EPR_Winner)
	{
		Health = 100.0f;
	}
	else
	{
		Health = 50.0f;
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

void AMyPlayer::Dash()
{
	if (GetCharacterMovement()->IsMovingOnGround() && bCanDash)
	{
		PlayerDashDirection = GetCharacterMovement()->Velocity;
		GetCharacterMovement()->AddImpulse(PlayerDashDirection * PlayerDashSpeed, true);
	}
}

void AMyPlayer::StartPunch()
{
	if (PlayerResult == EPlayerResult::EPR_Winner)
	{
		bStartPunching = true;
		bCanPunch = true;
	}
}

void AMyPlayer::StopPunch()
{
	bStartPunching = false;
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
		OnStartShooting.Broadcast();
	}
}

void AMyPlayer::StopShoot()
{
	if (PlayerStatus == EPlayerStatus::EPS_Fight)
	{
		OnStopShooting.Broadcast();
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