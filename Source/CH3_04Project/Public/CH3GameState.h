// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH3GameState.generated.h"

/**
 * 
 */
UCLASS()
class CH3_04PROJECT_API ACH3GameState : public AGameState
{
	GENERATED_BODY()

public:
	ACH3GameState();

	UFUNCTION(BlueprintCallable, Category = "Point")
	void AddPoint(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Point")
	void AddTime(float Amount);
	UFUNCTION(BlueprintPure, Category = "Point")
	int32 GetPoint()const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	void DecreaseTime(float DeltaTime);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float RemainingTime;

	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected(bool bCountsForLevelClear, int32 Amount);
	void EndLevel();
	void UpdateHUD();
	void UpdateGameResult();

	bool bHasNextLevel;

	UFUNCTION(BlueprintCallable)
	void StartNextLevel();

protected:
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Point")
	int32 TotalPoint;

	virtual void BeginPlay() override;
	bool bIsLevelEnded;
};
