// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "GoldCoinItem.generated.h"

UCLASS()
class CH3_04PROJECT_API AGoldCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	AGoldCoinItem();

protected:

	virtual void ActivateItem(AActor* Activator) override;
	
};
