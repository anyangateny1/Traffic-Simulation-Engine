#include "Road/Lane.h"

Lane::Lane(Road* parent, int index, LaneDirection dir, double offset, double width)
    : parent_road_(parent), lane_index_(index), direction_(dir),
      lateral_offset_(offset), width_(width) {}
