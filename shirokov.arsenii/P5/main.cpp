#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>
#include <stdexcept>
#include "pods.hpp"
#include "shape.hpp"
#include "rectangle.hpp"
#include "polygon.hpp"
#include "xquare.hpp"

namespace shirokov
{
  void scaleAboutPoint(const point_t &target, double coefficient, Shape *figure);
  void unsafeScaleAboutPoint(const point_t &target, double coefficient, Shape *figure);
  void scaleAboutPoint(const point_t &target, double coefficient, Shape *const *figures, size_t s);
  double getTotalArea(const Shape *const *figures, size_t s);
  rectangle_t getTotalFrameRect(const Shape *const *figures, size_t s);
  void printInfo(const Shape *const *figures, size_t s);
  void printTabs(size_t k);
  void printFrameInfo(const rectangle_t &frameRect, size_t indents);
  const size_t SIZE = 5;
}

int main()
{
  shirokov::Shape *figures[shirokov::SIZE];
  for (size_t i = 0; i < shirokov::SIZE; ++i)
  {
    figures[i] = nullptr;
  }
  try
  {
    figures[0] = new shirokov::Rectangle({5, 5}, 10, 10);
    figures[1] = new shirokov::Rectangle({0, 0}, {10, 10});
    const shirokov::point_t vertices[] = {{0, 0}, {4, 0}, {4, 3}, {0, 3}};
    figures[2] = new shirokov::Polygon(vertices, 4);
    figures[3] = new shirokov::Xquare({0, 0}, 5);
    figures[4] = new shirokov::Xquare({0, 5}, {0, 0});
  }
  catch (const std::bad_alloc &)
  {
    for (size_t i = 0; i < shirokov::SIZE; ++i)
    {
      delete figures[i];
    }
    std::cerr << "Couldn't allocate memory\n";
    return 2;
  }

  std::cout << "Before scaling:\n";
  shirokov::printInfo(figures, shirokov::SIZE);

  shirokov::point_t target = {0, 0};
  double coefficient = 0;
  std::cin >> target.x >> target.y >> coefficient;
  if (std::cin.fail())
  {
    std::cerr << "Input error\n";
    for (size_t i = 0; i < shirokov::SIZE; ++i)
    {
      delete figures[i];
    }
    return 1;
  }
  try
  {
    shirokov::scaleAboutPoint(target, coefficient, figures, shirokov::SIZE);
  }
  catch (const std::logic_error &e)
  {
    std::cerr << e.what() << '\n';
    for (size_t i = 0; i < shirokov::SIZE; ++i)
    {
      delete figures[i];
    }
    return 3;
  }
  std::cout << "After scaling:\n";
  shirokov::printInfo(figures, shirokov::SIZE);

  for (size_t i = 0; i < shirokov::SIZE; ++i)
  {
    delete figures[i];
  }
}

void shirokov::scaleAboutPoint(const point_t &target, double coefficient, Shape *figure)
{
  point_t point1 = figure->getFrameRect().pos;
  figure->move(target);
  point_t delta = {target.x - point1.x, target.y - point1.y};
  figure->safeScale(coefficient);
  point_t res = {target.x - delta.x * coefficient, target.y - delta.y * coefficient};
  figure->move(res);
}

void shirokov::unsafeScaleAboutPoint(const point_t &target, double coefficient, Shape *figure)
{
  point_t point1 = figure->getFrameRect().pos;
  figure->move(target);
  point_t delta = {target.x - point1.x, target.y - point1.y};
  figure->scale(coefficient);
  point_t res = {target.x - delta.x * coefficient, target.y - delta.y * coefficient};
  figure->move(res);
}

void shirokov::scaleAboutPoint(const point_t &target, double coefficient, Shape *const *figures, size_t s)
{
  if (coefficient <= 0)
  {
    throw std::logic_error("The coefficient must be positive");
  }
  for (size_t i = 0; i < s; ++i)
  {
    unsafeScaleAboutPoint(target, coefficient, figures[i]);
  }
}

double shirokov::getTotalArea(const Shape *const *figures, size_t s)
{
  double res = 0;
  for (size_t i = 0; i < s; ++i)
  {
    res += figures[i]->getArea();
  }
  return res;
}

shirokov::rectangle_t shirokov::getTotalFrameRect(const Shape *const *figures, size_t s)
{
  rectangle_t frameRect = figures[0]->getFrameRect();
  double minX = frameRect.pos.x - frameRect.width / 2;
  double maxX = frameRect.pos.x + frameRect.width / 2;
  double minY = frameRect.pos.y - frameRect.height / 2;
  double maxY = frameRect.pos.y + frameRect.height / 2;
  for (size_t i = 1; i < s; ++i)
  {
    frameRect = figures[i]->getFrameRect();
    double left = frameRect.pos.x - frameRect.width / 2;
    double right = frameRect.pos.x + frameRect.width / 2;
    double bottom = frameRect.pos.y - frameRect.height / 2;
    double top = frameRect.pos.y + frameRect.height / 2;

    minX = std::min(minX, left);
    maxX = std::max(maxX, right);
    minY = std::min(minY, bottom);
    maxY = std::max(maxY, top);
  }
  double width = maxX - minX;
  double height = maxY - minY;
  point_t pos = {(maxX + minX) / 2, (maxY + minY) / 2};

  return {width, height, pos};
}

void shirokov::printInfo(const Shape *const *figures, size_t s)
{
  std::cout << "Areas of figures:\n";
  for (size_t i = 0; i < s; ++i)
  {
    std::cout << "\tFigure " << i + 1 << ": " << figures[i]->getArea() << '\n';
  }
  std::cout << "Total area: " << getTotalArea(figures, s) << '\n';
  std::cout << "Frame rect of figures:\n";
  for (size_t i = 0; i < s; ++i)
  {
    rectangle_t frameRect = figures[i]->getFrameRect();
    std::cout << "\tFigure " << i + 1 << ": \n";
    printFrameInfo(frameRect, 2);
  }
  rectangle_t frameRect = getTotalFrameRect(figures, s);
  std::cout << "Total frame rect: \n";
  printFrameInfo(frameRect, 1);
}

void shirokov::printTabs(size_t k)
{
  for (size_t i = 0; i < k; ++i)
  {
    std::cout << '\t';
  }
}

void shirokov::printFrameInfo(const rectangle_t &frameRect, size_t k)
{
  printTabs(k);
  std::cout << "Width: " << frameRect.width << '\n';
  printTabs(k);
  std::cout << "Height: " << frameRect.height << '\n';
  printTabs(k);
  std::cout << "Center:\n";
  printTabs(k);
  std::cout << "\tx: " << frameRect.pos.x << '\n';
  printTabs(k);
  std::cout << "\ty: " << frameRect.pos.y << '\n';
}
