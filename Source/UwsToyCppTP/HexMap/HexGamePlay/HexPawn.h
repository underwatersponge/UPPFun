// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HexPawn.generated.h"

class UHexSeleteColor;

DECLARE_DELEGATE_OneParam(FLeftMouseDelegate, FVector);

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UHexMapEditor;
struct FInputActionValue;

UCLASS()
class UWSTOYCPPTP_API AHexPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHexPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FLeftMouseDelegate LeftMouseDelegate;
	FLeftMouseDelegate LegtouseClickDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	APlayerController* PawnController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UWidget");
	UHexSeleteColor* UWidgetSelColor;

	UPROPERTY(BlueprintReadOnly, Category = "Hex");
	UHexMapEditor* HexMapEditorComp;

	// input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (Categories = "InputInfo"))
	UInputMappingContext* HexMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_ViewportMoveFront;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_ViewportMoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_ViewportZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	UInputAction* IA_LeftMoouseClick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	float TurnRate = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	float LookUpRate = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), meta = (Categories = "InputInfo"))
	float ZoomSpeed = 100.f;

protected:
	UFUNCTION()
	void Input_ViewportMoveFront(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_ViewportMoveRight(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_Turn(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_LookUp(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void Input_ViewportZoom(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void Input_LeftMouseClick(const FInputActionValue& InputActionValue);

	void GetMousePositionInstance();
};


