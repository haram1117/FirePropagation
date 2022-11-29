// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAttributes.h"
#include "GameFramework/Actor.h"
#include "RawMesh.h"
#include "HeatTransferMesh.generated.h"

struct VertexInfo
{
	TArray<int> ids;
	int UniqueId;
	FVector vertexPosition;
	VertexInfo(int id, int _UniqueId, FVector _vertexPosition): UniqueId(_UniqueId)
	{
		ids.Add(id);
		vertexPosition.X = _vertexPosition.X;
		vertexPosition.Y = _vertexPosition.Y;
		vertexPosition.Z = _vertexPosition.Z;
	}

	bool CheckDuplicated(FVector position, int id)
	{
		if(position == vertexPosition)
		{
			SetID(id);
			return true;
		}
		return false;
	}
	void SetID(int id)
	{
		ids.Add(id);
	}
};
UCLASS()
class FIREPROPAGATION_API AHeatTransferMesh : public AActor
{
	
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeatTransferMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	UStaticMesh* StaticMesh;
	FMeshDescription* MeshDescription;
	TArray<FVector> MeshVertices;
	TArray<FColor> RandomColorArray;
	TArray<FColor> ColorArray;

	TArray<FVertexID> nowVertices;
	TArray<FVertexID> nextVertices;

	// FStaticMeshComponentLODInfo* LODInfo;
	FStaticMeshComponentLODInfo* LODInfo;

	TArray<VertexInfo> MeshVertices_info;
	
	void ColorVertexInstances(FVertexID id);
	TArray<FVertexID> FindAdjacentVertices(FVertexID id);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};

