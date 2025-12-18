#include "Road/MapLoader.h"
#include "Geometry/Position.h"
#include "Road/RoadGraph.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool MapLoader::LoadMapFromJson(RoadGraph& graph, std::string_view file_path) {
    std::ifstream input(file_path.data());
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
        int id = node_json.at("id").get<int>();
        double x = node_json.at("x").get<double>();
        double y = node_json.at("y").get<double>();

        graph.AddNode(id, Position{x, y});
    }

    for (const auto& road_json : root["roads"]) {
        int from_id = road_json.at("from").get<int>();
        int to_id = road_json.at("to").get<int>();
        double distance = road_json.at("distance").get<double>();

        std::vector<Position> curve_points;
        for (const auto& p : road_json["curve_points"]) {
            curve_points.push_back({p[0].get<double>(), p[1].get<double>()});
        }

        graph.AddRoad(from_id, to_id, distance, curve_points);
    }

    return true;
}
