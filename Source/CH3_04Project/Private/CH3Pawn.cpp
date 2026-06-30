// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CH3PlayerController.h"
#include "EnhancedInputComponent.h"



ACH3Pawn::ACH3Pawn()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 300.0f;
	RotationSpeed = 90.0f;

}


void ACH3Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACH3Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ACH3PlayerController* PlayerController = Cast<ACH3PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ACH3Pawn::MoveAction);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ACH3Pawn::StopMoveAction);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ACH3Pawn::LookAction);
			}
		}
	}
}

void ACH3Pawn::MoveAction(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveInput = value.Get<FVector2D>();

	CurrentMoveInput = MoveInput;
}

void ACH3Pawn::StopMoveAction(const FInputActionValue& value)
{
	CurrentMoveInput = FVector2D::ZeroVector;
}

void ACH3Pawn::LookAction(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	CurrentLookInput = LookInput;
}

void ACH3Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Controller) return;

	float DeltaMoveInputX = CurrentMoveInput.X * DeltaTime * MoveSpeed;
	float DeltaMoveInputY = CurrentMoveInput.Y * DeltaTime * MoveSpeed;

	const FVector DeltaMove = FVector(DeltaMoveInputX, DeltaMoveInputY, 0.0f);

	AddActorLocalOffset(FVector(DeltaMove));

	float DeltaLookInputX = CurrentLookInput.X * DeltaTime * RotationSpeed;
	float DeltaLookInputY = CurrentLookInput.Y * DeltaTime * RotationSpeed;

	const FRotator DeltaLook = FRotator(DeltaLookInputY,0.0f, 0.0f);

	SpringArmComp->AddLocalRotation(DeltaLook);
	AddActorLocalRotation(FRotator(0.0f, DeltaLookInputX, 0.0f));

	CurrentLookInput = FVector2D::ZeroVector;
}