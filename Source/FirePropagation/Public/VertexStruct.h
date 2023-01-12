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

public:
	void SetVertexColor(FColor color)
	{
		VertexColor = color;
	}

	FColor GetVertexColor() const
	{
		return VertexColor;
	}
	
};
