// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/HexGamePlay/HexPawn.h"
#include "UwsToyCppTP/HexMap/HexGamePlay/HexController.h"
#include "UwsToyCppTP/HexMap/UserWidgets/HexSeleteColor.h"
#include "UwsToyCppTP/HexMap/Hex/HexGrid.h"
#include "UwsToyCppTP/HexMap/HexGamePlay/HexMapEditor.h"
#include "UwsToyCppTP/HexMap/Hex/HexGridChunk.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AHexPawn::AHexPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(SceneComp);
	CameraBoom->TargetArmLength = 300.0f; // 	
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	HexMapEditorComp = CreateDefaultSubobject<UHexMapEditor>(TEXT("HexMapEditor"));
	UWidgetSelColor = CreateDefaultSubobject<UHexSeleteColor>(TEXT("UWidgetSelColor"));

	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AHexPawn::BeginPlay()
{
	Super::BeginPlay();
	UWidgetSelColor->AddToViewport();
	PawnController = Cast<AHexController>(GetController());
	//PawnController = UGameplayStatics::GetPlayerController(GetWorld(), 0);// uws
	if (PawnController)
	{
		PawnController->bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetWidgetToFocus(nullptr);

		PawnController->SetInputMode(InputMode);
	}
	
}

// Called every frame
void AHexPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHexPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);;
	check(Subsystem);

	Subsystem->AddMappingContext(HexMappingContext, 0);
	
	if (UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComp->BindAction(IA_ViewportMoveFront, ETriggerEvent::Triggered, this, &AHexPawn::Input_ViewportMoveFront);
		EnhancedInputComp->BindAction(IA_ViewportMoveRight, ETriggerEvent::Triggered, this, &AHexPawn::Input_ViewportMoveRight);
		EnhancedInputComp->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AHexPawn::Input_Turn);
		EnhancedInputComp->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AHexPawn::Input_LookUp);
		EnhancedInputComp->BindAction(IA_ViewportZoom, ETriggerEvent::Triggered, this, &AHexPawn::Input_ViewportZoom);
		EnhancedInputComp->BindAction(IA_LeftMoouseClick, ETriggerEvent::Started, this, &AHexPawn::Input_LeftMouseClick);
	}

}

void AHexPawn::Input_ViewportMoveFront(const FInputActionValue& InputActionValue)
{
	// uws 
	/*const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	FVector MoveOffset = FVector(MoveValue.X, MoveValue.Y, 0.f) * MoveSpeed * GetWorld()->GetDeltaSeconds();

	AddActorWorldOffset(MoveOffset, true);*/

	// uws to do
	if (Controller != nullptr)
	{
		float Val = InputActionValue.Get<float>();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddActorWorldOffset(Direction * MoveSpeed * Val);
	}
	
}

void AHexPawn::Input_ViewportMoveRight(const FInputActionValue& InputActionValue)
{
	if (Controller != nullptr)
	{
		
		float Val = InputActionValue.Get<float>();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddActorWorldOffset(Direction * MoveSpeed * Val);
	}
}

void AHexPawn::Input_Turn(const FInputActionValue& InputActionValue)
{
	FVector2D Val = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(Val.X  * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AHexPawn::Input_LookUp(const FInputActionValue& InputActionValue)
{
	FVector2D Val = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(Val.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHexPawn::Input_ViewportZoom(const FInputActionValue& InputActionValue)
{
	//if (!CameraBoom) return;
	float ZoomValue = InputActionValue.Get<float>();
	float MinArmLength = 50.f;
	float MaxArmLength = 5000.f;

	float NewArmLength = FMath::Clamp(CameraBoom->TargetArmLength + (ZoomValue * ZoomSpeed), MinArmLength, MaxArmLength);
	CameraBoom->TargetArmLength = NewArmLength;
}

void AHexPawn::Input_LeftMouseClick(const FInputActionValue& InputActionValue)
{
	GetMousePositionInstance();
}

void AHexPawn::GetMousePositionInstance()
{
	if (PawnController)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("GetMousePosiont")); // uws
		float LocationX, LocationY;
		PawnController->GetMousePosition(LocationX, LocationY);

		FVector WorldLocation, WorldDirection;
		if (PawnController->DeprojectScreenPositionToWorld(LocationX, LocationY, WorldLocation, WorldDirection))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DeprojectScreenPositionToWorld"));
			FHitResult HitResult;

			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(PawnController->GetPawn());

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
			
			if (GetWorld()->LineTraceSingleByObjectType(HitResult, WorldLocation, WorldLocation + WorldDirection * 2000.f
				, ObjectQueryParams, CollisionQueryParams))
			{
				//if (Cast<AHexGrid>(HitResult.GetActor()))
				if(HitResult.GetActor() && HitResult.GetActor()->IsA(AHexGridChunk::StaticClass()))
				{
					//LeftMouseDelegate.ExecuteIfBound(HitResult.Location);// for now not use that delegate
					//HexMapEditorComp->EditCell(HitResult.Location);
					UHexCellComponent* Center = HexMapEditorComp->GetCellFromLocation(HitResult.Location);
					HexMapEditorComp->EditCells(Center);
				}
			
				//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitResult.Location.ToString());// uws
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("HitLocation: %s"), *HitResult.Location.ToString())); // uws
				
			}
		
		}
	}
}

