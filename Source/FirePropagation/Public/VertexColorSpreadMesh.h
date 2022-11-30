// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VertexColorSpreadMesh.generated.h"

UENUM()
namespace AVertexColorSpreadChannel
{
	enum Channel
	{
		Red,
		Green,
		Blue,
		Alpha,
	};
}

UCLASS()
class FIREPROPAGATION_API AVertexColorSpreadMesh : public AActor
{
	GENERATED_BODY()
	AVertexColorSpreadMesh();

	FMeshDescription* MeshDescription;
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
		/**
	Spread the vertex colors.
	*/
	void Spread();
	/**
	Create LOD info entires, and ensure the override vertex color buffer exists.
	*/
	void InitialiseLODInfoAndBuffers();
	bool FirstInit;

	TArray<int32> GreenLevelIndices;
	TArray<int32> YellowLevelIndices;
	TArray<int32> OrangeLevelIndices;
	TArray<int32> AlreadyCheckedIndices; // red level indices
	void PaintVertexInstances(FVertexID id, FStaticMeshComponentLODInfo& LODInfo, FString ColorString);

	FString GetNextColor(FColor nowColor);
	
	/**
	Spread intense vertex colors out.
	*/
	bool SpreadIntenseColors(FStaticMeshComponentLODInfo* InstanceMeshLODInfo, FStaticMeshLODResources& LODModel);

	/**
	The adjacency cache store.
	*/
	TMultiMap<int32, int32> AdjacencyCache;

public:
	/**
	The mesh with spreadable vertex colors.
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ColorSpread)
	UStaticMeshComponent* ColorSpreadComponent;

	/**
	The particle to spawn when vertices reach a certain intensity
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ParticleSystem)
	class UParticleSystem *IntenseParticleSystem;

	/**
	A list of spawned particles associated with verts
	*/
	TMap<int32, class UParticleSystemComponent*> Particles;

	/**
	Whether the spreading is enabled or disabled.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ColorSpread)
	bool bActive;

	/**
	Interval between spreading in seconds. Directly affects the interval at which the code is run.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ColorSpread)
	float Interval;

	/**
	Intensity needed to trigger the color to bleed to adjacent vertices (0..1).
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ColorSpread)
	float TriggerIntensity;

	/**
	Distance to spread the color when the threshold is met. This will depend on how closely packed the vertices are.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ColorSpread)
	float Distance;

	/**
	Which channel to spread
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ColorSpread)
	TEnumAsByte<AVertexColorSpreadChannel::Channel> Channel;

	/**
	When point damage is taken.
	*/
	// UFUNCTION()
	// void TakePointDamageDelegate(float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser);

	TArray<int32> ProcessingVertices;
	// TArray<int32> FindNearVertices(FVector Position, FStaticMeshLODResources& LODModel, FStaticMeshComponentLODInfo* InstanceMeshLODInfo);
	/**
	Internal setting, whether the timer is current active.
	*/
	bool bTimerActive;
};
