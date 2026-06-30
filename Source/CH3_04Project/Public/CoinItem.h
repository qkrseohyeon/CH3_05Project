// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class CH3_04PROJECT_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
	bool bCountsForLevelClear;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;

	
};
