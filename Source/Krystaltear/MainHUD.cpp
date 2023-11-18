// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "CharacterBase.h"
#include "CustomLogging.h"
#include "Public/MinimalLayoutBase.h"
#include "Public/ModerateLayoutBase.h"
#include "Public/OverloadLayoutBase.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the current world
	World = GetWorld();
	checkf(World, TEXT("Failed to reference world."));

	// Ensure the values are valid for the widget classes used by the HUD
	checkf(MinimalLayoutClass, TEXT("Invalid MinimalLayoutClass reference"));
	checkf(ModerateLayoutClass, TEXT("Invalid ModerateLyoutClass reference"));
	checkf(OverloadLayoutClass, TEXT("Invalid OverloadLayoutClass reference"));

	// Create the types of layout widgets, and add them to the viewport
	// Maybe later change it for one widhet which mutates based on the ViewMode
	// When creating a widget, the first parameter (owning object) must be one of the following types:
	// UWidget, UWidgetTree, APlayerController, UGameInstance or UWorld
	MinimalLayoutWidget = CreateWidget<UMinimalLayoutBase>(World, MinimalLayoutClass);
	MinimalLayoutWidget->AddToViewport();
	MinimalLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	ModerateLayoutWidget = CreateWidget<UModerateLayoutBase>(World, ModerateLayoutClass);
	ModerateLayoutWidget->AddToViewport();
	ModerateLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	OverLoadLayoutWidget = CreateWidget<UOverloadLayoutBase>(World, OverloadLayoutClass);
	OverLoadLayoutWidget->AddToViewport();
	OverLoadLayoutWidget->SetVisibility(ESlateVisibility::Collapsed);

	// Set a reference to the character, and hook up the stat handlers
	if (const APlayerController* PlayerController = GetOwningPlayerController())
		PlayerCharacter = Cast<ACharacterBase>(PlayerController->GetPawn());
	checkf(PlayerCharacter, TEXT("Unable to get a reference to the player character"));

	// Set the initial ViewMode to the current one, which allows setting via the editor
	//SetCurrentViewMode(CurrentViewMode)
	UpdateWidgets();
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMainHUD::CycleToNextViewMode()
{
	++CurrentViewMode;
	UE_LOG(KrystaltearLog, Warning, TEXT("CycleToNextViewMode: %s"), *UEnum::GetValueAsString(CurrentViewMode))
	UpdateWidgets();
}

void AMainHUD::SetCurrentViewMode(EHudViewMode NewViewMode)
{

}

void AMainHUD::UpdateWidgets()
{
	// Unhook any delegate handler
	ClearAllHandlers();

	// Set all the widgets
}

void AMainHUD::ClearAllHandlers()
{
	PlayerCharacter->OnHealthChanged.Clear();
	PlayerCharacter->OnStaminaChanged.Clear();
	PlayerCharacter->OnKeyAction.Clear();
}


