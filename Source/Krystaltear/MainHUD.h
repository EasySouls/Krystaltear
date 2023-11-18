// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class ACharacterBase;
class UMinimalLayoutBase;
class UModerateLayoutBase;
class UOverloadLayoutBase;

UENUM(BlueprintType)
enum class EHudViewMode : uint8
{
	Disabled UMETA(Tooltip = "HUD is disabled"),
	Normal UMETA(Tooltip = "Only the necessary information is showed"),
	SensoryOverload UMETA(Tooltip = "All the information")
};

inline EHudViewMode& operator++(EHudViewMode& ViewMode)
{
	if (ViewMode == EHudViewMode::SensoryOverload)
		ViewMode = EHudViewMode::Disabled;
	else
		ViewMode = static_cast<EHudViewMode>(static_cast<int>(ViewMode) + 1);

	return ViewMode;
}

inline EHudViewMode& operator--(EHudViewMode& ViewMode)
{
	if (ViewMode == EHudViewMode::Disabled)
		ViewMode = EHudViewMode::SensoryOverload;
	else
		ViewMode = static_cast<EHudViewMode>(static_cast<int>(ViewMode) + 1);

	return ViewMode;
}

UCLASS(Abstract)
class KRYSTALTEAR_API AMainHUD : public AHUD
{
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMinimalLayoutBase> MinimalLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UModerateLayoutBase> ModerateLayoutClass = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverloadLayoutBase> OverloadLayoutClass = nullptr;

	// Allow code and blueprints to set the current ViewMode
	UFUNCTION(BlueprintCallable)
	void SetCurrentViewMode(EHudViewMode NewViewMode);

	// Cycle to the next ViewMode
	UFUNCTION(BlueprintCallable)
	void CycleToNextViewMode();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	// Determines which UI elements should be displayed
	UPROPERTY(EditAnywhere)
	EHudViewMode CurrentViewMode = EHudViewMode::Normal;

	// Whenever the ViewMode is changed, this function is called to update the widgets
	void UpdateWidgets();

	// Release any delegate bindings
	void ClearAllHandlers();

	UPROPERTY()
	UWorld* World = nullptr;

	UPROPERTY()
	TObjectPtr<UMinimalLayoutBase> MinimalLayoutWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UModerateLayoutBase> ModerateLayoutWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UOverloadLayoutBase> OverLoadLayoutWidget = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacterBase> PlayerCharacter = nullptr;
	
	GENERATED_BODY()
};
