
#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_FireAttack.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBarrierBeginOverlap, FHitResult, InHitResult);
UCLASS()
class U04_ACTION_API ACDoAction_FireAttack : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void OnAim() override;
	virtual void OffAim() override;


	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		virtual void OnBulletBeginOverlap(FHitResult InHitResult);

//protected:
//	UFUNCTION()
//		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
//
//	UFUNCTION()
//		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	class UCAim* Aim;
	class USkeletalMeshComponent* Rifle;
	class ACBarrier_Mirror* Barrier;
	
	
//private:
//	UFUNCTION()
//		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//public:
//	UPROPERTY(BlueprintAssignable)
//		FBarrierBeginOverlap OnBarrierBeginOverlap;

};
