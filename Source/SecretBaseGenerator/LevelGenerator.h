// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ESet.h"
#include <vector>
#include <cstring>

namespace LevelGeneration {

enum class WALL_TYPE {
	NONE,
	WALL,
	DOOR
};

enum class NODE_TYPE {
	ROOM,
	STAIR_WELL
};


typedef struct Node {
	int x, y;
	// N (+x), E (+y), S(-x), W (-y)
	WALL_TYPE walls[4] = {WALL_TYPE::NONE};

	NODE_TYPE type = NODE_TYPE::ROOM;

	Node(int x_val, int y_val)
	{
		x = x_val;
		y = y_val;
	}

	bool operator==(Node const& other) const
	{
		return other.x == x && other.y == y;
	}

	Node(Node const& other)
	{
		x = other.x;
		y = other.y;
		std::memcpy((void*) walls, (void const*) other.walls, sizeof(WALL_TYPE) * 4);
		type = other.type;
	}
} Node;

typedef struct Door {
	Node a;
	Node b;

	int set_a;
	int set_b;

	bool operator==(Door const& other) const
	{
		//return (other.a == a && other.b == b) || (other.a == b && other.b == a);
		return (set_a == other.set_a && set_b == other.set_b) ||
			(set_b == other.set_a && set_a == other.set_b);
	}

} Door;

class Room {
public:
	Room(ESet<Node> const & nodes);

	bool neighbours(Room const & other) const;

	/**
	 * Get node contained within the room that neighbours the passed node
	 * @param n node external to this room but adjacent to the perimeter of the room
	 * @returns neighbour node
	 * @throws std::invalid_argument if n is not neighbouring any node in the room
	 */
	Node& get_neighbour(Node const & n);

	bool operator==(Room const& other) const;

	ESet<Node> m_nodes;
	ESet<Node> m_perimeter_nodes;
};

/**
 *
 */
class SECRETBASEGENERATOR_API LevelGenerator
{
public:

	LevelGenerator();
	~LevelGenerator();

	std::vector<Node> GenerateLevel(int32 num_nodes, int32 num_rooms, int32 seed);

	static std::vector<Node> get_adjacents(Node n);

private:
	void spawn_node_set(ESet<Node>& node_set, int32 num_nodes, int32 seed);
	ESet<Room> generate_rooms(ESet<Node>& node_set, int32 num_rooms, int32 seed);
	void place_doors(ESet<Room>& rooms);
};

}
