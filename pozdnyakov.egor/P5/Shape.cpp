#include "Shape.hpp"
#include <stdexcept>

namespace pozdnyakov {
  void Shape::scale(double k) {
    if (k < 0.0) {
      throw std::invalid_argument("Scaling coefficient must be non-negative");
    }
    doScale(k);
  }

  void Shape::unsafeScale(double k) noexcept {
    doScale(k);
  }
}
