#include <iostream>
#include <shapes.hpp>
#include "storageActions.hpp"

int main()
{
  namespace karp = karpovich;
  karp::Rectangle rect{4.0, 2.0, {1.0, 1.0}};
  karp::Ellipse ell{3.0, 2.0, {-2.0, 0.0}};
  karp::Rubber rub{5.0, 2.0, {0.0, 0.0}, {1.0, 1.0}};
  karp::StorageShape shps = karp::StorageShape();
  shps.append(std::addressof(rect));
  shps.append(std::addressof(ell));
  shps.append(std::addressof(rub));
  karp::output(shps, shps.size());
  karp::point_t pt = {};
  double k = 0;
  if (!(std::cin >> pt.x >> pt.y >> k)) {
    std::cerr << "err: failed to read input\n";
    return 1;
  }
  try {
    scalefrompt(shps, shps.size(), k, pt);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  karp::output(shps, shps.size());
  return 0;
}
