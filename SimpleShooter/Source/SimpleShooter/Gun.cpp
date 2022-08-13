// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

		// draw debug camera of viewport
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn == nullptr) return;
		AController* OwnerController = OwnerPawn->GetController();
		if (OwnerController == nullptr) return;
		FVector Location;
		FRotator Rotation;
		OwnerController->GetPlayerViewPoint(Location, Rotation);

		FVector End = Location + Rotation.Vector() * MaxRange;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());
		bool bSuccess = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			Location, 
			End, 
			ECollisionChannel::ECC_GameTraceChannel1,
			Params
		);
		if (bSuccess)
		{
			FVector ShotDirection = -Rotation.Vector();
			// DrawDebugPoint(GetWorld(), HitResult.Location, 20, FColor::Red, true);
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactEffect,
				HitResult.Location,
				ShotDirection.Rotation()
			);
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				FPointDamageEvent DamageEvent(
					Damage,
					HitResult,
					ShotDirection,
					nullptr
				);
				HitActor->TakeDamage(
					Damage,
					DamageEvent,
					OwnerController,
					this
				);
			}

		}

		// DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

