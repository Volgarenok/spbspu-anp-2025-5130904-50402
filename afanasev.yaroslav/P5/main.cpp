#include <iostream>

namespace afanasev
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  class Shape
  {
  public:
    virtual ~Shape() = default;

    virtual double getArea() const = 0;

    virtual rectangle_t getFrameRect() const = 0;

    virtual void move(const point_t& point) = 0;
    virtual void move(double x, double y) = 0;

    virtual void scale(double coefficient) = 0;
  };
}



int main()
{
  using afanasev::point_t;
  using afanasev::rectangle_t;
  using afanasev::Shape;

  return 0;
}