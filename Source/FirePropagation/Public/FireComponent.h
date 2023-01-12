// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireSimulation.h"
#include "Components/ActorComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "FireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIREPROPAGATION_API UFireComponent : public UParticleSystemComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireComponent();
	UParticleSystem* ParticleSystem;

	TArray<FVertexID> VertexIds;
	FVector centerLocation;
	UFireSimulation* FireSimulation;

	void AddVertexToFireComponent(FVertexID VertexID, FVector newLocation);
	bool CheckInBoundary(FVertexID VertexID, FVector newLocation);
	void SetFireSimulation(UFireSimulation* __fireSim);
	void SmokeStart();
	void FireStart();
	
protected:

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
