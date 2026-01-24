#include <iostream>
#include <iomanip>
#include <new>
#include "rectangle.hpp"
#include "diamond.hpp"
#include "triangle.hpp"
#include "compositeShape.hpp"

void printInfo(const pozdnyakov::Shape &shape, const std::string &name)
{
  std::cout << name << ":\n";
  std::cout << "  Area: " << shape.getArea() << "\n";
  pozdnyakov::rectangle_t frame = shape.getFrameRect();
  std::cout << "  Frame: pos(" << frame.pos.x << ", " << frame.pos.y << "), w=" << frame.width << ", h=" << frame.height
            << "\n\n";
}

int main()
{
  using namespace pozdnyakov;

  std::cout << std::fixed << std::setprecision(1);

  try {
    CompositeShape composite;

    composite.addShape(new Rectangle({5.0, 5.0}, 10.0, 5.0));
    composite.addShape(new Diamond({20.0, 5.0}, 10.0, 10.0));
    composite.addShape(new Triangle({0.0, 0.0}, {5.0, 10.0}, {10.0, 0.0}));

    printInfo(composite, "Composite initial");

    double dx = 0.0;
    double dy = 0.0;
    double k = 0.0;

    if (!(std::cin >> dx >> dy >> k)) {
      std::cerr << "Incorrect input\n";
      return 1;
    }

    if (k < 0.0) {
      std::cerr << "Invalid scale coefficient\n";
      return 1;
    }

    composite.move(dx, dy);
    composite.scale(k);

    printInfo(composite, "Composite scaled");

    CompositeShape copy = composite;
    copy.move(100.0, 100.0);

    printInfo(composite, "Original");
    printInfo(copy, "Copy");
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed " << e.what() << "\n";
    return 2;
  } catch (const std::exception &e) {
    std::cerr << "Error " << e.what() << "\n";
    return 1;
  }

  return 0;
}
