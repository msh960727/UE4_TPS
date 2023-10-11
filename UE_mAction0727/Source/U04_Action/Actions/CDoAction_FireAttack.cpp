

#include "CDoAction_FireAttack.h"
#include "Global.h"
#include "CAim.h"
#include "CBullet.h"
#include "Barriers/CBarrier_Mirror.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/StaticMeshActor.h"
#include "Actions/CAttachment.h"

void ACDoAction_FireAttack::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	

}

void ACDoAction_FireAttack::DoAction()
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

void ACDoAction_FireAttack::Begin_DoAction()
{





	FVector location = Rifle->GetSocketLocation("MuzzleFlash");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;
	params.Owner = this;
	
	ACBullet* bulletObject = OwnerCharacter->GetWorld()->SpawnActor<ACBullet>(Datas[0].BulletClass, transform, params);	
	bulletObject->OnBulletBeginOverlap.AddDynamic(this, &ACDoAction_FireAttack::OnBulletBeginOverlap);

	TSubclassOf<UCameraShakeBase> shake = Datas[0].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);


	ACPlayer* rifle = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(rifle);

	FVector start, end, direction;
	rifle->GetLocationAndDirection(start, end, direction);

	//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 3.0f);

	//UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "MuzzleFlash", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	//UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "EjectBullet", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, muzzleLocation);

	FCollisionQueryParams colliparams;
	colliparams.AddIgnoredActor(Rifle->GetOwner());
	colliparams.AddIgnoredActor(OwnerCharacter);

	FHitResult hitResult;

	/*if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, colliparams))
	{
		FRotator rotator = hitResult.ImpactNormal.Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, hitResult.Location, rotator, FVector(2));
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(5), hitResult.Location, rotator, 10.0f);
	}*/

	//if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, colliparams))
	//{
	//	AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
	//	if (!!staticMeshActor)
	//	{
	//		USkeletalMeshComponent* skeletalmeshComponent = Cast<USkeletalMeshComponent>(staticMeshActor->GetRootComponent());
	//		if (!!skeletalmeshComponent)
	//		{
	//			if (skeletalmeshComponent->BodyInstance.bSimulatePhysics)
	//			{

	//				OnBulletBeginOverlap(hitResult);

	//				/*direction = staticMeshActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
	//				direction.Normalize();

	//				meshComponent->AddImpulseAtLocation(direction * meshComponent->GetMass() * 100, OwnerCharacter->GetActorLocation());*/

	//				return;
	//			}
	//		}			
	//		//if (staticMeshActor->IsA<ACBarrier_Mirror>() && staticMeshActor->GetActorLabel().Contains("Barrier"))
	//		//{

	//		//}//if (!!meshComponent)
	//	}//if (!!staticMeshActor)
	//}

	return;



}

void ACDoAction_FireAttack::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_FireAttack::OnAim()
{
	Aim->OnZoom();
}

void ACDoAction_FireAttack::OffAim()
{
	Aim->OffZoom();
}

void ACDoAction_FireAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(*bEquipped);

	Aim->Tick(DeltaTime);

	//ACPlayer* rifle = Cast<ACPlayer>(OwnerCharacter);
	//CheckNull(rifle);

	//FVector start, end, direction;
	//rifle->GetLocationAndDirection(start, end, direction);

	//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 3.0f);

	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);
	//params.AddIgnoredActor(OwnerCharacter);

	//FHitResult hitResult;

	//if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
	//{
	//	ACBarrier_Mirror* HittedBarrier;
	//	HittedBarrier = Cast<ACBarrier_Mirror>(hitResult.GetActor());
	//	if (!!HittedBarrier)
	//	{
	//		FVector hitpoint, endline, OutDirection, InDirection;
	//		
	//		
	//		hitpoint = hitResult.ImpactPoint;
	//		InDirection = hitpoint - start;
	//		OutDirection = (hitResult.Normal * 2.0f * (-InDirection | hitResult.Normal) + InDirection).GetSafeNormal();
	//		

	//		FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	//		conDirection *= 5000.0f;

	//		endline = hitpoint + conDirection;

	//		DrawDebugLine(GetWorld(), hitpoint, endline, FColor::Red, false, 3.0f);


	//		return;

	//		//}//if (!!meshComponent)
	//	}//if (!!staticMeshActor)
	//}


	//if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
	//{
	//	//PrintLine();


	//	AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
	//	if (!!staticMeshActor)
	//	{
	//		//PrintLine();



	//		UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
	//		if (!!meshComponent)
	//		{
	//			//PrintLine();



	//			if (meshComponent->BodyInstance.bSimulatePhysics)
	//			{
	//				rifle->OnAim();

	//				//PrintLine();


	//				return;
	//			}
	//		}//if (!!meshComponent)
	//	}//if (!!staticMeshActor)
	//}

	//rifle->OffAim();
}

void ACDoAction_FireAttack::OnBulletBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

//void ACDoAction_FireAttack::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
//{
//	if (OnBarrierBeginOverlap.IsBound())
//		OnBarrierBeginOverlap.Broadcast(SweepResult);
//}
