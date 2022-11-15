// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMeshBaseActor.h"
#include "DynamicMeshToMeshDescription.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshAttributes.h"
#include "StaticMeshDescription.h"

// Sets default values
ADynamicMeshBaseActor::ADynamicMeshBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"), false);
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ADynamicMeshBaseActor::BeginPlay()
{
	Super::BeginPlay();
	SetDescription();
	UpdateMesh();
}


void ADynamicMeshBaseActor::SetDescription()
{
	// Build a simple pyramid after play has begun
	// Mesh description will hold all the geometry, uv, normals going into the static mesh
	FStaticMeshAttributes Attributes(meshDesc);
	Attributes.Register();
	meshDescBuilder.SetMeshDescription( &meshDesc );
	meshDescBuilder.EnablePolyGroups();
	meshDescBuilder.SetNumUVLayers(1);
}

void ADynamicMeshBaseActor::UpdateMesh()
{
	meshDesc.Empty();
	MakeFloor();
	MakeFrontWall();
	MakeMesh();
}

void ADynamicMeshBaseActor::MakeFloor()
{
	int vertexNum = 2 * (width + 1);
	// Create the 5 vertices needed for the shape
	TArray< FVertexID > vertexIDs; vertexIDs.SetNum(vertexNum);
	// vertex 좌표
	for(int i = 0; i < vertexNum/2; i++)
	{
		vertexIDs[i] = meshDescBuilder.AppendVertex(FVector(100 * i, 0, 0));
		vertexIDs[i + width + 1] = meshDescBuilder.AppendVertex(FVector(100 * i, 100, 0));
	}
	
	// Array to store all the vertex instances (3 per face)
	TArray< FVertexInstanceID > vertexInsts;
	for(int i = 0; i < width; i++)
	{
		FVertexInstanceID instance = meshDescBuilder.AppendInstance(vertexIDs[0 + i]);
		// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0)); //floor fixed
		meshDescBuilder.SetInstanceUV(instance, FVector2D(0, 0), 0);
		meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f)); //fixed
		vertexInsts.Add(instance);
		
		instance = meshDescBuilder.AppendInstance(vertexIDs[1 + i]);
		// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
		meshDescBuilder.SetInstanceUV(instance, FVector2D(0, 1), 0);
		meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertexInsts.Add(instance);
	
		instance = meshDescBuilder.AppendInstance(vertexIDs[width + i + 1]);
		// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
		meshDescBuilder.SetInstanceUV(instance, FVector2D(1, 0), 0);
		meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertexInsts.Add(instance);
	
		instance = meshDescBuilder.AppendInstance(vertexIDs[width + i + 2]);
		// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
		meshDescBuilder.SetInstanceUV(instance, FVector2D(1, 1), 0);
		meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertexInsts.Add(instance);
	}


	// Allocate a polygon group
	FPolygonGroupID polygonGroup = meshDescBuilder.AppendPolygonGroup();
	for(int i = 0; i < width; i++)
	{
		meshDescBuilder.AppendTriangle(vertexInsts[0 + 4 * i], vertexInsts[1 + 4 * i], vertexInsts[2 + 4 * i], polygonGroup);
		meshDescBuilder.AppendTriangle(vertexInsts[1 + 4 * i], vertexInsts[3 + 4 * i], vertexInsts[2 + 4 * i], polygonGroup);
	}

}

void ADynamicMeshBaseActor::MakeFrontWall()
{
	int vertexNum = (height + 1) * (width + 1);
	// Create the 5 vertices needed for the shape
	TArray< FVertexID > vertexIDs; vertexIDs.SetNum(vertexNum);
	// vertex 좌표
	for(int i = 0; i < width + 1; i++)
	{
		for(int j = 0; j < height + 1; j++)
		{
			vertexIDs[i + j * (width + 1)] = meshDescBuilder.AppendVertex(FVector(100 * i, 100, 100 * (height - j)));
		}
	}
	
	// Array to store all the vertex instances (3 per face)
	TArray< FVertexInstanceID > vertexInsts;


	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			FVertexInstanceID instance = meshDescBuilder.AppendInstance(vertexIDs[i + j * (width + 1)]);
			// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0)); //floor fixed
			meshDescBuilder.SetInstanceUV(instance, FVector2D(0, 0), 0);
			meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f)); //fixed
			vertexInsts.Add(instance);
		
			instance = meshDescBuilder.AppendInstance(vertexIDs[i + 1 + j * (width + 1)]);
			// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
			meshDescBuilder.SetInstanceUV(instance, FVector2D(0, 1), 0);
			meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
			vertexInsts.Add(instance);
	
			instance = meshDescBuilder.AppendInstance(vertexIDs[i + (width + 1) * (j + 1)]);
			// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
			meshDescBuilder.SetInstanceUV(instance, FVector2D(1, 0), 0);
			meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
			vertexInsts.Add(instance);
	
			instance = meshDescBuilder.AppendInstance(vertexIDs[i + 1 + (width + 1) * (j + 1)]);
			// meshDescBuilder.SetInstanceNormal(instance, FVector(1, 1, 0));
			meshDescBuilder.SetInstanceUV(instance, FVector2D(1, 1), 0);
			meshDescBuilder.SetInstanceColor(instance, FVector4(1.0f, 1.0f, 1.0f, 1.0f));
			vertexInsts.Add(instance);
		}
	}


	// Allocate a polygon group
	FPolygonGroupID polygonGroup = meshDescBuilder.AppendPolygonGroup();
	int startIndex = 0;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			meshDescBuilder.AppendTriangle(vertexInsts[0 + startIndex], vertexInsts[2 + startIndex], vertexInsts[1 + startIndex], polygonGroup);
			meshDescBuilder.AppendTriangle(vertexInsts[1 + startIndex], vertexInsts[2 + startIndex], vertexInsts[3 + startIndex], polygonGroup);
			startIndex += 4;
		}
	}
}

void ADynamicMeshBaseActor::MakeMesh()
{
	// At least one material must be added
	UStaticMesh* staticMesh = NewObject<UStaticMesh>(this);
	// staticMesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = true;
	// mdParams.bFastBuild = true;

	// Build static mesh
	TArray<const FMeshDescription*> meshDescPtrs;
	meshDescPtrs.Emplace(&meshDesc);
	staticMesh->BuildFromMeshDescriptions(meshDescPtrs, mdParams);

	// Assign new static mesh to the static mesh component
	MeshComponent->SetStaticMesh(staticMesh);
}

// Called every frame
void ADynamicMeshBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMesh();

}

