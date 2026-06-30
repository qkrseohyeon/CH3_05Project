// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameMode.h"
#include "CH3Pawn.h"
#include "CH3GameMode.h"
#include "CH3PlayerController.h"

ACH3GameMode::ACH3GameMode()
{
	DefaultPawnClass = ACH3Pawn::StaticClass();
	PlayerControllerClass = ACH3PlayerController::StaticClass();
}

