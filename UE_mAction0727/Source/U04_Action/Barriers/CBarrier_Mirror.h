
#pragma once

#include "CoreMinimal.h"
//#include "ITrace.h"
#include "GameFramework/Actor.h"
#include "CBarrier_Mirror.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLightBeginOverlap, FHitResult, InHitResult);

UCLASS()
class U04_ACTION_API ACBarrier_Mirror : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	
public:	
	ACBarrier_Mirror();

	//void ReflectionDir(FHitResult& HitResult);
	void ChangeMaterial();

private:
	class UMaterialInstanceDynamic* Material;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
		virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(BlueprintAssignable)
		FLightBeginOverlap OnLightBeginOverlap;

	UPROPERTY(VisibleDefaultsOnly)
		TSubclassOf<class ACBullet> BulletClass;

public:	
	//virtual void Tick(float DeltaTime) override;

protected:
	float roughness = 1.0f;
	float metallic = 0.0f;
	int MaxCount;
	int HitCount;

	

};
