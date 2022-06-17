// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StrategyGameCamera.generated.h"

UCLASS()
class HEXAGON_API AStrategyGameCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStrategyGameCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxHeight;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinHeight;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomFactor;

	//Input variables
	FVector2D MovementInput;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveLeft(float AxisValue);
	void Zoom(float AxisValue);
};
