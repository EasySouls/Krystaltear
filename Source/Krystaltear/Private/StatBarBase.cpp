// Fill out your copyright notice in the Description page of Project Settings.


#include "Krystaltear/Public/StatBarBase.h"

#include "CustomLogging.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UStatBarBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(KrystaltearLog, Display, TEXT("NativeOnInitialized()"));
	UpdateWidget();
}

void UStatBarBase::OnIntStatUpdated(int32 OldValue, int32 NewValue, int32 MaxValue)
{
	// Use the float version of the function!
	OnFloatStatUpdated(static_cast<float>(OldValue), static_cast<float>(NewValue), static_cast<float>(MaxValue));
}

void UStatBarBase::OnFloatStatUpdated(float OldValue, float NewValue, float MaxValue)
{
	// Calculate the new percentage and clamp it between 0 and 1
	// Prevent divide by 0 errors
	if (MaxValue == 0.0f) MaxValue = KINDA_SMALL_NUMBER;

	CurrentPercentage = FMath::Clamp(NewValue / MaxValue, 0.0f, 1.0f);
	CurrentValue = NewValue;
	UpdateWidget();
}

void UStatBarBase::ProcessCurrentValueText()
{
	FString FloatString;

	if (CurrentValue < 1000.0f)
	{
		FloatString = FString::SanitizeFloat(CurrentValue);
		
		if (CurrentValue < 100.0f)
		{
			const int32 StringLen = FloatString.Len();
			if (StringLen > 4)
				FloatString = FloatString.Left(4);
			else if (StringLen < 4)
				FloatString = FloatString.Append("0", 4 - StringLen);
		}
		else
		{
			FloatString = FloatString.Left(3);
		}
	}
	else
	{
		// Scaled value
		const float ScaledValue = CurrentValue / 1000.0f;
		FloatString = FString::SanitizeFloat(ScaledValue);
		if (ScaledValue < 10.0f)
			FloatString = FloatString.Left(3).Append(TEXT("k"));
		else
		{
			FloatString = FloatString.Left(2).Append(TEXT("K"));
		}
	}

	CurrentValueText = FText::FromString(FloatString);
}

void UStatBarBase::UpdateWidget()
{
	// Check that the controls we want actually exist
	if (!PercentBar_Filled || !PercentBar_Empty) return;

	FSlateChildSize EmptySize = FSlateChildSize(ESlateSizeRule::Fill);
	EmptySize.Value = 1.0f - CurrentPercentage;

	FSlateChildSize FilledSize = FSlateChildSize(ESlateSizeRule::Fill);
	FilledSize.Value = CurrentPercentage;

	if (UVerticalBoxSlot* FilledSlot = Cast<UVerticalBoxSlot>(PercentBar_Filled->Slot))
		FilledSlot->SetSize(FilledSize);

	if (UVerticalBoxSlot* EmptySlot = Cast<UVerticalBoxSlot>(PercentBar_Empty->Slot))
		EmptySlot->SetSize(EmptySize);

	MainBorder->SetBrushColor(BarBackgroundColor);
	PercentBar_Filled->SetBrushColor(BarForeGroundColor);
	IconImage->SetBrush(IconBrush);

	ProcessCurrentValueText();

	ValueText->SetText(CurrentValueText);

	PercentBars->SetVisibility(bIsFullSize ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

#if WITH_EDITOR
void UStatBarBase::OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs)
{
	Super::OnDesignerChanged(EventArgs);
	// Update the widget, after editor changes due to layout.
	// For example, resizing the widget, or a container the widget is in.
	UpdateWidget();
}

// void UStatBarBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	if (const FString PropertyName = ((PropertyChangedEvent.Property != nullptr)
// 		                                  ? PropertyChangedEvent.Property->GetName()
// 		                                  : NAME_None).ToString(); PropertyName == TEXT("CurrentPercentage") || PropertyName == TEXT("CurrentValue"))
// 	{
// 		UpdateWidget();
// 	}
// }
#endif