#include "Geometry/Position.h"
#include "Identifiers/IDs.h"
#include "Road/Lane.h"
#include "Road/MapLoader.h"
#include "Road/RoadGraph.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool MapLoader::LoadMapFromJson(RoadGraph& graph, const std::filesystem::path& file_path) {
    std::ifstream input(file_path);
    if (!input.is_open()) {
        std::cerr << "error: could not open map file: " << file_path << "\n";
        return false;
    }

    json root;
    try {
        input >> root;
    } catch (const std::exception& e) {
        std::cerr << "error: json parse failed: " << e.what() << "\n";
        return false;
    }

    for (const auto& node_json : root["nodes"]) {
        auto id = NodeID(node_json.at("id").get<NodeID::value_type>());
        double x = node_json.at("x").get<double>();
        double y = node_json.at("y").get<double>();

        graph.AddNode(id, Position{x, y});
    }

    for (const auto& road_json : root["roads"]) {
        auto from_id = NodeID(road_json.at("from").get<NodeID::value_type>());
        auto to_id = NodeID(road_json.at("to").get<NodeID::value_type>());
        double distance = road_json.at("distance").get<double>();

        const auto& curve_json = road_json["curve_points"];
        std::vector<Position> curve_points;
        curve_points.reserve(curve_json.size());
        for (const auto& p : curve_json) {
            curve_points.emplace_back(p[0].get<double>(), p[1].get<double>());
        }

        const auto& lanes_json = road_json["lanes"];
        std::vector<LaneConfig> lane_configs;
        lane_configs.reserve(lanes_json.size());
        for (const auto& lane_json : lanes_json) {
            int dir_val = lane_json.at("dir").get<int>();
            lane_configs.emplace_back(
                LaneConfig{.dir = (dir_val == 1) ? LaneDirection::Forward : LaneDirection::Backward,
                           .offset = lane_json.at("offset_m").get<double>(),
                           .width = lane_json.at("width_m").get<double>()});
        }

        graph.AddRoad(from_id, to_id, distance, std::move(curve_points), lane_configs);
    }

    return true;
}
