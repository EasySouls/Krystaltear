// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainHUD.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class ACharacterBase;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Abstract)
class KRYSTALTEAR_API APlayerControllerBase : public APlayerController
{
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionMove = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionLook = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionJump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionToggleSprint = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputAction> ActionToggleCrouch = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|UI")
	TObjectPtr<UInputAction> ActionCycleUIMode = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
	
protected:
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleJump();
	void HandleToggleSprint();
	void HandleToggleCrouch();
	void HandleCycleUIMode();

	void AcceptScreenshot(int Width, int Height, const TArray<FColor>& Colors);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
private:
	// Used to store a reference to the InputComponent cast to an EnhancedInputComponent
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

	// Used to store a reference to the pawn we are controlling
	UPROPERTY()
	TObjectPtr<ACharacterBase> PlayerCharacter = nullptr;

	// Used to store a reference to the HUD
	UPROPERTY()
	TObjectPtr<AMainHUD> PlayerHUD = nullptr;
	
	GENERATED_BODY()
};
