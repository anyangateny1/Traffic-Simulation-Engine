#pragma once
#include "Geometry/Position.h"

class Node {
  public:
    explicit Node(int id, const Position& coords) noexcept : id_(id), pos_(coords) {}

    Node(const Node&) = default;
    Node(Node&&) noexcept = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) noexcept = default;

    int Id() const noexcept { return id_; }
    Position Pos() const noexcept { return pos_; }

  private:
    int id_;
    Position pos_;
};
