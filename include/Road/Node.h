#pragma once
#include "Geometry/Position.h"
#include "Identifiers/IDs.h"

class Node {
  public:
    explicit Node(NodeID id, const Position& coords) noexcept : id_(id), pos_(coords) {}

    Node(const Node&) = default;
    Node(Node&&) noexcept = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) noexcept = default;

    NodeID Id() const noexcept { return id_; }
    Position Pos() const noexcept { return pos_; }

  private:
    NodeID id_;
    Position pos_;
};
