// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyGameCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AStrategyGameCamera::AStrategyGameCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// Attach our components
	StaticMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// Assign SpringArm class variables.
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-30.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	// Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AStrategyGameCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStrategyGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		//CameraComp->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		SpringArmComp->TargetArmLength = FMath::Lerp<float>(MaxHeight, MinHeight, ZoomFactor);

		//Handle movement based on our "MoveX" and "MoveY" axes
		{
			if (!MovementInput.IsZero())
			{
				//Scale our movement input axis values by 100 units per second
				MovementInput = MovementInput.GetSafeNormal() * 100.0f * Speed * ((1.0f - ZoomFactor) * 10.0f);
				FVector NewLocation = GetActorLocation();
				NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
				NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
				SetActorLocation(NewLocation);
			}
		}
	}
}

// Called to bind functionality to input
void AStrategyGameCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &AStrategyGameCamera::MoveForward);
	InputComponent->BindAxis("MoveLeft", this, &AStrategyGameCamera::MoveLeft);
	InputComponent->BindAxis("Zoom", this, &AStrategyGameCamera::Zoom);
}

//Input functions
void AStrategyGameCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AStrategyGameCamera::MoveLeft(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void AStrategyGameCamera::Zoom(float AxisValue)
{
	ZoomFactor += FMath::Clamp<float>(AxisValue, -1.0f, 1.0f) * 0.1f;
}
