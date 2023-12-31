// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"
#include "..\GridManager.h"

#include <algorithm>

PathFinding::PathFinding(FIntVector2 cur_, FIntVector2 goal_, AGridManager* grid_, PathFindingMode mode, float weight_)
{
	stop = false;

	openList.Empty();
	closeList.Empty();

	goal = goal_;
	grid = grid_;
	heuristicCalc = mode;
	heuristicWeight = weight_;

	FVector mapSize = grid->GetMapSize();
	gridSize.X = mapSize.X;
	gridSize.Y = mapSize.Y;

	openList.Add(node_t(new NodePathFinding(cur_)));
}

PathFinding::~PathFinding() {}

bool PathFinding::CheckIdx(int x, int y)
{
	return x >= 0 && x < gridSize.X && y >= 0 && y < gridSize.Y;
}

void PathFinding::CreateSuccesor(FIntVector2 pos_, node_t parent_, int idxCorner)
{
	// CHECK NON POSSIBLE CELL
	{
		// OUT OF BOUNDS
		if (!CheckIdx(pos_.X, pos_.Y) || !grid->IsCellWalkableFromGridXY(pos_.X, pos_.Y)) {
			return;
		}

		// 4 CORNER CASES

		// SE
		if (idxCorner == 0 && (CheckIdx(pos_.X - 1, pos_.Y) && CheckIdx(pos_.X, pos_.Y + 1) && (!grid->IsCellWalkableFromGridXY(pos_.X - 1, pos_.Y) || !grid->IsCellWalkableFromGridXY(pos_.X, pos_.Y + 1)))) {
			return;
		}

		// NE
		if (idxCorner == 1 && (CheckIdx(pos_.X - 1, pos_.Y) && CheckIdx(pos_.X, pos_.Y - 1) && (!grid->IsCellWalkableFromGridXY(pos_.X - 1, pos_.Y) || !grid->IsCellWalkableFromGridXY(pos_.X, pos_.Y - 1)))) {
			return;
		}

		// NW
		if (idxCorner == 2 && (CheckIdx(pos_.X + 1, pos_.Y) && CheckIdx(pos_.X, pos_.Y - 1) && (!grid->IsCellWalkableFromGridXY(pos_.X + 1, pos_.Y) || !grid->IsCellWalkableFromGridXY(pos_.X, pos_.Y - 1)))) {
			return;
		}

		// SW
		if (idxCorner == 3 && (CheckIdx(pos_.X + 1, pos_.Y) && CheckIdx(pos_.X, pos_.Y + 1) && (!grid->IsCellWalkableFromGridXY(pos_.X + 1, pos_.Y) || !grid->IsCellWalkableFromGridXY(pos_.X, pos_.Y + 1)))) {
			return;
		}
	}

	node_t node(new NodePathFinding(pos_, parent_));

	// PATH FOUND
	if (pos_ == goal) {
		goalNode = node;
		stop = true;
		return;
	}

	// CALCULATE HEURISTIC
	// REVISIT THIS, CANT SUBSTRACT VECTORS??? WTF
	float h = -1.f;
	if (heuristicCalc == PathFindingMode::OCTILE) {
		h = Octile( { goal.X - pos_.X, goal.Y - pos_.Y } );
	}
	else if (heuristicCalc == PathFindingMode::CHEBYSHEV) {
		h = Chebyshev( { goal.X - pos_.X, goal.Y - pos_.Y });
	}
	else if (heuristicCalc == PathFindingMode::MANHATTAN) {
		h = Manhattan( { goal.X - pos_.X, goal.Y - pos_.Y });
	}
	else {
		h = Euclidean(goal, pos_);
	}

	// COST FROM PREV TO CURRENT NODE
	node->g = Euclidean(pos_, parent_->pos) + parent_->g;
	node->cost = node->g + h * heuristicWeight;

	for (int i = 0; i < openList.Num(); i++) {
		if (openList[i]->pos == pos_) {
			// FOUND A NODE WITH BETTER COST, IGNORE CURRENT NODE
			if (openList[i]->cost <= node->cost) {
				return;
			}

			// SUBSTITUTE CURRENT NODE AS COST IS BETTER
			else {
				openList[i] = node;
				return;
			}
		}
	}

	for (int i = 0; i < closeList.Num(); i++) {
		// FOUND A NODE WITH BETTER COST, IGNORE CURRENT NODE
		if (closeList[i]->pos == pos_ && closeList[i]->cost <= node->cost) {
			return;
		}

		// TOO COSTLY TO ERASE
		//else {
		//	closeList.erase(closeList.begin() + i);
		//}
	}

	openList.Add(node);
}

// FIND THE NODE WITH LEAST COST FROM OPEN LIST AND DELETE IT
node_t PathFinding::FindPop()
{
	float minCost = openList[0]->cost;
	node_t node = openList[0];
	int idx = 0;

	// FIND NODE
	for (int i = 1; i < openList.Num(); i++) {
		if (openList[i]->cost < minCost) {
			minCost = openList[i]->cost;
			node = openList[i];
			idx = i;
		}
	}

	// DELETE IT
	openList.RemoveAt(idx);

	return node;
}

FIntVector2 PathFinding::PFFindPath(FIntVector2 start, FIntVector2 end, AGridManager* grid, PathFindingMode mode, float weight_)
{
	PathFinding path(start, end, grid, mode, weight_);

	// UNTIL WE FIND A PATH OR LIST IS EMPTY
	while (!path.openList.IsEmpty()) {

		node_t node = path.FindPop();

		if (!grid->IsCellWalkableFromGridXY(node->pos.X, node->pos.Y)) {
			continue;
		}

		// 8 chid nodes
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y - 1 }, node, 0);
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y + 1 }, node, 1);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y + 1 }, node, 2);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y - 1 }, node, 3);
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y }, node, -1);
		path.CreateSuccesor({ node->pos.X, node->pos.Y + 1 }, node, -1);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y }, node, -1);
		path.CreateSuccesor({ node->pos.X, node->pos.Y - 1 }, node, -1);

		path.closeList.Add(node);

		// Path found
		if (path.stop) {

			// Get the next node to move
			node_t pathNode = path.goalNode;
			while (pathNode->parent != nullptr) {
				if (pathNode->parent->parent == nullptr) {
					break;
				}
				pathNode = pathNode->parent;
			}

			// Cost of thge whole journey, just return the first move
			return pathNode->pos;
		}
	}

	// OPEN LIST IS EMPTY, NO PATH FOUND
	return start;
}

TArray<FIntVector2> PathFinding::PFFindFullPath(FIntVector2 start, FIntVector2 end, AGridManager* grid, PathFindingMode mode, float weight_)
{
	PathFinding path(start, end, grid, mode, weight_);

	TArray<FIntVector2> pathVec;

	// UNTIL WE FIND A PATH OR LIST IS EMPTY
	while (!path.openList.IsEmpty()) {

		node_t node = path.FindPop();

		if (!grid->IsCellWalkableFromGridXY(node->pos.X, node->pos.Y)) {
			continue;
		}

		// 8 chid nodes
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y - 1 }, node, 0);
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y + 1 }, node, 1);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y + 1 }, node, 2);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y - 1 }, node, 3);
		path.CreateSuccesor({ node->pos.X + 1, node->pos.Y }, node, -1);
		path.CreateSuccesor({ node->pos.X, node->pos.Y + 1 }, node, -1);
		path.CreateSuccesor({ node->pos.X - 1, node->pos.Y }, node, -1);
		path.CreateSuccesor({ node->pos.X, node->pos.Y - 1 }, node, -1);

		path.closeList.Add(node);

		// Path found
		if (path.stop) {
			// Get the next node to move
			node_t pathNode = path.goalNode;
			while (pathNode != nullptr) {
				pathVec.Insert(pathNode->pos, 0);
				pathNode = pathNode->parent;
			}
			break;
		}
	}

	// OPEN LIST IS EMPTY, NO PATH FOUND
	return pathVec;
}


float PathFinding::Euclidean(FIntVector2 end, FIntVector2 start)
{
	float x = (end.X - start.X);
	float y = (end.Y - start.Y);
	return x * x + y * y;
}

float PathFinding::Octile(FIntVector2 pos_)
{
	static float sqrt2 = std::sqrt(2);
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return ((std::min)(x, y)) * sqrt2 + (std::max)(x, y) - (std::min)(x, y);
}

float PathFinding::Chebyshev(FIntVector2 pos_)
{
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return (std::max)(x, y);
}

float PathFinding::Manhattan(FIntVector2 pos_)
{
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return x + y;
}
