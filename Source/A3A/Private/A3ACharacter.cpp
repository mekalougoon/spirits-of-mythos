// Copyright Epic Games, Inc. All Rights Reserved.

#include "A3ACharacter.h"
#include "A3AProjectile.h"
#include "Dragon.h"
#include "Dynamite.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Ghoul.h"
#include "InGameWidget.h"
#include "Pixie.h"
#include "TP_WeaponComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"


//////////////////////////////////////////////////////////////////////////
// AA3ACharacter

AA3ACharacter::AA3ACharacter()
{
	// Character doesnt have a rifle at start
	bHasFire = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AA3ACharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UInGameWidget::StaticClass());
	if (bTakeDamage)
	{
		Health += HealthAlteration;
		if (Health > 100)
		{
			Health = 100.0f;
		}
		if (Health < 0)
		{
			Health = 0.0f;
		}
	}
	Cast<UInGameWidget>(Widgets[0])->UpdateHealth();
}

void AA3ACharacter::ChangeHealth(float amount)
{
	bTakeDamage = true;
	HealthAlteration = amount;
}

void AA3ACharacter::ResetDamage()
{
	bTakeDamage = false;
	HealthAlteration = 0.0f;
}

void AA3ACharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AA3ACharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AA3ACharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AA3ACharacter::Look);

		//Possess
		EnhancedInputComponent->BindAction(PossessAction, ETriggerEvent::Started, this, &AA3ACharacter::CallMyTrace);
	}
}


void AA3ACharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector3D
	FVector3d MovementVector = Value.Get<FVector3d>() * MovementMultiplier;

	if (Controller != nullptr)
	{
		// add movement 
		//AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		//AddMovementInput(GetActorRightVector(), MovementVector.X);
		//AddMovementInput(GetActorUpVector(), MovementVector.Z);
		AddActorLocalOffset(MovementVector, true);
	}
}

void AA3ACharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AA3ACharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasFire = bNewHasRifle;
}

bool AA3ACharacter::GetHasRifle()
{
	return bHasFire;
}


//////////////////////////////////////////////////////////////////// Trace Ray

void AA3ACharacter::Interact_Implementation()
{
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	for (FHitResult& Hit : OutHits)
	{
		UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
		if (HitMesh)
		{
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				Execute_Interact(Hit.GetActor());
				UE_LOG(LogClass, Warning, TEXT("Actor is Interactable!"));
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("Actor is NOT Interactable!"));
			}


			/*AController* SavedController = GetController();
			SavedController->UnPossess();
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		
			// possess new character
			AActor *Character = HitOut.GetActor();
			SavedController->Possess(Cast<APawn>(Character));
			}*/
		}
	}
}


bool AA3ACharacter::Trace(UWorld* World, TArray<AActor*>& ActorsToIgnore, const FVector& Start, const FVector& End,
                          FHitResult& HitOut, ECollisionChannel CollisionChannel, bool ReturnPhysMat)
{
	if (!World)
	{
		return false;
	}
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	const FName TraceTag("MyTraceTag");
	World->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;

	HitOut = FHitResult(ForceInit);

	World->LineTraceSingleByChannel(HitOut, Start, End, CollisionChannel, TraceParams);
	return (HitOut.GetActor() != nullptr);
}

void AA3ACharacter::PossessCooldown()
{
	if (--CoolDownRemaining <= 0)
	{
		GetWorldTimerManager().ClearTimer(PossessTimerHandle);
	}
}

void AA3ACharacter::CallMyTrace()
{
	FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector ForwardVector = GetFirstPersonCameraComponent()->GetForwardVector();
	FVector End = Start + ForwardVector * 1256;
	FHitResult HitData(ForceInit);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		if (HitData.GetActor())
		{
			UE_LOG(LogClass, Warning, TEXT("We Hit This Actor:. %s"), *HitData.GetActor()->GetName());
				ProcessTraceHit(HitData);
		}
		else
		{
		}
	}
}

void AA3ACharacter::RemoveMappings()
{
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	/*if(PlayerController)
	{*/

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		Cast<APlayerController>(GetController())->GetLocalPlayer());

	if (Subsystem)
	{
		//AController* SavedController = GetController();
		/*Subsystem->RemoveMappingContext(GhoulMappingContext);
		if(Subsystem->HasMappingContext(GhoulMappingContext))
		{
			int temp = 0;
		}
		Subsystem->RemoveMappingContext(FireMappingContext);*/
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}


FString AA3ACharacter::ProcessTraceHit(FHitResult& HitOut)
{
	if(HitOut.GetActor()->IsA(ADynamite::StaticClass())!= true)
	{
		UE_LOG(LogClass, Warning, TEXT("Actor is %p"), HitOut.GetActor()->GetClass());
		if (HitOut.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			if (CoolDownRemaining <= 0)
			{
				CoolDownRemaining = 3;
				GetWorldTimerManager().SetTimer(PossessTimerHandle, this, &AA3ACharacter::PossessCooldown, 1.0f, true,
												1.0f);
				Execute_Interact(HitOut.GetActor());
				UE_LOG(LogClass, Warning, TEXT("Actor is Interactable!"));
				AController* SavedController = GetController();
				RemoveMappings();
				SavedController->UnPossess();
				GetCharacterMovement()->SetMovementMode(MOVE_None);


				//AActor* Temp = UGameplayStatics::GetActorOfClass(GetWorld(), UTP_WeaponComponent::StaticClass());
				//Subsystem->RemoveMappingContext(Cast<UTP_WeaponComponent>(Temp)->FireMappingContext);

				// possess new character
				AActor* Character = HitOut.GetActor();
				SavedController->Possess(Cast<APawn>(Character));


				//update the currently possessed character tag

				TArray<UUserWidget*> Widgets;
				UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UInGameWidget::StaticClass());

				if (Character->IsA(ADragon::StaticClass()))
				{
					Ability = "Ability: Fireball";
				}
				else if (Character->IsA(APixie::StaticClass()))
				{
					Ability = "Ability: Flight";
				}
				else if (Character->IsA(AGhoul::StaticClass()))
				{
					Ability = "Ability: Invisibility";
				}
				else
				{
					Ability = "Ability: Jump";
				}
				UE_LOG(LogClass, Warning, TEXT("Actor is Interactable! %s"), *FString(Ability));
				Cast<UInGameWidget>(Widgets[0])->UpdateName(Ability);
				return Ability;
			}
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Actor is NOT Interactable!"));
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
		                                 "Seconds till regenerated" + FString::FromInt(CoolDownRemaining));
	}
	UE_LOG(LogClass, Warning, TEXT("Actor is Interactable! %s"), *FString(Ability));
	return Ability;
}

void AA3ACharacter::MoveCharacterToCage()
{
	FVector Force = FVector(0, -4000 ,0);
	LaunchCharacter(Force, 0, 0);
	
	
}
