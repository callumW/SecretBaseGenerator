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
    ESet<Node> rooms((unsigned) 353424);

    spawn_node_set(rooms);

    place_rooms(rooms);

    return rooms.to_vector();
}

void LevelGenerator::spawn_node_set(ESet<Node>& node_set)
{
    const int num_rooms = 60;
    ESet<Node> adjacent_room_set{};
    node_set.insert(Node(0, 0));	// add origin so player doesn't fall!

    adjacent_room_set.insert(Node(1, 0));
    adjacent_room_set.insert(Node(0, 1));
    adjacent_room_set.insert(Node(-1, 0));
    adjacent_room_set.insert(Node(0, -1));

    uint8 count = 1;
    while (count <= num_rooms) {
        // pick random room
        auto current_room = adjacent_room_set.get_random();
        adjacent_room_set.erase(current_room);

        node_set.insert(current_room);

        // calculate adjacent rooms
        std::vector<Node> adjacents = {
            {current_room.x, current_room.y-1},
            {current_room.x-1, current_room.y},
            {current_room.x+1, current_room.y},
            {current_room.x, current_room.y+1},
        };

        // add to adjacent room list
        for (auto & r : adjacents) {
            if (!node_set.contains(r)) {
                adjacent_room_set.insert(r);
            }
        }

        count++;
    }
}

std::vector<Node> LevelGenerator::get_adjacents(Node n)
{
    return {Node(n.x-1, n.y), Node(n.x+1, n.y), Node(n.x, n.y-1), Node(n.x, n.y+1)};
}

void LevelGenerator::place_rooms(ESet<Node>& node_set)
{

    const unsigned target_num_rooms = 5;

    ESet<std::pair<ESet<Node>, int>> seed_rooms(target_num_rooms,
                                                std::make_pair<ESet<Node>, int>({},0));

    unsigned count = 0;
    while (count < target_num_rooms) {
        auto node = node_set.get_random();
        auto & seed_set = seed_rooms[count].first;

        if (seed_set.insert(node)) {
            node_set.erase(node);
            count++;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Seeds: "));
    for (auto & seed_obj : seed_rooms) {
        UE_LOG(LogTemp, Warning, TEXT("     (%d,%d)"), seed_obj.first[0].x, seed_obj.first[0].y);
    }

    /** Distribute nodes between seeds to form rooms **/
    UE_LOG(LogTemp, Warning, TEXT("Entering seed growth stage"));
    while (!node_set.empty()) {
        // Get a random seed
        UE_LOG(LogTemp, Warning, TEXT("node_set size: %d"), node_set.size());
        for (auto const& node : node_set) {
            bool is_linked = false;
            auto adj = get_adjacents(node);
            for (auto const& seed_obj : seed_rooms) {
                for (auto n : adj) {
                    if (seed_obj.first.contains(n)) {
                        is_linked = true;
                    }
                }
            }

            if (!is_linked) {
                UE_LOG(LogTemp, Warning, TEXT("Room (%d,%d) is not linked to any seed!"),
                    node.x,
                    node.y
                );
            }
        }

        auto & seed = seed_rooms.get_random();
        int current_radius = seed.second + 1;
        UE_LOG(LogTemp, Warning, TEXT("Radius: %d"), current_radius);
        seed.second = current_radius;
        auto & seed_nodes = seed.first;


        /** Get nodes **/
        int min_x = seed_nodes[0].x - current_radius;
        int max_x = seed_nodes[0].x + current_radius;

        int min_y = seed_nodes[0].y - current_radius;
        int max_y = seed_nodes[0].y + current_radius;

        ESet<Node> new_nodes;

        for (int x = min_x; x <= max_x; x++) {
            Node one(x, min_y);
            Node two(x, max_y);

            new_nodes.insert(one);
            new_nodes.insert(two);
        }

        for (int y = min_y; y <= max_y; y++) {
            Node one(min_x, y);
            Node two(max_x, y);

            new_nodes.insert(one);
            new_nodes.insert(two);
        }

        int last_node_count = 0;
        while (!new_nodes.empty() && new_nodes.size() != last_node_count) {
            last_node_count = new_nodes.size();

            for (auto it = new_nodes.begin(); it != new_nodes.end();) {
                bool increment = true;
                if (node_set.contains(*it)) {
                    auto adjacents = get_adjacents(*it);

                    for (auto & n : adjacents) {
                        if (seed_nodes.contains(n)) {
                            seed_nodes.insert(*it);
                            node_set.erase(*it);
                            it = new_nodes.erase(it);
                            increment = false;
                            break;
                        }
                    }
                }
                if (increment) it++;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Total room count: %u"), node_set.size());


    /** Temporarily re-add rooms **/
    for (auto & seed_obj : seed_rooms) {
        auto & seed_set = seed_obj.first;
        for (auto & node : seed_set) {
            node_set.insert(node);
        }
    }
}

}
