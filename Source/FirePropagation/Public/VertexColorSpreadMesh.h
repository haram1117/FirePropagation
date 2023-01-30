// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FireComponent.h"
#include "MeshDescription.h"
#include "VertexStruct.h"
#include "GameFramework/Actor.h"
#include "FireSimulation.h"
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
	bool SaveText(FString SaveDirectory, FString FileName, FStaticMeshLODResources& LODModel, FStaticMeshComponentLODInfo& LODInfo, int vertexNum);

	TQueue<int32> GreenLevelIndices;
	TQueue<int32> YellowLevelIndices;
	TQueue<int32> OrangeLevelIndices;
	TQueue<int32> AlreadyCheckedIndices; // red level indices

	TArray<int32> processedVertexIndices;

	TVertexAttributesConstRef<FVector> VertexPositions;

	// TVertexAttributesConstRef<FVector> Normals;

	TVertexInstanceAttributesConstRef<FVector> Normals;
	
	// TMap<int32, bool> GreenLevelIndices;
	// TMap<int32, bool> YellowLevelIndices;
	// TMap<int32, bool> OrangeLevelIndices;
	// TMap<int32, bool> AlreadyCheckedIndices;
	void PaintVertexInstances(VertexStruct* vertexStruct, FStaticMeshComponentLODInfo& LODInfo, FString ColorString, FVector& vertexLocation);

	FColor GetNextColor(FColor nowColor);
	
	/**
	Spread intense vertex colors out.
	*/
	bool SpreadIntenseColors(FStaticMeshComponentLODInfo& InstanceMeshLODInfo, FStaticMeshLODResources& LODModel);

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


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UFireSimulation* FireSimulationManager;
	
	// UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	// UFireComponent* FireComponent;
	
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

	TSet<FVertexID> ProcessingVertices;
	TSet<FVertexID> NextProcessingVertices;
	TSet<FVertexID> StartedVertices;

	TMap<int, VertexStruct> startedVerticesMap;
	
	// TArray<int32> FindNearVertices(FVector Position, FStaticMeshLODResources& LODModel, FStaticMeshComponentLODInfo* InstanceMeshLODInfo);
	/**
	Internal setting, whether the timer is current active.
	*/
	bool bTimerActive;
};
