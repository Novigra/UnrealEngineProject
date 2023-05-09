// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPlacement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"

// Sets default values
AEnemyPlacement::AEnemyPlacement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMesh);
	BoxCollision->SetBoxExtent(FVector(40.0f));
}

// Called when the game starts or when spawned
void AEnemyPlacement::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPlacement::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyPlacement::OnOverlapEnd);
}

// Called every frame
void AEnemyPlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyPlacement::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("(Enemy) Reached the destination!"));

			Enemy->bEnemyPushBack = false;
			Enemy->EnemyStatus = EEnemyStatus::EES_Fight;
		}
	}
}

void AEnemyPlacement::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Destroy();
}