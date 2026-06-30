// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH3PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH3_04PROJECT_API ACH3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACH3PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* FlyAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	//게임결과UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Result")
	bool bResultHasNextLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	TSubclassOf<UUserWidget> GameResultWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameResult")
	UUserWidget* GameResultWidgetInstance;
	UFUNCTION(BlueprintCallable, Category = "GameResult")
	void ShowGameResult(bool bHasNextLevel);
	UFUNCTION(BlueprintCallable, Category = "GameResult")
	void NextGame();
	UFUNCTION(BlueprintCallable, Category = "GameResult")
	void OnResultButtonClicked();
	UFUNCTION(BlueprintPure, Category = "GameResult")
	UUserWidget* GetGameResultWidget() const;


protected:
	virtual void BeginPlay() override;
	
};
