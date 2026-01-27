#include "compositeShape.hpp"
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <memory>

pozdnyakov::CompositeShape::CompositeShape() noexcept:
  count_(0),
  capacity_(0),
  shapes_(nullptr)
{}

pozdnyakov::CompositeShape::CompositeShape(const CompositeShape &other):
  count_(other.count_),
  capacity_(other.count_),
  shapes_(new Shape *[other.count_])
{
  unsigned int i = 0;
  try {
    for (; i < count_; ++i) {
      shapes_[i] = other.shapes_[i]->clone();
    }
  } catch (...) {
    for (unsigned int k = 0; k < i; ++k) {
      delete shapes_[k];
    }
    delete[] shapes_;
    throw;
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
  if (this != std::addressof(other)) {
    CompositeShape temp(other);
    swap(temp);
  }
  return *this;
}

pozdnyakov::CompositeShape &pozdnyakov::CompositeShape::operator=(CompositeShape &&other) noexcept
{
  if (this != std::addressof(other)) {
    CompositeShape temp(std::move(other));
    swap(temp);
  }
  return *this;
}

pozdnyakov::CompositeShape::~CompositeShape()
{
  for (unsigned int i = 0; i < count_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::operator[](unsigned int index)
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  return shapes_[index];
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::operator[](unsigned int index) const
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  return shapes_[index];
}

void pozdnyakov::CompositeShape::addShape(Shape *shape)
{
  if (!shape) {
    throw std::invalid_argument("Null shape");
  }
  if (count_ >= capacity_) {
    unsigned int newCap = (capacity_ == 0) ? 1 : capacity_ * 2;
    Shape **newShapes = new Shape *[newCap];
    for (unsigned int i = 0; i < count_; ++i) {
      newShapes[i] = shapes_[i];
    }
    delete[] shapes_;
    shapes_ = newShapes;
    capacity_ = newCap;
  }
  shapes_[count_++] = shape;
}

void pozdnyakov::CompositeShape::removeShape(unsigned int index)
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  delete shapes_[index];
  for (unsigned int i = index; i < count_ - 1; ++i) {
    shapes_[i] = shapes_[i + 1];
  }
  shapes_[count_ - 1] = nullptr;
  count_--;
}

unsigned int pozdnyakov::CompositeShape::size() const noexcept
{
  return count_;
}

double pozdnyakov::CompositeShape::getArea() const noexcept
{
  double total = 0.0;
  for (unsigned int i = 0; i < count_; ++i) {
    total += shapes_[i]->getArea();
  }
  return total;
}

pozdnyakov::rectangle_t pozdnyakov::CompositeShape::getFrameRect() const noexcept
{
  if (count_ == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }

  rectangle_t frame = shapes_[0]->getFrameRect();
  double minX = frame.pos.x - frame.width / 2.0;
  double maxX = frame.pos.x + frame.width / 2.0;
  double minY = frame.pos.y - frame.height / 2.0;
  double maxY = frame.pos.y + frame.height / 2.0;

  for (unsigned int i = 1; i < count_; ++i) {
    frame = shapes_[i]->getFrameRect();
    double currentMinX = frame.pos.x - frame.width / 2.0;
    double currentMaxX = frame.pos.x + frame.width / 2.0;
    double currentMinY = frame.pos.y - frame.height / 2.0;
    double currentMaxY = frame.pos.y + frame.height / 2.0;

    minX = std::min(minX, currentMinX);
    maxX = std::max(maxX, currentMaxX);
    minY = std::min(minY, currentMinY);
    maxY = std::max(maxY, currentMaxY);
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
  for (unsigned int i = 0; i < count_; ++i) {
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
  for (unsigned int i = 0; i < count_; ++i) {
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
