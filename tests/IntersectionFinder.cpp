#include "Map/IntersectionFinder.h"
#include "Road/RoadGraph.h"
#include "Road/RoadIntersection.h"

#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

namespace {

const std::vector<LaneConfig> DEFAULT_LANES = {{LaneDirection::Forward, 0.0, 3.0}};

void AddStraightRoad(RoadGraph& graph, NodeID from, NodeID to) {
    graph.AddRoad(from, to, 1.0, {}, DEFAULT_LANES);
}

bool ContainsRoad(const RoadIntersection& ri, RoadID id) {
    return std::find(ri.intersecting_roads.begin(), ri.intersecting_roads.end(), id) !=
           ri.intersecting_roads.end();
}

} // namespace

TEST_CASE("No intersections with an empty graph") {
    RoadGraph graph;
    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.empty());
}

TEST_CASE("No intersections with a single road") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 0.0});
    graph.AddNode(NodeID(1), {10.0, 0.0});
    AddStraightRoad(graph, NodeID(0), NodeID(1));

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.empty());
}

TEST_CASE("No intersections for parallel roads") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 0.0});
    graph.AddNode(NodeID(1), {10.0, 0.0});
    graph.AddNode(NodeID(2), {0.0, 5.0});
    graph.AddNode(NodeID(3), {10.0, 5.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(3));

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.empty());
}

TEST_CASE("Two crossing roads produce one intersection") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(3));

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0].intersecting_roads.size() == 2);
    REQUIRE(result[0].pos.x_coord == Catch::Approx(5.0));
    REQUIRE(result[0].pos.y_coord == Catch::Approx(5.0));
}

TEST_CASE("Intersection contains the correct road IDs") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1)); // RoadID(0)
    AddStraightRoad(graph, NodeID(2), NodeID(3)); // RoadID(1)

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 1);
    REQUIRE(ContainsRoad(result[0], RoadID(0)));
    REQUIRE(ContainsRoad(result[0], RoadID(1)));
}

TEST_CASE("Three roads at the same point merge into one intersection") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});
    graph.AddNode(NodeID(4), {0.0, 0.0});
    graph.AddNode(NodeID(5), {10.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1)); // RoadID(0) horizontal
    AddStraightRoad(graph, NodeID(2), NodeID(3)); // RoadID(1) vertical
    AddStraightRoad(graph, NodeID(4), NodeID(5)); // RoadID(2) diagonal

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0].intersecting_roads.size() == 3);
    REQUIRE(ContainsRoad(result[0], RoadID(0)));
    REQUIRE(ContainsRoad(result[0], RoadID(1)));
    REQUIRE(ContainsRoad(result[0], RoadID(2)));
    REQUIRE(result[0].pos.x_coord == Catch::Approx(5.0));
    REQUIRE(result[0].pos.y_coord == Catch::Approx(5.0));
}

TEST_CASE("Four roads at the same point merge into one intersection") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});
    graph.AddNode(NodeID(4), {0.0, 0.0});
    graph.AddNode(NodeID(5), {10.0, 10.0});
    graph.AddNode(NodeID(6), {0.0, 10.0});
    graph.AddNode(NodeID(7), {10.0, 0.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1)); // horizontal
    AddStraightRoad(graph, NodeID(2), NodeID(3)); // vertical
    AddStraightRoad(graph, NodeID(4), NodeID(5)); // diagonal /
    AddStraightRoad(graph, NodeID(6), NodeID(7)); // diagonal backslash

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0].intersecting_roads.size() == 4);
    REQUIRE(result[0].pos.x_coord == Catch::Approx(5.0));
    REQUIRE(result[0].pos.y_coord == Catch::Approx(5.0));
}

TEST_CASE("No duplicate road IDs in a merged intersection") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});
    graph.AddNode(NodeID(4), {0.0, 0.0});
    graph.AddNode(NodeID(5), {10.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(3));
    AddStraightRoad(graph, NodeID(4), NodeID(5));

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 1);

    const auto& ids = result[0].intersecting_roads;
    auto sorted = ids;
    std::sort(sorted.begin(), sorted.end(),
              [](RoadID a, RoadID b) { return a.value() < b.value(); });
    auto last = std::unique(sorted.begin(), sorted.end());
    REQUIRE(std::distance(sorted.begin(), last) == static_cast<long>(ids.size()));
}

TEST_CASE("Two distinct crossing points remain as separate intersections") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {20.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});
    graph.AddNode(NodeID(4), {15.0, 0.0});
    graph.AddNode(NodeID(5), {15.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1)); // long horizontal
    AddStraightRoad(graph, NodeID(2), NodeID(3)); // vertical at x=5
    AddStraightRoad(graph, NodeID(4), NodeID(5)); // vertical at x=15

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.size() == 2);
    REQUIRE(result[0].intersecting_roads.size() == 2);
    REQUIRE(result[1].intersecting_roads.size() == 2);

    double x0 = result[0].pos.x_coord;
    double x1 = result[1].pos.x_coord;
    REQUIRE(std::min(x0, x1) == Catch::Approx(5.0));
    REQUIRE(std::max(x0, x1) == Catch::Approx(15.0));
}

TEST_CASE("Roads sharing an endpoint do not produce an intersection") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 0.0});
    graph.AddNode(NodeID(1), {5.0, 10.0});
    graph.AddNode(NodeID(2), {10.0, 0.0});
    graph.AddNode(NodeID(4), {5.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(4));

    auto result = IntersectionFinder::FindIntersections(graph);
    REQUIRE(result.empty());
}

TEST_CASE("AddIntersections stores results retrievable via GetIntersections") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(3));

    REQUIRE(graph.GetIntersections().empty());

    auto intersections = IntersectionFinder::FindIntersections(graph);
    graph.AddIntersections(std::move(intersections));

    REQUIRE(graph.GetIntersections().size() == 1);
    REQUIRE(graph.GetIntersections()[0].intersecting_roads.size() == 2);
    REQUIRE(graph.GetIntersections()[0].pos.x_coord == Catch::Approx(5.0));
    REQUIRE(graph.GetIntersections()[0].pos.y_coord == Catch::Approx(5.0));
}

TEST_CASE("AddIntersections replaces previous intersections") {
    RoadGraph graph;
    graph.AddNode(NodeID(0), {0.0, 5.0});
    graph.AddNode(NodeID(1), {10.0, 5.0});
    graph.AddNode(NodeID(2), {5.0, 0.0});
    graph.AddNode(NodeID(3), {5.0, 10.0});

    AddStraightRoad(graph, NodeID(0), NodeID(1));
    AddStraightRoad(graph, NodeID(2), NodeID(3));

    auto first = IntersectionFinder::FindIntersections(graph);
    graph.AddIntersections(std::move(first));
    REQUIRE(graph.GetIntersections().size() == 1);

    std::vector<RoadIntersection> empty;
    graph.AddIntersections(std::move(empty));
    REQUIRE(graph.GetIntersections().empty());
}
