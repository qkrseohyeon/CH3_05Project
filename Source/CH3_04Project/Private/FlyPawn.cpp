// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CH3PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CH3GameState.h"

// Sets default values
AFlyPawn::AFlyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
	StaticMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 300.0f;
	RotationSpeed = 60.0f;

	GravityAcceleration = -980.0f;
	VerticalSpeed = 0.0f;
	LineTraceLength = 100.0f;

	//연료
	CurrentFuel = 0.0f;
	MaxFuel = 100.0f;
	FuelConsumptionRate = 10.0f;

}

void AFlyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACH3PlayerController* PlayerController = Cast<ACH3PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AFlyPawn::MoveAction);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &AFlyPawn::StopMoveAction);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AFlyPawn::LookAction);
			}
			if (PlayerController->FlyAction)
			{
				EnhancedInput->BindAction(PlayerController->FlyAction, ETriggerEvent::Triggered, this, &AFlyPawn::FlyAction);
			}
			if (PlayerController->FlyAction)
			{
				EnhancedInput->BindAction(PlayerController->FlyAction, ETriggerEvent::Completed, this, &AFlyPawn::StopFlyAction);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(PlayerController->RollAction, ETriggerEvent::Triggered, this, &AFlyPawn::RollAction);
			}
			if (PlayerController->RollAction)
			{
				EnhancedInput->BindAction(PlayerController->RollAction, ETriggerEvent::Completed, this, &AFlyPawn::StopRollAction);
			}
		}
	}
}

void AFlyPawn::MoveAction(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	CurrentMoveInput = MoveInput;
}

void AFlyPawn::StopMoveAction(const FInputActionValue& value)
{
	CurrentMoveInput = FVector2D::ZeroVector;
}

void AFlyPawn::LookAction(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	CurrentLookInput = LookInput;
}

void AFlyPawn::FlyAction(const FInputActionValue& value)
{
	if (!Controller) return;
	const float FlyInput = value.Get<float>();

	CurrentFlyInput = FlyInput;
}

void AFlyPawn::StopFlyAction(const FInputActionValue& value)
{
	CurrentFlyInput = 0.0f;
}

void AFlyPawn::RollAction(const FInputActionValue& value)
{
	const float RollInput = value.Get<float>();

	CurrentRollInput = RollInput;
}

void AFlyPawn::StopRollAction(const FInputActionValue& value)
{
	CurrentRollInput = 0.0f;
}

void AFlyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Controller) return;

	float DeltaMoveInputX = CurrentMoveInput.X * DeltaTime * MoveSpeed;
	float DeltaMoveInputY = CurrentMoveInput.Y * DeltaTime * MoveSpeed;

	const FVector DeltaMove = FVector(DeltaMoveInputX, DeltaMoveInputY, 0.0f);

	AddActorLocalOffset(DeltaMove);

	bool bIsGrounded = false;
	FHitResult FloorHitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0.0f, 0.0f, LineTraceLength);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bIsGrounded = GetWorld()->LineTraceSingleByChannel(
		FloorHitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	//DrawDebugLine(
		//GetWorld(),
		//Start,
		//End,
		//bIsGrounded ? FColor::Green : FColor::Red,
		//false,
		//0.0f,
		//0,
		//2.0f
	//);

	float DeltaFlyInputZ = CurrentFlyInput * DeltaTime * MoveSpeed;
	if (CurrentFuel <= 0.0f)
	{
		DeltaFlyInputZ = 0.0f;
	}

	if (bIsGrounded || (CurrentFlyInput > 0.0f && CurrentFuel > 0.0f))
	{
		VerticalSpeed = 0.0f;
	}
	else
	{
		VerticalSpeed += GravityAcceleration * DeltaTime;
	}

	float GravityMoveZ = VerticalSpeed * DeltaTime;
	float TotalMoveZ = DeltaFlyInputZ + GravityMoveZ;
	if (bIsGrounded && TotalMoveZ < 0.0f)
	{
		TotalMoveZ = 0.0f;
	}
	AddActorWorldOffset(FVector(0.0f, 0.0f, TotalMoveZ));

	if (TotalMoveZ > 0)
	{
		ConsumeFuel(FuelConsumptionRate * DeltaTime);
	}

	CurrentZ = GetActorLocation().Z;

	float DeltaLookInputX = CurrentLookInput.X * DeltaTime * RotationSpeed;
	float DeltaLookInputY = CurrentLookInput.Y * DeltaTime * RotationSpeed;
	float DeltaRollInput = CurrentRollInput * DeltaTime * RotationSpeed;

	const FRotator DeltaLook = FRotator(DeltaLookInputY, 0.0f, 0.0f);

	SpringArmComp->AddLocalRotation(DeltaLook);
	AddActorLocalRotation(FRotator(0.0f, DeltaLookInputX, DeltaRollInput));

	CurrentLookInput = FVector2D::ZeroVector;

	//GEngine->AddOnScreenDebugMessage(	2,0.0f,FColor::Magenta,FString::Printf(TEXT("Remain Fuel: %.1f"), CurrentFuel));
}
	
void AFlyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlyPawn::AddFuel(float Amount)
{
	CurrentFuel += Amount;
	//디버깅메시지
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("Current Fuel: %f"),CurrentFuel));
}

void AFlyPawn::ConsumeFuel(float Amount)
{
	CurrentFuel -= Amount;
}





