// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Collision mesh component
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision mesh"));
	SetRootComponent(CollisionMesh);
	// Trigger collision for physics engine (for on hit event broadcast)
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true); //TODO change to false

	// Particle effect for launch
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch blast"));
	LaunchBlast->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	// Auto activates by default
	
	// Particle effect for impact
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->SetAutoActivate(false);

	// Movement component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile movement"));
	ProjectileMovement->bAutoActivate = false;

	// Radial force component for explosive force on impact
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	// subscribe to on hit event
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

// called by physics system when projectile collides with something
void AProjectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	// Switch to impact particle effect
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	// Apply force
	ExplosionForce->FireImpulse();
	// Change root component from mesh before destroying it
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Apply damage in radius
	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GetActorLocation(),
		ExplosionForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>() // damage all actors
	);

	// Destroy projectile after delay
	FTimerHandle TimerHandle; // Out parameter needed for function call
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AProjectile::DestroyProjectile,
		DestroyDelay,
		false
	);
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * LaunchSpeed);
	ProjectileMovement->SetActive(true);
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}