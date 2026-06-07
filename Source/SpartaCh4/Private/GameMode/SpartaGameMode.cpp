// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SpartaGameMode.h"
#include "Character/SpartaCharacter.h"
#include "Controller/SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
}
