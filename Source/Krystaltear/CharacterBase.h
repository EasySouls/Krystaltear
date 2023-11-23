// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatSystem.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

// Delegate for when stats based on integers are changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FIntStatUpdated,
	int32, OldValue,
	int32, NewValue,
	int32, MaxValue);

// Delegate for when stats based on floats are changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FFloatStatUpdated,
	float, OldValue,
	float, NewValue,
	float, MaxValue);

// Delegate for when the player dies
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerIsDead);

// Different actions invonving the keys
UENUM(BlueprintType)
enum class EPlayerKeyAction: uint8
{
	AddKey UMETA(Tooltip = "Attempt to add a key to the player"),
	RemoveKey UMETA(Tooltip = "Attemp to remove a key from the player"),
	TestKey UMETA(Tooltip = " Check if the player has a specific key")
};

// Delegate for when actions occur with the player's keys
// KeyString is the key involved in the action
// KeyAction shows what was attempted
// IsSuccess shows if the attempted action happened
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FKeysAction,
											   FString, KeyString,
											   EPlayerKeyAction, KeyAction,
											   bool, IsSuccess);

UCLASS()
class KRYSTALTEAR_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
public:

	ACharacterBase();

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	virtual void Jump() override;
	
	virtual void Crouch(bool bClientSimulation = false) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Getter for the Ability System Player Component
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMaxWalkSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float RunningMaxWalkSpeed = 800.0f;

	UFUNCTION(BlueprintCallable, Category="Player|Stamina")
	void SetRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void ToggleRunning();

	// Called to set the flag indicating the player jumped since the last update.
	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasJumped();

	// Called to se the flag indicating the player sprinted since the last update.
	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void SetHasRan();

#pragma region Health

	UFUNCTION(BlueprintPure, Category="Player|Health")
	int GetHealth() const;
	
	UFUNCTION(BlueprintPure, Category="Player|Health")
	int GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void UpdateHealth(int DeltaHealth);
	
	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void RestoreToFullHealth();
	
	UFUNCTION(BlueprintCallable, Category="Player|Health")
	void SetMaxHealth(int NewMaxHealth);

	// Triggered when the player's health is updated
	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FIntStatUpdated OnHealthChanged;
	
	// Triggered when the player dies
	UPROPERTY(BlueprintAssignable, Category = "Player|Health")
	FPlayerIsDead OnPlayerDied;
	
#pragma endregion
	
#pragma region Stamina

	UFUNCTION(BlueprintPure, Category="Player|Stamina")
	float GetStamina() const;
	
	UFUNCTION(BlueprintPure, Category="Player|Stamina")
	float GetStaminaRegenRate() const;
	
	UFUNCTION(BlueprintCallable, Category="Player|Stamina")
	void SetStaminaRegenRate(float NewStaminaRegenRate);
	
	// Triggered when the player's stamina changes
	UPROPERTY(BlueprintAssignable, Category = "Player|Stamina")
	FFloatStatUpdated OnStaminaChanged;
	
#pragma endregion
	
#pragma region Keys
	
	// Add a key if the Player does not already has it
	UFUNCTION(BlueprintCallable, Category="Player|Keys")
	void AddKey(FString KeyToAdd);

	// Remove thekey if the Player has it
	UFUNCTION(BlueprintCallable, Category="Player|Keys")
	void RemoveKey(FString KeyToRemove);

	// Check if the Player has the key
	UFUNCTION(BlueprintPure, Category="Player|Keys")
	bool IsPlayerCarryingKey(const FString& DesiredKey) const;
	
	// Triggered when something happens with the player's keys
	UPROPERTY(BlueprintAssignable, Category = "Player|Keys")
	FKeysAction OnKeyAction;
	
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	// Combat System Component
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UCombatSystem> CombatSystemComponent;
	
private:

	// Is the character currently set to sprint?
	bool bIsRunning = false;

	// Did the character sprint since the last update?
	bool bHasRun = false;
	
	// Has the player jumped since the last update?
	bool bHasJumped = false;
	
	// Health
	static constexpr int BaseHealthValue = 100;
	static constexpr int MinMaxHealth = 40;
	int	MaxHealth = BaseHealthValue;
	int CurrentHealth = BaseHealthValue;

	// Stamina
	static constexpr float BaseStaminaValue = 50.0f;
	static constexpr float JumpStaminaCost = 20.0f;
	static constexpr float SprintStaminaCost = 3.0f;
	static constexpr float RestStaminaRegenRateMultiplier = 2.0f;
	static constexpr float MinStaminaRegenRate = 1.0f;
	float MaxStamina = BaseStaminaValue;
	float CurrentStamina = BaseStaminaValue;
	float StaminaRegenRate = 4.0f;

	// Player keys
	TArray<FString> Keys;

	GENERATED_BODY()
};
