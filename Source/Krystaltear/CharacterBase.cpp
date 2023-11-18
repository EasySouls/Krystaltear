// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.5f);
    AActor::SetActorTickEnabled(true);
}

int ACharacterBase::GetHealth() const
{
	return CurrentHealth;
}

int ACharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

void ACharacterBase::UpdateHealth(const int DeltaHealth)
{
	// If the player is already dead, their health cannot be modified again
	if (CurrentHealth <= 0)
		return;

	// The value before we change it
	const int OldValue = CurrentHealth;
	
	CurrentHealth += DeltaHealth;

	// Make sure that the new CurrentHealth value is inside an acceptable range
	// In this case it will never be less than -1, or more than the MaxHealth
	CurrentHealth = FMath::Clamp(CurrentHealth, -1.0f, MaxHealth);

	// We only want to notify listeners if the value changed
	// For example when the player drinks a health potion at full health,
	// the listeners don't need to know about it
	if (CurrentHealth != OldValue)
	{
		OnStaminaChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
	}
	
	if (CurrentHealth <= 0.0f)
	{
		// Notify the listeners, that the player is dead
		OnPlayerDied.Broadcast();
	}
}

void ACharacterBase::RestoreToFullHealth()
{
	CurrentHealth = MaxHealth;
}

void ACharacterBase::SetMaxHealth(const int NewMaxHealth)
{
	const int OldValue = MaxHealth;

	// We just assume that the new value is within an acceptable range.
	// Might be better if we had some range checking?
	MaxHealth = NewMaxHealth;

	// Changing the MaxHealth 'might' also change the CurrentHealth,
	// if it is now less than the current health.
	// Regardless of that, we should fire the notification,
	// just in case there are any widgets listening which need to calculate a new %

	if (MaxHealth != OldValue) // We need to fire a notification
		{
		if (MaxHealth < OldValue)
		{
			// MaxHealth decreased, 
			// so we need to also cap the CurrentHealth to the new Max.
			if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
		}

		// There was a change, so notify any listeners
		OnHealthChanged.Broadcast(OldValue, CurrentHealth, MaxHealth);
		}
}

float ACharacterBase::GetStamina() const
{
	return CurrentStamina;
}

float ACharacterBase::GetStaminaRegenRate() const
{
	return StaminaRegenRate;
}

void ACharacterBase::SetStaminaRegenRate(float NewStaminaRegenRate)
{
	if (NewStaminaRegenRate < MinStaminaRegenRate)
		StaminaRegenRate = MinStaminaRegenRate;
	else
		StaminaRegenRate = NewStaminaRegenRate;
}

void ACharacterBase::SetRunning(bool IsRunning)
{
	bIsRunning = IsRunning;

	// Set the speed at which the player moves, based on if they are walking or running
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunningMaxWalkSpeed : NormalMaxWalkSpeed;
}

void ACharacterBase::ToggleRunning()
{
	SetRunning(!bIsRunning);
}

void ACharacterBase::SetHasJumped()
{
	bHasJumped = true;
}

void ACharacterBase::SetHasRan()
{
	bHasRun = true;
}

void ACharacterBase::AddKey(FString KeyToAdd)
{
	if (Keys.Contains(KeyToAdd))
	{ 
		OnKeyAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, false);
	}
	else
	{
		Keys.Add(KeyToAdd);
		OnKeyAction.Broadcast(KeyToAdd, EPlayerKeyAction::AddKey, true);
	}
}

void ACharacterBase::RemoveKey(FString KeyToRemove)
{
	if (Keys.Contains(KeyToRemove))
	{
		Keys.Remove(KeyToRemove);
		OnKeyAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, true);
	}
	else
	{
		OnKeyAction.Broadcast(KeyToRemove, EPlayerKeyAction::RemoveKey, true);
	}
}

bool ACharacterBase::IsPlayerCarryingKey(const FString& DesiredKey) const
{
	const bool Result = Keys.Contains(DesiredKey);
	OnKeyAction.Broadcast(DesiredKey, EPlayerKeyAction::TestKey, Result);
	return Result;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Enable crouching by default
	if (GetMovementComponent())
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void ACharacterBase::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	// If the player is running, check if they have enough stamina available
	// If not, return to walking
	if (bIsRunning && CurrentStamina <= 0)
	{
		SetRunning(false);
	}
	
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	// Set the flag indicating if the character ran
	if (bIsRunning) bHasRun = true;
}

void ACharacterBase::Jump()
{
	// Jump requires stamina
	if (CurrentStamina - JumpStaminaCost >= 0.0f)
	{
		UnCrouch();	
		Super::Jump();
		bHasJumped = true;
	}
}

void ACharacterBase::Crouch(bool bClientSimulation)
{
	SetRunning(false);
	Super::Crouch(bClientSimulation);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// There are currently one thing that can restore over time
	// Stamina

#pragma region Update Stamina

	float ActualStaminaRegenRate = StaminaRegenRate;

	// Stamina is spent if the player jumps or sprints
	if (bHasJumped) ActualStaminaRegenRate =  -JumpStaminaCost;
	else if (bHasRun) ActualStaminaRegenRate = -SprintStaminaCost;
	// Stamina regen is multiplied if the player is crouching
	else if (bIsCrouched) ActualStaminaRegenRate *= RestStaminaRegenRateMultiplier;

	// Keep track of value before being changed
	const float PreviousStamina = CurrentStamina;

	// Update the current stamina between the min and may value
	CurrentStamina = FMath::Clamp(CurrentStamina + ActualStaminaRegenRate,
		0.0f, MaxStamina);

	// If the value has actually changedm notify the listeners
	if (CurrentStamina != PreviousStamina)
	{
		OnStaminaChanged.Broadcast(PreviousStamina, CurrentStamina, MaxStamina);
	}

	// Reset the flags indicating physical exertion
	bHasRun = false;
	bHasJumped = false;
	
#pragma endregion

	// Debug logs
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Silver,
		*(FString::Printf(
			TEXT("Movement - IsCrouched:%d | IsSprinting:%d"), bIsCrouched, bIsRunning)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Red,
		*(FString::Printf(
			TEXT("Health - Current:%d | Max:%d"), CurrentHealth, MaxHealth)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Green,
		*(FString::Printf(
			TEXT("Stamina - Current:%f | Max:%f"), CurrentStamina, MaxStamina)));
	GEngine->AddOnScreenDebugMessage(-1, 0.49f, FColor::Orange,
		*(FString::Printf(
			TEXT("Keys - %d Keys Currently Held"), Keys.Num())));
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

