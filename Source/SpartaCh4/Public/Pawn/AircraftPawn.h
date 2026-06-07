// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AircraftPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SPARTACH4_API AAircraftPawn : public APawn
{
	GENERATED_BODY()

public:
	AAircraftPawn();
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement")
	float NormalSpeed;
	
private:
	void Move(const FInputActionValue& value);
	void MoveCompleted(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	
	FVector2D MoveInput;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Camera")
	UCapsuleComponent * CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Camera")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Camera")
	UCameraComponent* CameraComp;
	
	
};
