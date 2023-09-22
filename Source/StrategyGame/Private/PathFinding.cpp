// Fill out your copyright notice in the Description page of Project Settings.


#include "Pathfinding.h"
#include "..\GridManager.h"

#include <algorithm>

PathFinding::PathFinding()
{
}

PathFinding::~PathFinding() {}

void PathFinding::Reset(FIntVector2 cur_, FIntVector2 goal_, AGridManager* grid_)
{
	stop = false;

	openList.clear();
	closeList.clear();

	goal = goal_;
	grid = grid_;
	heuristicCalc = 1;
	heuristicWeight = 1.01f;

	FVector mapSize = grid->GetMapSize();
	gridSize.X = mapSize.X;
	gridSize.Y = mapSize.Y;

	openList.push_back(node_t(new NodePathFinding(cur_)));
}

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
	if (heuristicCalc == 1) {
		h = Octile( { goal.X - pos_.X, goal.Y - pos_.Y } );
	}
	else if (heuristicCalc == 2) {
		h = Chebyshev( { goal.X - pos_.X, goal.Y - pos_.Y });
	}
	else if (heuristicCalc == 3) {
		h = Manhattan( { goal.X - pos_.X, goal.Y - pos_.Y });
	}
	else {
		h = Euclidean(goal, pos_);
	}

	// COST FROM PREV TO CURRENT NODE
	node->g = Euclidean(pos_, parent_->pos) + parent_->g;
	node->cost = node->g + h * heuristicWeight;

	for (int i = 0; i < openList.size(); i++) {
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

	for (int i = 0; i < closeList.size(); i++) {
		// FOUND A NODE WITH BETTER COST, IGNORE CURRENT NODE
		if (closeList[i]->pos == pos_ && closeList[i]->cost <= node->cost) {
			return;
		}

		// TOO COSTLY TO ERASE
		//else {
		//	closeList.erase(closeList.begin() + i);
		//}
	}

	openList.push_back(node);
}

// FIND THE NODE WITH LEAST COST FROM OPEN LIST AND DELETE IT
node_t PathFinding::FindPop()
{
	float minCost = openList[0]->cost;
	node_t node = openList[0];
	int idx = 0;

	// FIND NODE
	for (int i = 1; i < openList.size(); i++) {
		if (openList[i]->cost < minCost) {
			minCost = openList[i]->cost;
			node = openList[i];
			idx = i;
		}
	}

	// DELETE IT
	openList.erase(openList.begin() + idx);

	return node;
}

node_path FindPath(FIntVector2 start, FIntVector2 end, AGridManager* grid)
{
	static PathFinding path;
	path.Reset(start, end, grid);

	// UNTIL WE FIND A PATH OR LIST IS EMPTY
	while (!path.openList.empty()) {

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

		path.closeList.push_back(node);

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
			return { node->cost, pathNode->pos };
		}
	}

	// OPEN LIST IS EMPTY, NO PATH FOUND
	return { 0.f, start };
}

std::list<FIntVector2> FindFullPath(FIntVector2 start, FIntVector2 end, AGridManager* grid)
{
	static PathFinding path;
	path.Reset(start, end, grid);

	std::list<FIntVector2> pathVec;

	// UNTIL WE FIND A PATH OR LIST IS EMPTY
	while (!path.openList.empty()) {

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

		path.closeList.push_back(node);

		// Path found
		if (path.stop) {
			// Get the next node to move
			node_t pathNode = path.goalNode;
			while (pathNode != nullptr) {
				pathVec.push_front(pathNode->pos);
				pathNode = pathNode->parent;
			}
			break;
		}
	}

	// OPEN LIST IS EMPTY, NO PATH FOUND
	return pathVec;
}


float Euclidean(FIntVector2 end, FIntVector2 start)
{
	float x = (end.X - start.X);
	float y = (end.Y - start.Y);
	return x * x + y * y;
}

float Octile(FIntVector2 pos_)
{
	static float sqrt2 = std::sqrt(2);
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return ((std::min)(x, y)) * sqrt2 + (std::max)(x, y) - (std::min)(x, y);
}

float Chebyshev(FIntVector2 pos_)
{
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return (std::max)(x, y);
}

float Manhattan(FIntVector2 pos_)
{
	float x = std::abs(pos_.X);
	float y = std::abs(pos_.Y);
	return x + y;
}
