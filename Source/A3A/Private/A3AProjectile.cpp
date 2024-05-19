// Copyright Epic Games, Inc. All Rights Reserved.

#include "A3AProjectile.h"

#include "Brazier.h"
#include "Dynamite.h"
#include "Wall.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AA3AProjectile::AA3AProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AA3AProjectile::OnHit);
	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NS_FireTrail,CollisionComp, NAME_None, FVector(10.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

	

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	/*if(material)
	{
		matInstance = UMaterialInstanceDynamic::Create(Material, this);
		CollisionComp->SetMaterial(0, matInstance);
	}*/
}

void AA3AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	//add effect on impact
	// UNiagaraComponent* FireTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_FireTrail, this->GetActorLocation(), this->GetActorRotation());

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->IsA(AWall::StaticClass()) == true)
		{
			Cast<AWall>(OtherActor)->ChangeHealth();
			UE_LOG(LogTemp, Warning, TEXT("Wall has been hit"));
		}

		else if (OtherActor->IsA(ABrazier::StaticClass()) == true)
		{
			Cast<ABrazier>(OtherActor)->TurnOn();
			UE_LOG(LogTemp, Warning, TEXT("Brazier has been hit"));
		}

		else if (OtherActor->IsA(ADynamite::StaticClass()) == true)
		{
			IInteractable::Execute_Interact(Hit.GetActor());
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT
			       ("Actor is NOT Interactable!"));
		}
		Destroy();
	}
}

