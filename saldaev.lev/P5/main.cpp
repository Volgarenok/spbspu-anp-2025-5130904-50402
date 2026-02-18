#include <cmath>
#include <iostream>
#include "geom.hpp"
#include "polygon.hpp"
#include "rectangle.hpp"
#include "shape.hpp"
#include "square.hpp"

namespace saldaev
{
  const size_t ShapeAmount = 3;

  void isotropicScaleFromPoint(Shape **shps, size_t k, point_t pt, double coef);
  void tellStatistics(const Shape *const *shps, size_t k) noexcept;
}

int main()
{
  using saldaev::point_t;
  using saldaev::Polygon;
  using saldaev::Rectangle;
  using saldaev::Shape;
  using saldaev::Square;

  Shape *shps[saldaev::ShapeAmount] = {nullptr, nullptr, nullptr};
  try {
    shps[0] = new Rectangle({10, 10}, 5, 3);
    shps[1] = new Square({-1, -1}, 10);
    point_t vss[6] = {{0, 0}, {-2, 3}, {-1, 5}, {0, 4}, {1, 5}, {2, 3}};
    shps[2] = new Polygon(vss, 6);
  } catch (const std::exception &e) {
    for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
      delete shps[i];
    }
    return 1;
  }

  std::cout << "before:\n";
  saldaev::tellStatistics(shps, saldaev::ShapeAmount);

  double x = 0, y = 0, c = 0;
  std::cin >> x >> y >> c;
  if (std::cin.fail() || c <= 0) {
    std::cerr << "bad input" << '\n';
    for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
      delete shps[i];
    }
    return 1;
  }

  isotropicScaleFromPoint(shps, saldaev::ShapeAmount, {x, y}, c);

  std::cout << "\nafter:\n";
  saldaev::tellStatistics(shps, saldaev::ShapeAmount);

  for (size_t i = 0; i < saldaev::ShapeAmount; ++i) {
    delete shps[i];
  }

  return 0;
}

void saldaev::isotropicScaleFromPoint(Shape **shps, size_t k, point_t pt, double coef)
{
  for (size_t i = 0; i < k; ++i) {
    point_t p1 = shps[i]->getFrameRect().pos;
    shps[i]->move(pt);
    point_t p2 = shps[i]->getFrameRect().pos;
    shps[i]->move(pt + ((p1 - p2) * coef));
    shps[i]->nsfwscale(coef);
  }
}

void saldaev::tellStatistics(const Shape *const *shps, size_t k) noexcept
{
  double area = 0;
  double sum_area = 0;
  saldaev::rectangle_t frame = {0, 0, {0, 0}};
  double mix = shps[0]->getFrameRect().pos.x - shps[0]->getFrameRect().width / 2;
  double max = mix;
  double miy = shps[0]->getFrameRect().pos.y - shps[0]->getFrameRect().height / 2;
  double may = miy;

  for (size_t i = 0; i < k; ++i) {
    area = shps[i]->getArea();
    frame = shps[i]->getFrameRect();
    std::cout << " * own area - " << area << ", frame: {" << frame.pos.x << ", ";
    std::cout << frame.pos.y << "}, w - " << frame.width << ", h - " << frame.height << '\n';
    sum_area += area;
    mix = std::min(mix, frame.pos.x - frame.width / 2);
    max = std::max(max, frame.pos.x + frame.width / 2);
    miy = std::min(miy, frame.pos.y - frame.height / 2);
    may = std::max(may, frame.pos.y + frame.height / 2);
  }
  std::cout << "sum area - " << sum_area << ", frame: {" << (mix + max) / 2 << ", ";
  std::cout << (miy + may) / 2 << "}, w - " << max - mix << ", h - " << may - miy << "\n";
}
