#pragma once

#include "Identifiers/IDs.h"

class IntersectionPolicy {
  public:
    virtual ~IntersectionPolicy() = default;

    virtual bool CanProceed(VehicleID vehicle, RoadID from_road) = 0;

    virtual void Tick(float dt) = 0;
};
