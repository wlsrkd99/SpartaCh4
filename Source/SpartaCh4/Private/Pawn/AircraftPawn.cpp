// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/AircraftPawn.h"

// Sets default values
AAircraftPawn::AAircraftPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAircraftPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAircraftPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAircraftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

