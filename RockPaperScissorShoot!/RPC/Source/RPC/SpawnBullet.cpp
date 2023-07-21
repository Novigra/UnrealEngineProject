// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBullet.h"
#include "MyPlayer.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnBullet::ASpawnBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BulletCollision"));
	BulletCollision->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BulletCollision);

	BulletSpeed = 10.0f;
}

// Called when the game starts or when spawned
void ASpawnBullet::BeginPlay()
{
	Super::BeginPlay();
	
	MyPlayer = UGameplayStatics::GetPlayerController(this, 0)->GetPawn<AMyPlayer>();
	CurrentWeapon = MyPlayer->GetEquippedWeapon();

	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnBullet::OnOverlapBegin);
	BulletCollision->OnComponentEndOverlap.AddDynamic(this, &ASpawnBullet::OnOverlapEnd);
}

// Called every frame
void ASpawnBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASpawnBullet::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}