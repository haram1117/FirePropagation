// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FIREPROPAGATION_API VertexStruct
{
public:
	VertexStruct(int __id, FColor color)
	{
		VertexID = __id;
		VertexColor = color;
	};
	~VertexStruct();

private:
	int VertexID;
	FVector vertexLocation;
	FColor VertexColor;
	int fireID = 0;

public:

	void SetVertexID(int _id)
	{
		VertexID = _id;
	}

	int GetVertexID() const
	{
		return VertexID;
	}
	
	void SetVertexColor(FColor color)
	{
		VertexColor = color;
	}

	FColor GetVertexColor() const
	{
		return VertexColor;
	}

	void setFireID(int id)
	{
		fireID = id;
	}

	int getFireID() const
	{
		return fireID;
	}
	
};
