// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "CharacterBase.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

void APlayerControllerBase::HandleLook(const FInputActionValue& InputActionValue)
{
	// Value is a Vector2D
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// Add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void APlayerControllerBase::HandleMove(const FInputActionValue& InputActionValue)
{
	// Value is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Add movement to the Player's Character pawn
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y, false);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X, false);
	}
}

void APlayerControllerBase::HandleJump()
{
	// Make the Player's Pawn jump, disabling crouch if it was active
	if (PlayerCharacter)
	{
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
	}
}

void APlayerControllerBase::HandleToggleSprint()
{
}

void APlayerControllerBase::HandleToggleCrouch()
{
	if (PlayerCharacter && PlayerCharacter->bIsCrouched)
		PlayerCharacter->UnCrouch();
	else
		PlayerCharacter->Crouch();
}

void APlayerControllerBase::HandleCycleUIMode()
{
	if (PlayerHUD)
		PlayerHUD->CycleToNextViewMode();
}

void APlayerControllerBase::AcceptScreenshot(int Width, int Height, const TArray<FColor>& Colors)
{
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Store the reference to the player's pawn
	PlayerCharacter = Cast<ACharacterBase>(InPawn);
	checkf(PlayerCharacter,
		TEXT("APlayerControllerBase derived classes should only be posess ACharacterBase derived pawns"));

	// Get a reference to the EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get reference to the EnhancedInputComponent"));

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Inable to get reference to the EnhancedInputLocalPlayerSubsystem"));

	// Wipe existing mappings, and add our mapping
	checkf(InputMappingContext, TEXT("InputMappingContext was not specified"));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
	// Bind the input actions
	// Only attempt to bind if a valid value was provided
	if (ActionMove)
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleMove);
	}

	if (ActionLook)
	{
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleLook);
	}

	if (ActionJump)
	{
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleJump);
	}

	if (ActionToggleSprint)
	{
		EnhancedInputComponent->BindAction(ActionToggleSprint, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleToggleSprint);
	}

	if (ActionToggleCrouch)
	{
		EnhancedInputComponent->BindAction(ActionToggleCrouch, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleToggleCrouch);
	}

	if (ActionCycleUIMode)
	{
		EnhancedInputComponent->BindAction(ActionCycleUIMode, ETriggerEvent::Triggered, this,
			&APlayerControllerBase::HandleCycleUIMode);
	}
}

void APlayerControllerBase::OnUnPossess()
{
	// Unbind things here
	EnhancedInputComponent->ClearActionBindings();
	
	Super::OnUnPossess();
}
