#pragma once

class Node {
  public:
    explicit Node(int id, double x_coord, double y_coord) noexcept
        : id_(id), xCoordinate_(x_coord), yCoordinate_(y_coord) {}

    Node(const Node& other) = delete;
    Node operator=(const Node& other) = delete;
    Node(Node&& other) = delete;
    Node operator=(Node&& other) = delete;

    int id_{};
    double xCoordinate_{};
    double yCoordinate_{};
};
