// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaPit.h"

#include "A3ACharacter.h"

// Sets default values
ALavaPit::ALavaPit()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));

	LavaPitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	LavaPitMesh->SetupAttachment(RootComponent);

	LavaPitHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox Component"));
	LavaPitHitBox->SetupAttachment(RootComponent);

	LavaPitHitBox->OnComponentBeginOverlap.AddDynamic(this, &ALavaPit::OnHitBoxOverlapBegin);
	LavaPitHitBox->OnComponentEndOverlap.AddDynamic(this, &ALavaPit::OnHitBoxOverlapEnd);
}

void ALavaPit::SpawnNSSystem()
{
	UNiagaraComponent* LavaPitComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), NS_LavaPit, this->GetActorLocation(), this->GetActorRotation());
}


void ALavaPit::OnHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                                    AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		Cast<AA3ACharacter>(OtherActor)->ChangeHealth(HealthAlteration);
		UE_LOG(LogTemp, Warning, TEXT("Pressurepad Stepped On"));
	}
}

void ALavaPit::OnHitBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent,
                                  AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<AA3ACharacter>(OtherActor))
	{
		Cast<AA3ACharacter>(OtherActor)->ResetDamage();
	}
}


// Called when the game starts or when spawned
void ALavaPit::BeginPlay()
{
	Super::BeginPlay();
	SpawnNSSystem();
}
