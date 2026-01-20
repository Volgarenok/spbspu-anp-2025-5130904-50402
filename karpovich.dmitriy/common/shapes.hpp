#ifndef SHAPES_HPP
#define SHAPES_HPP
#include <cmath>
#include <podshape.hpp>
namespace karpovich
{
  const double PI = std::acos(-1.0);
  struct Rectangle final: Shape
  {
    Rectangle(double width, double height, point_t centr) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double width_, height_;
      point_t centr_;
      void doScale(double k) noexcept override;
  };
  struct Rubber final: Shape
  {
    Rubber(double radius1, double radius2, point_t centr1, point_t centr2) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double radius1_, radius2_;
      point_t centr1_, centr2_;
      void doScale(double k) noexcept override;
  };
  struct Ellipse final: Shape
  {
    Ellipse(double semiax1, double semiax2, point_t centr) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t p) noexcept override;
    private:
      double semiax1_, semiax2_;
      point_t centr_;
      void doScale(double k) noexcept override;
  };
}
#endif
