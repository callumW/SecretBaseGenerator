// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "ESet.h"

namespace LevelGeneration {

Room::Room(ESet<Node> const & nodes)
{
    m_nodes = nodes;
    for (auto & n : m_nodes) {
        auto neighbour_nodes = LevelGenerator::get_adjacents(n);

        if (!m_nodes.contains(neighbour_nodes[0])) {
            n.walls[0] = WALL_TYPE::WALL;
            m_perimeter_nodes.insert(n);
        }
        if (!m_nodes.contains(neighbour_nodes[1])) {
            n.walls[1] = WALL_TYPE::WALL;
            m_perimeter_nodes.insert(n);
        }
        if (!m_nodes.contains(neighbour_nodes[2])) {
            n.walls[2] = WALL_TYPE::WALL;
            m_perimeter_nodes.insert(n);
        }
        if (!m_nodes.contains(neighbour_nodes[3])) {
            n.walls[3] = WALL_TYPE::WALL;
            m_perimeter_nodes.insert(n);
        }
    }

}

bool Room::neighbours(Room const & other) const
{
    for (auto const & n : m_perimeter_nodes) {
        auto adjacents = LevelGenerator::get_adjacents(n);
        for (auto const & adj : adjacents) {
            if (other.m_perimeter_nodes.contains(adj)) {
                return true;
            }
        }
    }
    return false;
}

bool Room::operator==(Room const & other) const
{
    return m_nodes == other.m_nodes;
}

LevelGenerator::LevelGenerator()
{
}

LevelGenerator::~LevelGenerator()
{
}

std::vector<Node> LevelGenerator::GenerateLevel(int32 num_nodes, int32 num_rooms, int32 seed)
{
    ESet<Node> nodes((unsigned) seed);

    spawn_node_set(nodes, num_nodes, seed);

    auto rooms = generate_rooms(nodes, num_rooms, seed);

    nodes.clear();

    for ( auto & room : rooms ) {
        for ( auto & n : room.m_nodes ) {
            nodes.insert(n);
        }
    }

    return nodes.to_vector();
}

void LevelGenerator::spawn_node_set(ESet<Node>& node_set, int32 num_nodes, int32 seed)
{
    if (num_nodes <= 0) {
        throw std::invalid_argument("LevelGenerator::spawn_node_set: num_nodes <= 0");
    }
    ESet<Node> adjacent_room_set{(unsigned)seed};
    node_set.insert(Node(0, 0));	// add origin so player doesn't fall!

    adjacent_room_set.insert(Node(1, 0));
    adjacent_room_set.insert(Node(0, 1));
    adjacent_room_set.insert(Node(-1, 0));
    adjacent_room_set.insert(Node(0, -1));

    uint8 count = 1;
    while (count <= num_nodes) {
        // pick random room
        auto current_room = adjacent_room_set.get_random();
        adjacent_room_set.erase(current_room);

        node_set.insert(current_room);

        // calculate adjacent rooms
        auto adjacents = get_adjacents(current_room);

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
    return {Node(n.x+1, n.y), Node(n.x, n.y+1), Node(n.x-1, n.y), Node(n.x, n.y-1)};
}

ESet<Room> LevelGenerator::generate_rooms(ESet<Node>& node_set, int32 num_rooms, int32 seed)
{

    const unsigned target_num_rooms = 5;

    ESet<std::pair<ESet<Node>, int>> seed_rooms(target_num_rooms,
                                                std::make_pair<ESet<Node>, int>({(unsigned)seed},0));

    /** Select nodes that will be seeds of rooms **/
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

        auto & seed_node = seed_rooms.get_random();
        int current_radius = seed_node.second + 1;

        seed_node.second = current_radius;
        auto & seed_nodes = seed_node.first;


        /** Get perimeter nodes **/
        int min_x = seed_nodes[0].x - current_radius;
        int max_x = seed_nodes[0].x + current_radius;

        int min_y = seed_nodes[0].y - current_radius;
        int max_y = seed_nodes[0].y + current_radius;

        ESet<Node> new_nodes((unsigned)seed);

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

        /** Add to room if not already in another room & are connected to this room (not diagonally) **/
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

    ESet<Room> rooms;
    for (auto const& seed_obj : seed_rooms) {
        rooms.insert(Room(seed_obj.first));
    }

    return rooms;
}

void LevelGenerator::place_doors(ESet<Room>& rooms)
{

    UE_LOG(LogTemp, Warning, TEXT("Calculating doors"));

    ESet<Door> all_doors;

    // TODO need to check evaluated rooms?

    // to for each room, look at neighbours and pick a random room to add.

    // for (int i = 0; i < seed_rooms.size(); i++) {
    //     for (int j = 0; j < seed_rooms.size(); j++) {
    //         ESet<Node> a_boundary_nodes((unsigned)seed);
    //         ESet<Node> b_boundary_nodes((unsigned)seed);
    //
    //         if (i == j) {
    //             continue;
    //         }
    //         else {
    //             for (auto & node : seed_rooms[i].first) {
    //                 auto adjacents = get_adjacents(node);
    //                 for (auto & neighbor : adjacents) {
    //                     auto location = seed_rooms[j].first.where_is(neighbor);
    //                     if (location.second) {
    //                         a_boundary_nodes.insert(node);
    //                         b_boundary_nodes.insert(*(location.first));
    //                     }
    //                 }
    //             }
    //         }
    //
    //         std::vector<std::pair<ESet<Node>&, int>> boundary_sets;
    //         if (a_boundary_nodes.size() > b_boundary_nodes.size()) {
    //             boundary_sets.push_back(std::pair<ESet<Node>&, int>(b_boundary_nodes, j));
    //             boundary_sets.push_back(std::pair<ESet<Node>&, int>(a_boundary_nodes, i));
    //         }
    //         else {
    //             boundary_sets.push_back(std::pair<ESet<Node>&, int>(a_boundary_nodes, i));
    //             boundary_sets.push_back(std::pair<ESet<Node>&, int>(b_boundary_nodes, j));
    //         }
    //
    //
    //         if (!boundary_sets[0].first.empty()) {
    //             size_t node_index = 0;
    //             auto node_a = boundary_sets[0].first.get_random(node_index);
    //             auto node_b = boundary_sets[1].first[node_index];
    //
    //             if (node_a.x < node_b.x) {  // door to north
    //                 node_a.walls[0] = WALL_TYPE::DOOR;
    //                 node_b.walls[2] = WALL_TYPE::DOOR;
    //             }
    //             else if (node_a.x > node_b.x) {
    //                 node_a.walls[2] = WALL_TYPE::DOOR;
    //                 node_b.walls[0] = WALL_TYPE::DOOR;
    //             }
    //             else if (node_a.y < node_b.y) {
    //                 node_a.walls[1] = WALL_TYPE::DOOR;
    //                 node_b.walls[3] = WALL_TYPE::DOOR;
    //             }
    //             else if (node_a.y > node_b.y) {
    //                 node_a.walls[3] = WALL_TYPE::DOOR;
    //                 node_b.walls[1] = WALL_TYPE::DOOR;
    //             }
    //
    //             Door door = {
    //                 node_a,
    //                 node_b,
    //                 boundary_sets[0].second,
    //                 boundary_sets[1].second
    //             };
    //
    //             all_doors.insert(door);
    //         }
    //     }
    // }
    //
    // for (auto & door : all_doors) {
    //     node_set.replace(door.a);
    //     node_set.replace(door.b);
    // }
}

}
