// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"

#include "ESet.h"
#include <vector>
#include <cstring>
#include <tuple>

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

	FColor color = FColor(255, 0, 0, 255);

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
		color = other.color;
	}
} Node;

class Room {
public:
	Room(ESet<Node> const & nodes, FColor const color = FColor());

	bool neighbours(Room const & other) const;

	/**
	 * Get node contained within the room that neighbours the passed node
	 * @param n node external to this room but adjacent to the perimeter of the room
	 * @returns neighbour node or null if no neighbour found
	 */
	Node* get_neighbour(Node const & n);

	std::pair<Node*, Node*> get_neighbouring_nodes(Room & other);

	bool operator==(Room const& other) const;
	bool operator!=(Room const& other) const { return !(*this == other); }

	ESet<Node> m_nodes;
	ESet<Node> m_perimeter_nodes;

	FColor color;
};

typedef struct Door {
	Room* room_a = nullptr;
	Room* room_b = nullptr;

	Node* node_a = nullptr;
	Node* node_b = nullptr;

	bool operator==(Door const& other) const
	{
		return (other.room_a == room_a && other.room_b == room_b) ||
			(other.room_b == room_a && other.room_a == room_b);
	}
} Door;

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
	ESet<Node> place_doors(ESet<Room>& rooms);

	std::tuple<std::vector<struct FColor>, size_t, size_t> to_image(ESet<Node>& node_set);
	void output_to_file(ESet<Node>& node_set, TCHAR const * name);
};

}
