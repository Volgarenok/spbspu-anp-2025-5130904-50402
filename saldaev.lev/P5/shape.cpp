#include "shape.hpp"
#include <stdexcept>

void saldaev::Shape::scale(double coef)
{
  if (coef <= 0) {
    throw std::logic_error("coef <= 0");
  }
  doScale(coef);
}

void saldaev::Shape::nsfwscale(double coef) noexcept
{
  doScale(coef);
}
