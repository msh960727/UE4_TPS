

#include "CBarrier_Mirror.h"
#include "Global.h"
#include "Actions/CDoAction.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Actions/CBullet.h"

ACBarrier_Mirror::ACBarrier_Mirror()
{
	//PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;


	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/Cube.Cube'");
	Mesh->SetStaticMesh(mesh);



	MaxCount = 3;
	HitCount = 0;

}

//void ACBarrier_Mirror::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
//{
//	FHitResult HitMirror;
//	
//	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
//	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//	RootComponent = Mesh;
//
//	FCollisionObjectQueryParams params;
//	params.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);
//
//	if (GetWorld()->LineTraceSingleByObjectType(HitMirror, OutStart, OutEnd, params))
//	{
//		OutDirection = (HitMirror.Normal * 2.0f * (-OutDirection | HitMirror.Normal) + OutDirection).GetSafeNormal();
//		OutStart = HitMirror.ImpactPoint;
//		CLog::Log("Hi");
//		FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
//		conDirection *= 5000.0f;
//		OutEnd = OutStart + conDirection;
//	}
//
//
//
//}

//void ACBarrier_Mirror::ReflectionDir(FHitResult& HitResult)
//{
//}

void ACBarrier_Mirror::ChangeMaterial()
{
	HitCount++;

	if (roughness >= 0.0f && MaxCount > 0)
	{
		metallic = (HitCount / (float)MaxCount);
		roughness = 1.0f - metallic;
	}
	if(MaxCount == HitCount)
	{
		metallic = 1.0f;
		roughness = 0.0f;
		HitCount = 0;
	}
	
	Material->SetScalarParameterValue("Metallic", metallic);
	Material->SetScalarParameterValue("Roughness", roughness);


}

void ACBarrier_Mirror::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/Mirror_Inst.Mirror_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Mesh->SetMaterial(0, Material);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACBarrier_Mirror::OnComponentBeginOverlap);


}

void ACBarrier_Mirror::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	FName path = TEXT("Blueprint'/Game/Actions/BP_CBullet_FireAttack.BP_CBullet_FireAttack_C'");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));

	FActorSpawnParameters params;
	params.Owner = this;

	FVector OutDirection, Start, End, Direction;

	Start = SweepResult.TraceStart;
	End = SweepResult.ImpactPoint;
	Direction = End - Start;
	OutDirection = (SweepResult.ImpactNormal * 2.0f * (-Direction | SweepResult.ImpactNormal) + Direction).GetSafeNormal();

	GetWorld()->SpawnActor<ACBullet>(GeneratedBP, SweepResult.ImpactPoint, OutDirection.Rotation(), params);
	/*ACBullet* bulletObject = */



}



//void ACBarrier_Mirror::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	
//	if (!!OnComponentBeginOverlap)
//	{
//
//	}
//
//	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
//}

