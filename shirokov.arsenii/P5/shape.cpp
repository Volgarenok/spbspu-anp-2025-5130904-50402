#include "shape.hpp"
#include <stdexcept>

void shirokov::Shape::safeScale(double coefficient)
{
  if (coefficient <= 0)
  {
    throw std::logic_error("The coefficient must be positive");
  }
  scale(coefficient);
}
