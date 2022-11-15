// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralBuilding.h"

// Sets default values
AProceduralBuilding::AProceduralBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FirstWallComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FirstWallMesh"), false);
	FirstCornerComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FirstCornerMesh"), false);
	MiddleWallComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MiddleWallMesh"), false);
	MiddleCornerComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MiddleCornerMesh"), false);
	RoofDefaultComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoofDefaultMesh"), false);
	RoofVerticalEdgeComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoofVertEdgeMesh"), false);
	RoofHorizontalEdgeComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RoofHoriEdgeMesh"), false);
}

void AProceduralBuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateBuilding();
}

void AProceduralBuilding::PostActorCreated()
{
	Super::PostActorCreated();
}

// Called when the game starts or when spawned
void AProceduralBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProceduralBuilding::CreateBuilding()
{
	
	CreateCorners();
	CreateWalls();
	CreateRoof();
	GetMeshVertices();
}

void AProceduralBuilding::CreateCorners()
{
	// Create First Floor Corner Component
	// FirstCornerComponent->SetFlags(RF_Transactional);
	FirstCornerTransform.Empty();
	FirstCornerComponent->ClearInstances();
	FirstCornerTransform.Add(FTransform::Identity);
	FirstCornerTransform.Add(FTransform(FRotator(0, -90, 0),FVector(Width * WallLength + CornerLength * 2, 0, 0)));
	FirstCornerTransform.Add(FTransform(FRotator(0, -180, 0),FVector(Width * WallLength + CornerLength * 2, -Depth * WallLength - CornerLength * 2, 0)));
	FirstCornerTransform.Add(FTransform(FRotator(0, 90, 0),FVector(0, -Depth * WallLength - CornerLength * 2, 0)));
	FirstCornerComponent->AddInstances(FirstCornerTransform, false);

	// Create Middle Floor Corner Component
	// MiddleCornerComponent->SetFlags(RF_Transactional);
	MiddleCornerTransform.Empty();
	MiddleCornerComponent->ClearInstances();
	for(int h = 1; h < Height; h++)
	{
		MiddleCornerTransform.Add(FTransform(FVector(0, 0, FirstWallHeight + (h-1) * MiddleWallHeight)));
		MiddleCornerTransform.Add(FTransform(FRotator(0, -90, 0),FVector(Width * WallLength + CornerLength * 2, 0, FirstWallHeight + (h-1) * MiddleWallHeight)));
		MiddleCornerTransform.Add(FTransform(FRotator(0, -180, 0),FVector(Width * WallLength + CornerLength * 2, -Depth * WallLength - CornerLength * 2, FirstWallHeight + (h-1) * MiddleWallHeight)));
		MiddleCornerTransform.Add(FTransform(FRotator(0, 90, 0),FVector(0, -Depth * WallLength - CornerLength * 2, FirstWallHeight + (h-1) * MiddleWallHeight)));
	}
	MiddleCornerComponent->AddInstances(MiddleCornerTransform, false);
	
}

void AProceduralBuilding::CreateWalls()
{
	// Create First Floor Wall Component
	// FirstWallComponent->SetFlags(RF_Transactional);
	FirstWallTransform.Empty();
	FirstWallComponent->ClearInstances();
	for(int i = 0; i < Width; i++)
	{
		FirstWallTransform.Add(FTransform(FVector(300 * i + CornerLength, 0, 0)));
	}
	for(int i = 0; i < Depth; i++)
	{
		FirstWallTransform.Add(FTransform(FRotator(0, -90, 0), FVector(300 * Width + CornerLength * 2,  -300 * i - CornerLength, 0)));
	}
	for(int i = Width; i > 0; i--)
	{
		FirstWallTransform.Add(FTransform(FRotator(0, -180, 0), FVector( 300 * i + CornerLength, -300 * Depth - CornerLength * 2, 0)));
	}
	for(int i = Depth; i > 0; i--)
	{
		FirstWallTransform.Add(FTransform(FRotator(0, 90, 0), FVector(0,  -300 * i - CornerLength, 0)));
	}
	FirstWallComponent->AddInstances(FirstWallTransform, false);

	
	// Create Middle Floor Wall Component
	// MiddleWallComponent->SetFlags(RF_Transactional);
	MiddleWallTransform.Empty();
	MiddleWallComponent->ClearInstances();
	for(int h = 1; h < Height; h++)
	{
		for(int i = 0; i < Width; i++)
		{
			MiddleWallTransform.Add(FTransform(FVector(300 * i + CornerLength, 0, FirstWallHeight + (h-1) * MiddleWallHeight)));
		}
		for(int i = 0; i < Depth; i++)
		{
			MiddleWallTransform.Add(FTransform(FRotator(0, -90, 0), FVector(300 * Width + CornerLength * 2,  -300 * i - CornerLength, FirstWallHeight + (h-1) * MiddleWallHeight)));
		}
		for(int i = Width; i > 0; i--)
		{
			MiddleWallTransform.Add(FTransform(FRotator(0, -180, 0), FVector( 300 * i + CornerLength, -300 * Depth - CornerLength * 2, FirstWallHeight + (h-1) * MiddleWallHeight)));
		}
		for(int i = Depth; i > 0; i--)
		{
			MiddleWallTransform.Add(FTransform(FRotator(0, 90, 0), FVector(0,  -300 * i - CornerLength, FirstWallHeight + (h-1) * MiddleWallHeight)));
		}
	}
	MiddleWallComponent->AddInstances(MiddleWallTransform, false);
}

void AProceduralBuilding::CreateRoof()
{
	RoofVertEdgeTransform.Empty();
	RoofVerticalEdgeComponent->ClearInstances();
	for(int i = 0; i < Depth; i++)
	{
		RoofVertEdgeTransform.Add(FTransform(FVector(0, -1 * (CornerLength + RoofLength * i), (Height - 1) * MiddleWallHeight + FirstWallHeight)));
	}
	for(int i = 0; i < Depth; i++)
	{
		RoofVertEdgeTransform.Add(FTransform(FVector(Width * RoofLength + CornerLength, -1 * (CornerLength + RoofLength * i), (Height - 1) * MiddleWallHeight + FirstWallHeight)));
	}
	RoofVerticalEdgeComponent->AddInstances(RoofVertEdgeTransform, false);
	
	RoofHoriEdgeTransform.Empty();
	RoofHorizontalEdgeComponent->ClearInstances();
	for(int j = 0; j < Width; j++)
	{
		RoofHoriEdgeTransform.Add(FTransform(FVector(1 * (CornerLength + RoofLength * j), 0, (Height - 1) * MiddleWallHeight + FirstWallHeight)));
	}
	for(int j = 0; j < Width; j++)
	{
		RoofHoriEdgeTransform.Add(FTransform(FVector(1 * (CornerLength + RoofLength * j),  - Depth * RoofLength - CornerLength, (Height - 1) * MiddleWallHeight + FirstWallHeight)));
	}
	RoofHorizontalEdgeComponent->AddInstances(RoofHoriEdgeTransform, false);
	
	RoofDefaultTransform.Empty();
	RoofDefaultComponent->ClearInstances();
	for(int i = 0; i < Depth; i++)
	{
		for(int j = 0; j < Width; j++)
		{
			RoofDefaultTransform.Add(FTransform(FVector(j * RoofLength + CornerLength, -i * RoofLength - CornerLength, (Height - 1) * MiddleWallHeight + FirstWallHeight)));
		}
	}
	RoofDefaultComponent->AddInstances(RoofDefaultTransform, false);
}

void AProceduralBuilding::GetMeshVertices()
{
	VertexArray.Empty();
	if (!IsValidLowLevel()) return;
	if (!FirstWallComponent) return;
	if (!FirstWallComponent->GetStaticMesh()) return;
	if (!FirstWallComponent->GetStaticMesh()->RenderData) return;

	if(FirstWallComponent->GetStaticMesh()->RenderData->LODResources.Num() > 0)
	{
		FPositionVertexBuffer* VertexBuffer = &FirstWallComponent->GetStaticMesh()->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for (int32 Index = 0; Index < VertexCount; Index++)
			{
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition((Index)));
				VertexArray.Add(WorldSpaceVertexLocation);
			}
		}
	}
}

// Called every frame
void AProceduralBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}