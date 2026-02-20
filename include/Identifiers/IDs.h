#pragma once
#include "StrongID.h"

struct NodeTag {};
struct RoadTag {};
struct LaneTag {};
struct VehicleTag {};
struct IntersectionTag {};

using NodeID = StrongID<NodeTag>;
using RoadID = StrongID<RoadTag>;
using LaneID = StrongID<LaneTag>;
using VehicleID = StrongID<VehicleTag>;
using IntersectionID = StrongID<IntersectionTag>;
