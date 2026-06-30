// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyPawn.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class CH3_04PROJECT_API AFlyPawn : public APawn
{
	GENERATED_BODY()

public:
	AFlyPawn();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	void AddFuel(float Amount);
	float CurrentZ;

	//¿¬·á 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float CurrentFuel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float MaxFuel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fuel")
	float FuelConsumptionRate;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveAction(const FInputActionValue& value);
	void StopMoveAction(const FInputActionValue& value);
	void LookAction(const FInputActionValue& value);
	void FlyAction(const FInputActionValue& value);
	void StopFlyAction(const FInputActionValue& value);
	void RollAction(const FInputActionValue& value);
	void StopRollAction(const FInputActionValue& value);

	FVector2D CurrentMoveInput;
	FVector2D CurrentLookInput;
	float CurrentFlyInput;
	float CurrentRollInput;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	float RotationSpeed;

	float GravityAcceleration;
	float VerticalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	float LineTraceLength;

	void ConsumeFuel(float Amount);

};
