// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/AircraftPawn.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/AircaftPlayerController.h"
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
	RotationSpeed = 50.f;
	MouseWheelRollAmount = 5.f;
	Gravity = -980.f;

	MoveInput = FVector3d::ZeroVector;
	LookInput = FVector2D::ZeroVector;
	RollKeyInput = 0.f;
	FallSpeed = 0.f;
}

void AAircraftPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const FVector LocalOffset = MoveInput * NormalSpeed * DeltaSeconds;
	AddActorLocalOffset(LocalOffset, true);

	FRotator DeltaRotation(LookInput.Y * RotationSpeed * DeltaSeconds, LookInput.X * RotationSpeed * DeltaSeconds, RollKeyInput * RotationSpeed * DeltaSeconds);
	AddActorLocalRotation(DeltaRotation, true);
	
	ApplyGravity(DeltaSeconds);
}

void AAircraftPawn::ApplyGravity(float DeltaSeconds)
{
	if (MoveInput.IsNearlyZero())
	{
		FHitResult HitResult;
		const float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
		const FVector TraceStart = GetActorLocation();
		const FVector TraceEnd = TraceStart - FVector(0.f, 0.f, CapsuleHalfHeight + 5.f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		const bool bIsOnGround = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			ECollisionChannel::ECC_Visibility,
			CollisionParams
		);

		if (bIsOnGround)
		{
			FallSpeed = 0.f;
		}
		else
		{
			FallSpeed += Gravity * DeltaSeconds;
		}
	
		AddActorWorldOffset(FVector(0.f, 0.f, FallSpeed * DeltaSeconds), true);
	}
	else
	{
		FallSpeed = 0.f;
	}
}

void AAircraftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AAircaftPlayerController* PlayerController = Cast<AAircaftPlayerController>(GetController()))
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
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&AAircraftPawn::LookCompleted
				);
			}

			if (PlayerController->RollKeyAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollKeyAction,
					ETriggerEvent::Triggered,
					this,
					&AAircraftPawn::RollByKey
				);
				EnhancedInput->BindAction(
					PlayerController->RollKeyAction,
					ETriggerEvent::Completed,
					this,
					&AAircraftPawn::RollByKeyCompleted
				);
			}

			if (PlayerController->RollMouseAction)
			{
				EnhancedInput->BindAction(
					PlayerController->RollMouseAction,
					ETriggerEvent::Triggered,
					this, 
					&AAircraftPawn::RollByMouse
				);
			}
		}
	}
}

void AAircraftPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	MoveInput = value.Get<FVector3d>();
}

void AAircraftPawn::MoveCompleted(const FInputActionValue& value)
{
	MoveInput = FVector3d::ZeroVector;
}

void AAircraftPawn::Look(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}

void AAircraftPawn::LookCompleted(const FInputActionValue& value)
{
	LookInput = FVector2D::ZeroVector;
}

void AAircraftPawn::RollByKey(const FInputActionValue& value)
{
	RollKeyInput = value.Get<float>();
}

void AAircraftPawn::RollByKeyCompleted(const FInputActionValue& value)
{
	RollKeyInput = 0.f;
}

void AAircraftPawn::RollByMouse(const FInputActionValue& value)
{
	const float RollValue = value.Get<float>();
	AddActorLocalRotation(FRotator(0.f, 0.f, RollValue * MouseWheelRollAmount));
}
