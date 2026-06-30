// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameState.h"
#include "FlyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CH3GameInstance.h"
#include "CoinItem.h"
#include "CH3PlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ACH3GameState::ACH3GameState()
{
	TotalPoint = 0;
	RemainingTime = 30.0f;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	bIsLevelEnded = false;
	PrimaryActorTick.bCanEverTick = true;
}

void ACH3GameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACH3GameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ACH3GameState::GetPoint() const
{
	return TotalPoint;
}

void ACH3GameState::AddPoint(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CH3GameInstance->AddToScore(Amount);
			TotalPoint += Amount;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Total Score Updated: %d"), TotalScore));
		}
	}
}

void ACH3GameState::StartLevel()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	RemainingTime;
	bIsLevelEnded = false;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CurrentLevelIndex = CH3GameInstance->CurrentLevelIndex;
		}
	}

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = (CurrentLevelIndex + 1) * 10;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	UpdateHUD();

	//디버깅 메시지
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("Level %d Start!, Spawned %d coin")
	//	, CurrentLevelIndex + 1,
	//	SpawnedCoinCount));
}

void ACH3GameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACH3GameState::OnCoinCollected(bool bCountsForLevelClear, int32 Amount)
{
	if (bIsLevelEnded)
	{
		return;
	}
	AddPoint(Amount);
	if (bCountsForLevelClear)
	{
		CollectedCoinCount++;

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Clear Coin Collected: %d / %d"),
		//		CollectedCoinCount,
		//		SpawnedCoinCount));

		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Bonus Coin Collected. + %d"), Amount));
	}
}

void ACH3GameState::EndLevel()
{
	if (bIsLevelEnded)
	{
		return;
	}

	bIsLevelEnded = true;
	CurrentLevelIndex++;
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CH3GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	bHasNextLevel = CurrentLevelIndex < MaxLevels;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->ShowGameResult(bHasNextLevel);
			UpdateGameResult();
		}
	}
	return;
}

void ACH3GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->ShowMainMenu(true);
		}
	}

	//디버깅메시지
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("GAME OVER !!!")));
}

void ACH3GameState::StartNextLevel()
{
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		FName NextLevelName = LevelMapNames[CurrentLevelIndex];

		UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
	}
	else
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
			{
				CH3PlayerController->StartGame();
			}
		}
	}
}

void ACH3GameState::AddTime(float Amount)
{
	RemainingTime += Amount;
}

void ACH3GameState::DecreaseTime(float DeltaTime)
{
	RemainingTime -= DeltaTime;
	if (RemainingTime <= 0.0f)
	{
		RemainingTime = 0;
	}
	if (RemainingTime <= 0.0f)
	{
		EndLevel();
		return;
	}
}

void ACH3GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLevelEnded)
	{
		return;
	}

	DecreaseTime(DeltaTime);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = Cast<APawn>(PlayerController->GetPawn()))
		{
			if (AFlyPawn* FlyPawn = Cast<AFlyPawn>(Pawn))
			{
				float CurrentZ = FlyPawn->GetActorLocation().Z;
				if (CurrentZ < -2500.0f)
				{
					bIsLevelEnded = true;
					OnGameOver();
				}
			}
		}
	}
}

void ACH3GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ACH3PlayerController* SpartaPlayerController = Cast<ACH3PlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainTime = FMath::Max(0.0f, RemainingTime);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), RemainTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCH3GameInstance* SpartaGameInstance = Cast<UCH3GameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					int32 DisplayLevel = FMath::Clamp(CurrentLevelIndex + 1, 1, MaxLevels);

					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), DisplayLevel)));
				}


				if (UTextBlock* FuelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Fuel"))))
				{
						if (APawn* Pawn = Cast<APawn>(PlayerController->GetPawn()))
						{
							if (AFlyPawn* FlyPawn = Cast<AFlyPawn>(Pawn))
							{
								FuelText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), FlyPawn->CurrentFuel)));
							}
						}
					}
				}
			}
		}
	}


void ACH3GameState::UpdateGameResult()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			if (UUserWidget* GameResultWidget = CH3PlayerController->GetGameResultWidget())
			{
				if (UTextBlock* ScoreText = Cast<UTextBlock>(GameResultWidget->GetWidgetFromName(TEXT("CollectedCoin"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
						if (CH3GameInstance)
						{
								ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CollectedCoinCount, SpawnedCoinCount)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(GameResultWidget->GetWidgetFromName(TEXT("Level"))))
				{
					int32 DisplayLevel = FMath::Clamp(CurrentLevelIndex, 1, MaxLevels);

					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), DisplayLevel)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(GameResultWidget->GetWidgetFromName(TEXT("TotalScore"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
						if (CH3GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CH3GameInstance->TotalScore)));
						}
					}
				}

			}
		}
	}
}