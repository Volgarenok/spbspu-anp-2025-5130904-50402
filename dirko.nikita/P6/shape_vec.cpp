#include "shape_vec.hpp"
#include <cstddef>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <utility>

dirko::Shape_vec::Shape_vec():
  shps_(nullptr),
  size_(0),
  cap_(0)
{}

dirko::Shape_vec::~Shape_vec() noexcept
{
  clear();
  size_ = 0;
}

dirko::Shape_vec::Shape_vec(const Shape_vec &other):
  shps_(other.size_ > 0 ? new Shape *[other.cap_]() : nullptr),
  size_(0),
  cap_(other.cap_)
{
  try {
    for (; size_ < other.size_; ++size_) {
      shps_[size_] = other.shps_[size_]->clone();
    }
  } catch (...) {
    clear();
    size_ = 0;
    throw;
  }
}

dirko::Shape_vec::Shape_vec(Shape_vec &&other) noexcept:
  shps_(other.shps_),
  size_(other.size_),
  cap_(other.cap_)
{
  other.shps_ = nullptr;
}

dirko::Shape_vec &dirko::Shape_vec::operator=(Shape_vec &&other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    shps_ = other.shps_;
    other.shps_ = nullptr;
    size_ = other.size_;
    cap_ = other.cap_;
  }
  return *this;
}

dirko::Shape_vec &dirko::Shape_vec::operator=(const Shape_vec &other)
{
  if (this != std::addressof(other)) {
    Shape_vec tmp(other);
    swap(tmp);
  }
  return *this;
}

dirko::rectangle_t dirko::Shape_vec::getFrameRect() const noexcept
{
  return getTotalFrame(shps_, size_);
}

void dirko::Shape_vec::scale_(double coef) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shps_[i]->doScale(coef);
  }
}

void dirko::Shape_vec::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shps_[i]->move(dx, dy);
  }
}

void dirko::Shape_vec::move(point_t point) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shps_[i]->move(point);
  }
}

double dirko::Shape_vec::getArea() const noexcept
{
  double area = 0;
  for (size_t i = 0; i < size_; ++i) {
    area += shps_[i]->getArea();
  }
  return area;
}

dirko::Shape_vec *dirko::Shape_vec::clone() const
{
  return new Shape_vec(*this);
}

void dirko::Shape_vec::append(const Shape *elem)
{
  add(elem, size_);
}
void dirko::Shape_vec::preappend(const Shape *elem)
{
  add(elem, 0);
}
void dirko::Shape_vec::add(const Shape *elem, size_t index)
{
  if (cap_ == size_) {
    if (cap_ == 0) {
      cap_ = 10;
    } else {
      cap_ *= 2;
    }
    reserve(cap_);
  }
  for (size_t i = size_; i > index; --i) {
    shps_[i] = shps_[i - 1];
  }
  shps_[index] = elem->clone();
  ++size_;
}

dirko::Shape &dirko::Shape_vec::last() noexcept
{
  return get(size_ - 1);
}
dirko::Shape &dirko::Shape_vec::first() noexcept
{
  return get(0);
}
const dirko::Shape &dirko::Shape_vec::lastConst() const noexcept
{
  return getConst(size_ - 1);
}
const dirko::Shape &dirko::Shape_vec::firstConst() const noexcept
{
  return getConst(0);
}
dirko::Shape &dirko::Shape_vec::at(size_t index)
{
  if (index >= size_) {
    throw std::invalid_argument("Out of bounds");
  }
  return *shps_[index];
}
const dirko::Shape &dirko::Shape_vec::atConst(size_t index) const
{
  if (index >= size_) {
    throw std::invalid_argument("Out of bounds");
  }
  return *shps_[index];
}
dirko::Shape &dirko::Shape_vec::get(size_t index) noexcept
{
  return *shps_[index];
}
const dirko::Shape &dirko::Shape_vec::getConst(size_t index) const noexcept
{
  return *shps_[index];
}
void dirko::Shape_vec::remove(size_t index)
{
  if (index >= size_) {
    throw std::invalid_argument("Out of bounds");
  }
  for (size_t i = 0; i < size_; ++i) {
    if (i > index) {
      shps_[i - 1] = shps_[i];
    } else if (i == index) {
      delete shps_[i];
    }
  }
  --size_;
  shps_[size_] = nullptr;
}
void dirko::Shape_vec::dropFirst()
{
  if (size_ == 0) {
    throw std::logic_error("Empty vector");
  }
  remove(0);
}
void dirko::Shape_vec::dropLast()
{
  if (size_ == 0) {
    throw std::logic_error("Empty vector");
  }
  remove(size_ - 1);
}
void dirko::Shape_vec::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    delete shps_[i];
  }
  delete[] shps_;
}
size_t dirko::Shape_vec::size() const noexcept
{
  return size_;
}
bool dirko::Shape_vec::empty() const noexcept
{
  return size_ == 0;
}
size_t dirko::Shape_vec::capasity() const noexcept
{
  return cap_;
}
void dirko::Shape_vec::shrink()
{
  reserve(size_);
}
void dirko::Shape_vec::reserve(size_t newCap)
{
  if (newCap < size_) {
    throw std::logic_error("To small capasity");
  }
  Shape **tmp = new Shape *[newCap]();
  for (size_t i = 0; i < size_; ++i) {
    tmp[i] = shps_[i];
  }
  delete[] shps_;
  shps_ = tmp;
  cap_ = newCap;
}

void dirko::Shape_vec::swap(Shape_vec &other) noexcept
{
  std::swap(size_, other.size_);
  std::swap(cap_, other.cap_);
  std::swap(shps_, other.shps_);
}
