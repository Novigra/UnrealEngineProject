// Fill out your copyright notice in the Description page of Project Settings.


#include "Placement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyPlayer.h"

// Sets default values
APlacement::APlacement()
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
void APlacement::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APlacement::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &APlacement::OnOverlapEnd);
}

// Called every frame
void APlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlacement::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		if (MyPlayer)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Reached the destination!"));

			MyPlayer->bPushBack = false;
			MyPlayer->PlayerStatus = EPlayerStatus::EPS_Fight;
		}
	}
}

void APlacement::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Destroy();
}