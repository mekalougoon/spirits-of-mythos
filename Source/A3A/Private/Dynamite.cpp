// Fill out your copyright notice in the Description page of Project Settings.


#include "Dynamite.h"

// Sets default values
ADynamite::ADynamite()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	EndMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndMesh"));
	EndMesh->SetupAttachment(BaseMesh);
	StringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StringMesh"));
	StringMesh->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void ADynamite::BeginPlay()
{
	Super::BeginPlay();
}

void ADynamite::Interact_Implementation()
{
	//IInteractable::Interact_Implementation();
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(SweepSize);
	DrawDebugSphere(GetWorld(), Location, ExplosionSphere.GetSphereRadius(), 50, FColor::Blue, false, 1.0f, 0, 0);


	if (GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, ExplosionSphere))
	{
		for (auto& Hit : OutHits)
		{
			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			if (Mesh)
			{
				switch (GravityType)
				{
				case EGravityType::GE_Down:
					Mesh->SetEnableGravity(true);
					break;
				case EGravityType::GE_Push:
					Mesh->AddRadialImpulse(Location, SweepSize, 2000.0f, ERadialImpulseFalloff::RIF_Linear, true);
					break;
				default:
					UE_LOG(LogClass, Warning, TEXT("No Gravity Type Selected"));
				}
			}
		}
	}
}
