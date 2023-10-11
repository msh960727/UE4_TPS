// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CBullet.h"
#include "CBullet_ChangeMaterial.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeBulletBeginOverlap, FHitResult, InHitResult);

UCLASS()
class U04_ACTION_API ACBullet_ChangeMaterial : public ACBullet
{
	GENERATED_BODY()

public:
	ACBullet_ChangeMaterial();

protected:
	virtual void BeginPlay() override;

protected:
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	UPROPERTY(BlueprintAssignable)
		FChangeBulletBeginOverlap OnChangeBulletBeginOverlap;
	
};
