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
  clear();
  delete[] shapes_;
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::operator[](unsigned int index)
{
  return get(index);
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::operator[](unsigned int index) const
{
  return get(index);
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::at(unsigned int index)
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  return shapes_[index];
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::at(unsigned int index) const
{
  if (index >= count_) {
    throw std::out_of_range("Index out of range");
  }
  return shapes_[index];
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::get(unsigned int index)
{
  return shapes_[index];
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::get(unsigned int index) const
{
  return shapes_[index];
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::first()
{
  if (empty()) {
    throw std::out_of_range("Composite Shape is empty");
  }
  return shapes_[0];
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::first() const
{
  if (empty()) {
    throw std::out_of_range("Composite Shape is empty");
  }
  return shapes_[0];
}

pozdnyakov::Shape *pozdnyakov::CompositeShape::last()
{
  if (empty()) {
    throw std::out_of_range("Composite Shape is empty");
  }
  return shapes_[count_ - 1];
}

const pozdnyakov::Shape *pozdnyakov::CompositeShape::last() const
{
  if (empty()) {
    throw std::out_of_range("Composite Shape is empty");
  }
  return shapes_[count_ - 1];
}

void pozdnyakov::CompositeShape::append(Shape *shape)
{
  add(shape, count_);
}

void pozdnyakov::CompositeShape::preappend(Shape *shape)
{
  add(shape, 0);
}

void pozdnyakov::CompositeShape::add(Shape *shape, unsigned int index)
{
  if (!shape) {
    throw std::invalid_argument("Null shape");
  }
  if (index > count_) {
    throw std::out_of_range("Index out of range");
  }

  if (count_ >= capacity_) {
    reserve((capacity_ == 0) ? 1 : capacity_ * 2);
  }

  for (unsigned int i = count_; i > index; --i) {
    shapes_[i] = shapes_[i - 1];
  }
  shapes_[index] = shape;
  count_++;
}

void pozdnyakov::CompositeShape::remove(unsigned int index)
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

void pozdnyakov::CompositeShape::dropFirst()
{
  remove(0);
}

void pozdnyakov::CompositeShape::dropLast()
{
  remove(count_ - 1);
}

void pozdnyakov::CompositeShape::clear()
{
  for (unsigned int i = 0; i < count_; ++i) {
    delete shapes_[i];
  }
  count_ = 0;
}

unsigned int pozdnyakov::CompositeShape::size() const noexcept
{
  return count_;
}

bool pozdnyakov::CompositeShape::empty() const noexcept
{
  return count_ == 0;
}

unsigned int pozdnyakov::CompositeShape::capacity() const noexcept
{
  return capacity_;
}

void pozdnyakov::CompositeShape::reserve(unsigned int newCapacity)
{
  if (newCapacity <= capacity_) {
    return;
  }
  Shape **newShapes = new Shape *[newCapacity];
  for (unsigned int i = 0; i < count_; ++i) {
    newShapes[i] = shapes_[i];
  }
  delete[] shapes_;
  shapes_ = newShapes;
  capacity_ = newCapacity;
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
