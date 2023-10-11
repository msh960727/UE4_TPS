

#include "CBullet.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBullet::ACBullet()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->InitialSpeed = 2e+4f;
	Projectile->MaxSpeed = 2e+4f;
	Projectile->ProjectileGravityScale = 0.0f;


	InitialLifeSpan = 7.0f;

}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);

	for (USphereComponent* sphere : components)
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACBullet::OnComponentBeginOverlap);
	
}

void ACBullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!!Explosion)
	//{
	//	FTransform transform = ExplosionTransform;
	//	transform.AddToTranslation(SweepResult.ImpactPoint);
	//	transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	//}

	if (OnBulletBeginOverlap.IsBound())
		OnBulletBeginOverlap.Broadcast(SweepResult);

	Destroy();
}

