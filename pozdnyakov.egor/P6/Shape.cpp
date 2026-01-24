#include "shape.hpp"
#include <stdexcept>

void pozdnyakov::Shape::scale(double k)
{
  if (k < 0.0) {
    throw std::invalid_argument("Scaling coefficient must be non-negative");
  }
  doScale(k);
}

void pozdnyakov::Shape::unsafeScale(double k) noexcept
{
  doScale(k);
}
