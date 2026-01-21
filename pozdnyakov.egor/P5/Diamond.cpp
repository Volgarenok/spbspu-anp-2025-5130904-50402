#include "Diamond.hpp"

namespace pozdnyakov {
  Diamond::Diamond(const point_t& center, double diag_h, double diag_v) :
    center_(center),
    diag_h_(diag_h),
    diag_v_(diag_v)
  {
  }

  double Diamond::getArea() const noexcept {
    return (diag_h_ * diag_v_) / 2.0;
  }

  rectangle_t Diamond::getFrameRect() const noexcept {
    return { diag_h_, diag_v_, center_ };
  }

  void Diamond::move(const point_t& pos) noexcept {
    center_ = pos;
  }

  void Diamond::move(double dx, double dy) noexcept {
    center_.x += dx;
    center_.y += dy;
  }

  void Diamond::doScale(double k) noexcept {
    diag_h_ *= k;
    diag_v_ *= k;
  }
}
