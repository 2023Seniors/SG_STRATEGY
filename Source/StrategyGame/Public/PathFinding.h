// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chaos/Pair.h"
#include "Containers/Array.h"
#include "Containers/List.h"

struct NodePathFinding
{
	typedef TSharedPtr<NodePathFinding> node_t;

	NodePathFinding(FIntVector2 pos_, node_t parent_ = nullptr, float cost_ = 0.f, float g_ = 0.f) :
		pos(pos_), parent(parent_), cost(cost_), g(g_)
	{}

	FIntVector2 pos;
	node_t parent;
	float cost;
	float g;
};

typedef TSharedPtr<NodePathFinding> node_t;
typedef TPair<float, FIntVector2> node_path;

class AGridManager;

enum class PathFindingMode
{
	EUCLIDEAN = 0,
	OCTILE,
	CHEBYSHEV,
	MANHATTAN
};

class STRATEGYGAME_API PathFinding
{
public:
	static FIntVector2 PFFindPath(FIntVector2 start, FIntVector2 end, AGridManager* world, PathFindingMode mode = PathFindingMode::EUCLIDEAN, float weight_ = 1.01f);
	static TArray<FIntVector2> PFFindFullPath(FIntVector2 start, FIntVector2 end, AGridManager* world, PathFindingMode mode = PathFindingMode::EUCLIDEAN, float weight_ = 1.01f);
	
private:
	PathFinding(FIntVector2 cur_, FIntVector2 goal_, AGridManager* grid_, PathFindingMode mode = PathFindingMode::EUCLIDEAN, float weight_ = 1.01f);
	~PathFinding();

	void CreateSuccesor(FIntVector2 pos_, node_t parent_, int idxCorner);
	bool CheckIdx(int x, int z);
	node_t FindPop();

	float Euclidean(FIntVector2 end, FIntVector2 start);
	float Octile(FIntVector2 pos_);
	float Chebyshev(FIntVector2 pos_);
	float Manhattan(FIntVector2 pos_);

	AGridManager* grid;

	node_t goalNode;

	FIntVector2 goal;
	FIntVector2 gridSize;

	bool stop = false;

	float			  heuristicWeight;
	PathFindingMode	  heuristicCalc;

	TArray<node_t> openList;
	TArray<node_t> closeList;
};
