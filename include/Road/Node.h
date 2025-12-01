#pragma once

class Node {
  public:
    explicit Node(int id, double x_coord, double y_coord) noexcept
        : id_(id), x_coordinate_(x_coord), y_coordinate_(y_coord) {}

    Node(const Node&) = default;
    Node(Node&&) noexcept = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) noexcept = default;

    int Id() const noexcept { return id_; }
    double X() const noexcept { return x_coordinate_; }
    double Y() const noexcept { return y_coordinate_; }

  private:
    int id_;
    double x_coordinate_;
    double y_coordinate_;
};
