#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <shapes.hpp>

int main()
{
  constexpr size_t pol_n = 5;
  constexpr size_t n = 3;
  dirko::Rectangle rec{5, 7, {3, 3}};
  dirko::Bubble bub{5, {2, 0}};
  dirko::point_t pts[pol_n] = {{2, 2}, {2, 7}, {2.5, 8}, {3, 7}, {3, 2}};
  dirko::Polygon pol{pol_n, pts};
  dirko::Shape *shps[n] = {std::addressof(rec), std::addressof(bub), std::addressof(pol)};
  std::cout << "Before scale:\n\n";
  dirko::output(std::cout, shps, n);
  dirko::point_t point = {};
  double coef = 0;
  if (!(std::cin >> point.x >> point.y >> coef)) {
    std::cerr << "Cant read\n";
    return 1;
  }
  if (coef <= 0) {
    std::cerr << "Non positive coef\n";
    return 1;
  }
  try {
    dirko::scaleFromPoint(shps, n, point, coef);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << "\n\nAfter scale:\n\n";
  dirko::output(std::cout, shps, n);
}
