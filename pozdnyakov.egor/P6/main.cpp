#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <new>
#include <limits>
#include <iomanip>

namespace pozdnyakov
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  class Shape
  {
  public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    virtual void scale(double k)
    {
      if (k < 0.0) {
        throw std::invalid_argument("Scaling coefficient must be non-negative");
      }
      doScale(k);
    }
    virtual Shape* clone() const = 0;

    void unsafeScale(double k) noexcept
    {
      doScale(k);
    }

  private:
    virtual void doScale(double k) noexcept = 0;
  };
}
