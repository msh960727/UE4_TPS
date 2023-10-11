
#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/CUserWidget_ActionList.h"


ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, 0, -90));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;


	CHelpers::GetClass<UCUserWidget_ActionList>(&ActionListClass, "WidgetBlueprint'/Game/Widgets/WB_ActionList.WB_ActionList_C'");
}

void ACPlayer::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	OutDirection = Camera->GetForwardVector();

	FTransform transform = Camera->GetComponentToWorld();
	FVector cameraLocation = transform.GetLocation();
	OutStart = cameraLocation + OutDirection;

	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	conDirection *= 5000.0f;

	OutEnd = cameraLocation + conDirection;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetUnarmedMode();

	ActionList = CreateWidget<UCUserWidget_ActionList, APlayerController>(GetController<APlayerController>(), ActionListClass);
	ActionList->AddToViewport();
	ActionList->SetVisibility(ESlateVisibility::Hidden);

	ActionList->GetData(0).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFist);
	ActionList->GetData(1).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnOneHand);
	ActionList->GetData(2).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnTwoHand);
	ActionList->GetData(3).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnWarp);
	ActionList->GetData(4).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFireStorm);
	ActionList->GetData(5).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnIceBall);


}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);
	
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);

	PlayerInputComponent->BindAction("FireStorm", EInputEvent::IE_Pressed, this, &ACPlayer::OnFireStorm);

	PlayerInputComponent->BindAction("IceBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnIceBall);
	
	PlayerInputComponent->BindAction("FireAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnFireAttack);
	PlayerInputComponent->BindAction("ChangeMaterial", EInputEvent::IE_Pressed, this, &ACPlayer::OnChangeMaterial);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, this, &ACPlayer::OnTarget);
	PlayerInputComponent->BindAction("TargetLeft", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetLeft);
	PlayerInputComponent->BindAction("TargetRight", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetRight);


	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewActionList);
	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Released, this, &ACPlayer::OffViewActionList);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	//CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	//CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnAvoid()
{
	//CheckFalse(Status->CanMove());
	//CheckFalse(State->IsIdleMode());

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackstepMode();

		return;
	}

	State->SetRollMode();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll:  Begin_Roll(); break;
		case EStateType::Backstep: Begin_Backstep(); break;
	}

}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();

}

void ACPlayer::End_Roll()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	State->SetIdleMode();
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackstep();

}


void ACPlayer::End_Backstep()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	State->SetIdleMode();
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnFireStorm()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFireStormMode();
}

void ACPlayer::OnIceBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetIceBallMode();
}

void ACPlayer::OnFireAttack()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFireAttackMode();
}

void ACPlayer::OnChangeMaterial()
{
	CheckFalse(State->IsIdleMode());

	Action->SetChangeMaterialMode();
}



void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::OnTarget()
{
	Target->ToggleTarget();
}

void ACPlayer::OnTargetLeft()
{
	Target->ChangeTargetLeft();
}

void ACPlayer::OnTargetRight()
{
	Target->ChangeTargetRight();
}

void ACPlayer::OnAim()
{
	Action->DoAim();
}

void ACPlayer::OffAim()
{
	Action->UndoAim();
}

void ACPlayer::OnViewActionList()
{
	CheckFalse(State->IsIdleMode());

	ActionList->SetVisibility(ESlateVisibility::Visible);

	GetController<APlayerController>()->bShowMouseCursor = true;
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffViewActionList()
{
	ActionList->SetVisibility(ESlateVisibility::Hidden);

	GetController<APlayerController>()->bShowMouseCursor = false;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}
