// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoinItem.h"
#include "SilverCoinItem.generated.h"

UCLASS()
class CH3_04PROJECT_API ASilverCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	ASilverCoinItem();

protected:

	virtual void ActivateItem(AActor* Activator) override;
};
