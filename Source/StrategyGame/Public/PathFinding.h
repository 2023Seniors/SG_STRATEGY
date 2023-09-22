// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <vector>
#include <list>
#include <memory>

struct NodePathFinding
{
	typedef std::shared_ptr<NodePathFinding> node_t;

	NodePathFinding(FIntVector2 pos_, node_t parent_ = nullptr, float cost_ = 0.f, float g_ = 0.f) :
		pos(pos_), parent(parent_), cost(cost_), g(g_)
	{}

	FIntVector2 pos;
	node_t parent;
	float g;
	float cost;
};

typedef std::shared_ptr<NodePathFinding> node_t;
typedef std::pair<float, FIntVector2> node_path;

class AGridManager;

class STRATEGYGAME_API PathFinding
{
public:
	PathFinding();
	~PathFinding();

	void Reset(FIntVector2 cur_, FIntVector2 goal_, AGridManager* grid_);
	void CreateSuccesor(FIntVector2 pos_, node_t parent_, int idxCorner);
	bool CheckIdx(int x, int z);

	node_t FindPop();

	AGridManager* grid;

	node_t goalNode;

	FIntVector2 goal;
	FIntVector2 gridSize;

	bool stop = false;

	int	  heuristicCalc;
	float heuristicWeight;

	std::vector<node_t> openList;
	std::vector<node_t> closeList;
};


float Euclidean(FIntVector2 end, FIntVector2 start);
float Octile(FIntVector2 pos_);
float Chebyshev(FIntVector2 pos_);
float Manhattan(FIntVector2 pos_);

node_path FindPath(FIntVector2 start, FIntVector2 end, AGridManager* world);
std::list<FIntVector2> FindFullPath(FIntVector2 start, FIntVector2 end, AGridManager* world);
