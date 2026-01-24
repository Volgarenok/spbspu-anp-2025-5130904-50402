#include "compositeShape.hpp"
#include <algorithm>
#include <stdexcept>
#include <limits>

pozdnyakov::CompositeShape::CompositeShape():
  count_(0),
  capacity_(0),
  shapes_(nullptr)
{}

pozdnyakov::CompositeShape::CompositeShape(const CompositeShape &other):
  count_(other.count_),
  capacity_(other.count_),
  shapes_(new Shape *[other.count_])
{
  for (size_t i = 0; i < count_; ++i) {
    shapes_[i] = other.shapes_[i]->clone();
  }
}

pozdnyakov::CompositeShape::CompositeShape(CompositeShape &&other) noexcept:
  count_(other.count_),
  capacity_(other.capacity_),
  shapes_(other.shapes_)
{
  other.count_ = 0;
  other.capacity_ = 0;
  other.shapes_ = nullptr;
}

pozdnyakov::CompositeShape &pozdnyakov::CompositeShape::operator=(const CompositeShape &other)
{
  if (this != &other) {
    CompositeShape temp(other);
    swap(temp);
  }
  return *this;
}

pozdnyakov::CompositeShape &pozdnyakov::CompositeShape::operator=(CompositeShape &&other) noexcept
{
  if (this != &other) {
    CompositeShape temp(std::move(other));
    swap(temp);
  }
  return *this;
}

pozdnyakov::CompositeShape::~CompositeShape()
{
  for (size_t i = 0; i < count_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
}

void pozdnyakov::CompositeShape::addShape(Shape *shape)
{
  if (!shape) {
    throw std::invalid_argument("Null shape");
  }
  if (count_ >= capacity_) {
    size_t newCap = (capacity_ == 0) ? 1 : capacity_ * 2;
    Shape **newShapes = new Shape *[newCap];
    for (size_t i = 0; i < count_; ++i) {
      newShapes[i] = shapes_[i];
    }
    delete[] shapes_;
    shapes_ = newShapes;
    capacity_ = newCap;
  }
  shapes_[count_++] = shape;
}

void pozdnyakov::CompositeShape::removeShape(size_t index)
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  delete shapes_[index];
  for (size_t i = index; i < count_ - 1; ++i) {
    shapes_[i] = shapes_[i + 1];
  }
  shapes_[count_ - 1] = nullptr;
  count_--;
}

size_t pozdnyakov::CompositeShape::getCount() const noexcept
{
  return count_;
}

double pozdnyakov::CompositeShape::getArea() const noexcept
{
  double total = 0.0;
  for (size_t i = 0; i < count_; ++i) {
    total += shapes_[i]->getArea();
  }
  return total;
}

// Исправлено: убрано ошибочное CompositeShape:: перед rectangle_t
pozdnyakov::rectangle_t pozdnyakov::CompositeShape::getFrameRect() const noexcept
{
  if (count_ == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  double minX = std::numeric_limits< double >::infinity();
  double maxX = -std::numeric_limits< double >::infinity();
  double minY = std::numeric_limits< double >::infinity();
  double maxY = -std::numeric_limits< double >::infinity();

  for (size_t i = 0; i < count_; ++i) {
    rectangle_t frame = shapes_[i]->getFrameRect();
    minX = std::min(minX, frame.pos.x - frame.width / 2.0);
    maxX = std::max(maxX, frame.pos.x + frame.width / 2.0);
    minY = std::min(minY, frame.pos.y - frame.height / 2.0);
    maxY = std::max(maxY, frame.pos.y + frame.height / 2.0);
  }
  return {maxX - minX, maxY - minY, {(minX + maxX) / 2.0, (minY + maxY) / 2.0}};
}

void pozdnyakov::CompositeShape::move(const point_t &pos) noexcept
{
  point_t center = getFrameRect().pos;
  move(pos.x - center.x, pos.y - center.y);
}

void pozdnyakov::CompositeShape::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < count_; ++i) {
    shapes_[i]->move(dx, dy);
  }
}

void pozdnyakov::CompositeShape::scale(double k)
{
  Shape::scale(k);
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::clone() const
{
  return new CompositeShape(*this);
}

void pozdnyakov::CompositeShape::doScale(double k) noexcept
{
  point_t center = getFrameRect().pos;
  for (size_t i = 0; i < count_; ++i) {
    point_t shapePos = shapes_[i]->getFrameRect().pos;
    double dx = (shapePos.x - center.x) * (k - 1.0);
    double dy = (shapePos.y - center.y) * (k - 1.0);
    shapes_[i]->move(dx, dy);
    shapes_[i]->unsafeScale(k);
  }
}

void pozdnyakov::CompositeShape::swap(CompositeShape &other) noexcept
{
  std::swap(count_, other.count_);
  std::swap(capacity_, other.capacity_);
  std::swap(shapes_, other.shapes_);
}
