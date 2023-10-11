// Fill out your copyright notice in the Description page of Project Settings.


#include "CDoAction_ChangeMaterial.h"
#include "Global.h"
#include "CAim.h"
#include "CBullet.h"
#include "CBullet_ChangeMaterial.h"
#include "Barriers/CBarrier_Mirror.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMeshActor.h"
#include "Actions/CAttachment.h"

void ACDoAction_ChangeMaterial::DoAction()
{
	Super::DoAction();

	CheckFalse(*bEquipped);

	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZoom());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();


	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Attack"))
		{
			Rifle = CHelpers::GetComponent<USkeletalMeshComponent>(actor);

			break;
		}
	}
}

void ACDoAction_ChangeMaterial::Begin_DoAction()
{

	FVector location = Rifle->GetSocketLocation("MuzzleFlash");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = this;

	ACBullet_ChangeMaterial* bulletObject = OwnerCharacter->GetWorld()->SpawnActor<ACBullet_ChangeMaterial>(Datas[0].BulletClass, transform, params);
	//bulletObject->OnChangeBulletBeginOverlap.AddDynamic(this, &ACDoAction_ChangeMaterial::OnBulletBeginOverlap);

	/*TSubclassOf<UCameraShakeBase> shake = Datas[0].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);*/

	ACPlayer* rifle = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(rifle);

	FVector start, end, direction;
	rifle->GetLocationAndDirection(start, end, direction);

	//UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "MuzzleFlash", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "EjectBullet", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);

	FCollisionQueryParams colliparams;
	colliparams.AddIgnoredActor(Rifle->GetOwner());
	colliparams.AddIgnoredActor(OwnerCharacter);

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, colliparams))
	{
		ACBarrier_Mirror* HittedBarrier;
		HittedBarrier = Cast<ACBarrier_Mirror>(hitResult.GetActor());
		if (!!HittedBarrier)
		{
			OnBulletBeginOverlap(hitResult);

			return;

			//}//if (!!meshComponent)
		}//if (!!staticMeshActor)
	}
}

void ACDoAction_ChangeMaterial::OnBulletBeginOverlap(FHitResult InHitResult)
{

	Barrier = Cast<ACBarrier_Mirror>(InHitResult.GetActor());
	Barrier->ChangeMaterial();
}
