// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralBuilding.generated.h"

UCLASS()
class FIREPROPAGATION_API AProceduralBuilding : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AProceduralBuilding();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostActorCreated() override;
	UPROPERTY(EditAnywhere)
	int Width;
	UPROPERTY(EditAnywhere)
	int Depth;
	UPROPERTY(EditAnywhere)
	int Height;

	UPROPERTY(EditAnywhere)
	int CornerLength;

	UPROPERTY(EditAnywhere)
	int WallLength;

	UPROPERTY(EditAnywhere)
	int RoofLength;
	
	UPROPERTY(EditAnywhere)
	int FirstWallHeight;

	UPROPERTY(EditAnywhere)
	int MiddleWallHeight;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* FirstWallComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* FirstCornerComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MiddleWallComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* MiddleCornerComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* RoofDefaultComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* RoofVerticalEdgeComponent;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* RoofHorizontalEdgeComponent;
	
	TArray<FTransform> FirstWallTransform;
	TArray<FTransform> FirstCornerTransform;
	
	TArray<FTransform> MiddleWallTransform;
	TArray<FTransform> MiddleCornerTransform;

	TArray<FTransform> RoofDefaultTransform;
	TArray<FTransform> RoofVertEdgeTransform;
	TArray<FTransform> RoofHoriEdgeTransform;

	TArray<FVector> VertexArray;
	

	/**
	 * @brief Building Start
	 */
	void CreateBuilding();

	/**
	 * @brief Corner mesh로 Building의 Corner 제작
	 */
	void CreateCorners();
	/**
	 * @brief Wall mesh로 Building의 Wall 제작
	 */
	void CreateWalls();
	/**
	 * @brief Roof mesh로 Building의 Roof 제작
	 */
	void CreateRoof();

	/**
	 * @brief instancing된 mesh의 vertices 추출
	 */
	void GetMeshVertices();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
