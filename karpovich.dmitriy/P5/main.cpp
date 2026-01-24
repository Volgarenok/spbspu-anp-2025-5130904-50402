#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <shapes.hpp>

int main()
{
  namespace karp = karpovich;
  karp::Rectangle rect{4.0, 2.0, {1.0, 1.0}};
  karp::Ellipse ell{3.0, 2.0, {-2.0, 0.0}};
  karp::Rubber rub{5.0, 2.0, {0.0, 0.0}, {1.0, 1.0}};
  const size_t n = 3;
  karp::Shape* shapes[n] = {std::addressof(rect), std::addressof(ell), std::addressof(rub)};
  karp::output(shapes, n);

  karp::point_t pt;
  double k = 0;
  if (!(std::cin >> pt.x >> pt.y >> k)) {
    std::cerr << "err: failed to read input\n";
    return 1;
  }

  try {
    scalefrompt(shapes, n, k, pt);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what();
    return 1;
  }

  karp::output(shapes, n);
  return 0;
}
