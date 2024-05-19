// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghoul.h"

#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "StaticMeshAttributes.h"
#include "TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"


AGhoul::AGhoul()
{
	bIsInvisible = false;
}

// start ability
bool AGhoul::SetStartAbility()
{
	//ability
	CoolDownRemaining = 14;
	GetWorldTimerManager().SetTimer(InvisibleTimerHandle, this, &AGhoul::AbilityCooldown, 1.0f, true, 1.0f);
	bIsInvisible = true;
	UE_LOG(LogClass, Warning, TEXT("actor is now invisible"));
	Mesh1P->SetMaterial(0, MatInstance);
	MatInstance->SetScalarParameterValue("Opacity", 0.5);
	return bIsInvisible;
}


// end ability
bool AGhoul::SetEndAbility()
{
	//dynamic mesh
	Mesh1P->SetMaterial(0, DefaultMatInstance);
	//MatInstance->SetScalarParameterValue("Opacity", 1);
	//MatInstance->SetScalarParameterValue("Panner", 0);

	//ability
	GetWorldTimerManager().ClearTimer(InvisibleTimerHandle);
	bIsInvisible = false;
	UE_LOG(LogClass, Warning, TEXT("actor is no longer invisible"));
	return bIsInvisible;
}

void AGhoul::AbilityCooldown()
{
	if (--CoolDownRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(InvisibleTimerHandle);
	}
}

bool AGhoul::GetAbilityStatus()
{
	return bIsInvisible;
}

void AGhoul::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			Cast<APlayerController>(GetController())->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(GhoulMappingContext, 0);
		}

		//invisible
		EnhancedInputComponent->BindAction(InvisibleAction, ETriggerEvent::Triggered, this, &AGhoul::Invisible);
	}
}

void AGhoul::Invisible(const FInputActionValue& Value)
{
	if (bIsInvisible == true)
	{
		SetEndAbility();
	}
	else
	{
		SetStartAbility();
	}
}

void AGhoul::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AA3ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Material)
	{
		MatInstance = UMaterialInstanceDynamic::Create(Material, this);
		Mesh1P->SetMaterial(0, MatInstance);
		Mesh1P->SetMaterial(0, DefaultMatInstance);
	}
}
