// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/AircraftPawn.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/SpartaPlayerController.h"
#include "GameFramework/SpringArmComponent.h"


AAircraftPawn::AAircraftPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComp;  
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->SetRelativeLocation(FVector(0,0,40));
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
    
	NormalSpeed = 600.0f;
}

void AAircraftPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const FVector LocalOffset = FVector(MoveInput.X, MoveInput.Y, 0.f) * NormalSpeed * DeltaSeconds;
	AddActorLocalOffset(LocalOffset, true);
}

void AAircraftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AAircraftPawn::Move
				);
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AAircraftPawn::MoveCompleted
				);
			}
            
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AAircraftPawn::Look
				);
			}  
		}
	}
}

void AAircraftPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = value.Get<FVector2d>();
}

void AAircraftPawn::MoveCompleted(const FInputActionValue& value)
{
	MoveInput = FVector2d::ZeroVector;
}

void AAircraftPawn::Look(const FInputActionValue& value)
{
	const FVector2D LookAxis = value.Get<FVector2D>();
	AddActorLocalRotation(FRotator(LookAxis.Y, LookAxis.X, 0.f));
}

