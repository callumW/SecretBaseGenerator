// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "ESet.h"

namespace LevelGeneration {

LevelGenerator::LevelGenerator()
{
}

LevelGenerator::~LevelGenerator()
{
}

std::vector<Node> LevelGenerator::GenerateLevel()
{
    std::vector<Node> rooms;

    spawn_node_set(rooms);

    place_rooms(rooms);

    return rooms;
}

void LevelGenerator::spawn_node_set(std::vector<Node>& node_set)
{
    const int num_rooms = 60;
    ESet<Node> room_set;
    ESet<Node> adjacent_room_set{};
    room_set.insert(Node(0, 0));	// add origin so player doesn't fall!

    adjacent_room_set.insert(Node(1, 0));
    adjacent_room_set.insert(Node(0, 1));

    uint8 count = 1;
    while (count <= num_rooms) {
        // pick random room
        auto current_room = adjacent_room_set.get_random();
        adjacent_room_set.erase(current_room);

        room_set.insert(current_room);

        // calculate adjacent rooms
        std::vector<Node> adjacents = {
            {current_room.x, current_room.y-1},
            {current_room.x-1, current_room.y},
            {current_room.x+1, current_room.y},
            {current_room.x, current_room.y+1},
        };

        // add to adjacent room list
        for (auto & r : adjacents) {
            if (!room_set.contains(r)) {
                adjacent_room_set.insert(r);
            }
        }

        room_set.insert(current_room);
        count++;
    }

    node_set.swap(room_set.vec());
}

void LevelGenerator::place_rooms(std::vector<Node>& node_set)
{

}

}
