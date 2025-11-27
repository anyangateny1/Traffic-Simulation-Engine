#pragma once
#include "Road/Node.h"

class Road {
    Node from_;
    Node to_;
    double length_;


    Road(const Road&) = delete;
    Road(Road&&) = delete;
    Road& operator=(const Road&) = delete;
    Road& operator=(Road&&) = delete;
};
