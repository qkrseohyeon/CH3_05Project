// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "CH3GameState.h"
#include "CH3GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"


ACH3PlayerController::ACH3PlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	FlyAction(nullptr),
	RollAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
	GameResultWidgetClass(nullptr),
	GameResultWidgetInstance(nullptr)
{
}

void ACH3PlayerController::BeginPlay()
{
	Super::BeginPlay();
	//IMC활성화
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ACH3PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ACH3PlayerController::ShowMainMenu(bool bIsRestart)
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameResultWidgetInstance)
	{
		GameResultWidgetInstance->RemoveFromParent();
		GameResultWidgetInstance = nullptr;
	}

	// 메뉴 UI 생성
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(InputMode);
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("RESTART")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("START")));
			}
		}
	}
}

// 게임 HUD 표시
void ACH3PlayerController::ShowGameHUD()
{
	// HUD가 켜져 있다면 닫기
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	// 이미 메뉴가 떠 있으면 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameResultWidgetInstance)
	{
		GameResultWidgetInstance->RemoveFromParent();
		GameResultWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			ACH3GameState* CH3GameState = GetWorld() ? GetWorld()->GetGameState<ACH3GameState>() : nullptr;
			if (CH3GameState)
			{
				CH3GameState->UpdateHUD();
			}
		}
	}
}

// 게임 시작 - BasicLevel 오픈, GameInstance 데이터 리셋
void ACH3PlayerController::StartGame()
{
	if (UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		CH3GameInstance->CurrentLevelIndex = 0;
		CH3GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}

void ACH3PlayerController::ShowGameResult(bool bHasNextLevel)
{
	bResultHasNextLevel = bHasNextLevel;

	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameResultWidgetInstance)
	{
		GameResultWidgetInstance->RemoveFromParent();
		GameResultWidgetInstance = nullptr;
	}

	if (GameResultWidgetClass)
	{
		GameResultWidgetInstance = CreateWidget<UUserWidget>(this, GameResultWidgetClass);
		if (GameResultWidgetInstance)
		{
			GameResultWidgetInstance->AddToViewport();

			bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameResultWidgetInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			SetInputMode(InputMode);
		}
	}
	if (UTextBlock* ButtonText = Cast<UTextBlock>(GameResultWidgetInstance->GetWidgetFromName(TEXT("NextLevelButtonText"))))
	{
		if (bHasNextLevel)
		{
			ButtonText->SetText(FText::FromString(TEXT("NEXT")));
		}
		else
		{
			ButtonText->SetText(FText::FromString(TEXT("RESTART")));
		}
	}
}

void ACH3PlayerController::NextGame()
{
	if (GameResultWidgetInstance)
	{
		GameResultWidgetInstance->RemoveFromParent();
		GameResultWidgetInstance = nullptr;
	}

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	ACH3GameState* CH3GameState = GetWorld() ? GetWorld()->GetGameState<ACH3GameState>() : nullptr;
	if (CH3GameState)
	{
		CH3GameState->UpdateHUD();
	}
}

void ACH3PlayerController::OnResultButtonClicked()
{
	if (bResultHasNextLevel)
	{
		if (ACH3GameState* CH3GameState = GetWorld() ? GetWorld()->GetGameState<ACH3GameState>() : nullptr)
		{
			CH3GameState->StartNextLevel();
		}
	}
	else
	{
		StartGame();
	}
}

UUserWidget* ACH3PlayerController::GetGameResultWidget() const
{
	return GameResultWidgetInstance;
}
