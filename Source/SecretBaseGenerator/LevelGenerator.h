// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <vector>

namespace LevelGeneration {

enum class NODE_TYPE {
	NONE,
	WALL,
	DOOR
};


typedef struct Node {
	int x, y;
	// N (-y), E (+x), S(+y), W (-x)
	NODE_TYPE walls[4] = {NODE_TYPE::NONE};

	Node(int x_val, int y_val)
	{
		x = x_val;
		y = y_val;
	}

	bool operator==(Node const& other) const
	{
		return other.x == x && other.y == y;
	}
} Node;

/**
 *
 */
class SECRETBASEGENERATOR_API LevelGenerator
{
public:

	LevelGenerator();
	~LevelGenerator();

	std::vector<Node> GenerateLevel();

private:
	void spawn_node_set(std::vector<Node>& node_set);
	void place_rooms(std::vector<Node>& node_set);
};

}
