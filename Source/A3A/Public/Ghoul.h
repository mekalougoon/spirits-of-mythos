// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A3ACharacter.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "EnhancedInputComponent.h"
#include "Ghoul.generated.h"


UCLASS()
class A3A_API AGhoul : public AA3ACharacter
{
	GENERATED_BODY()
	AGhoul();

public:
	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Material;

	AA3ACharacter* Player;
	UMaterialInstanceDynamic* MatInstance;
	UMaterialInstanceDynamic* DefaultMatInstance;


	UPROPERTY(EditAnywhere)
	bool bIsInvisible;

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool SetStartAbility();

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool SetEndAbility();

	UFUNCTION(BlueprintCallable, Category = Ability)
	bool GetAbilityStatus();


	FTimerHandle InvisibleTimerHandle;
	float AbilityRemaining;

	UFUNCTION()
	void AbilityCooldown();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/*/** MappingContext #1#
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* GhoulMappingContext;*/

	/** invisible Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InvisibleAction;

	/** Called for invisible input */
	void Invisible(const FInputActionValue& Value);

	void BeginPlay() override;
};
