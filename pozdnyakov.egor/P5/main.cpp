#include <iostream>
#include <new>
#include "Shape.hpp"
#include "Rectangle.hpp"
#include "Diamond.hpp"
#include "Triangle.hpp"
#include "utils.hpp"

int main()
{
  using namespace pozdnyakov;

  const size_t count = 3;
  Shape **shapes = new Shape *[count] {
  };

  try {
    shapes[0] = new Rectangle({5.0, 5.0}, 10.0, 5.0);
    shapes[1] = new Diamond({20.0, 5.0}, 10.0, 10.0);
    shapes[2] = new Triangle({0.0, 0.0}, {5.0, 10.0}, {10.0, 0.0});
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << "\n";
    clearShapes(shapes, count);
    return 2;
  }

  printShapesInfo(shapes, count);
  std::cout << "\n";

  point_t targetPoint;
  std::cout << "Enter scaling center (x y): ";
  if (!(std::cin >> targetPoint.x >> targetPoint.y)) {
    std::cerr << "Invalid coordinates input.\n";
    clearShapes(shapes, count);
    return 1;
  }

  double k;
  std::cout << "Enter scaling coefficient: ";
  if (!(std::cin >> k)) {
    std::cerr << "Invalid input for coefficient.\n";
    clearShapes(shapes, count);
    return 1;
  }

  try {
    scaleShapes(shapes, count, targetPoint, k);
  } catch (const std::exception &e) {
    std::cerr << "Error during scaling: " << e.what() << "\n";
    clearShapes(shapes, count);
    return 1;
  }

  std::cout << "\n";
  printShapesInfo(shapes, count);

  clearShapes(shapes, count);

  return 0;
}
