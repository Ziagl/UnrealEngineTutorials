// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
    UE_LOG(LogTemp, Warning, TEXT("ActorDied"));
    if (DeadActor == Tank)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tank"));
        Tank->HandleDestruction();
        if (Tank->GetTankPlayerController())
        {
            Tank->DisableInput(Tank->GetTankPlayerController());
            Tank->GetTankPlayerController()->bShowMouseCursor = false;
        }
    }
    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Tower"));
        DestroyedTower->HandleDestruction();
    }
}