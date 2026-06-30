// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameInstance.h"

UCH3GameInstance::UCH3GameInstance()
{
	CurrentLevelIndex = 0;
	TotalScore = 0;
}

void UCH3GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("+ %d Point / Total: %d Point"),Amount, TotalScore));
}

 