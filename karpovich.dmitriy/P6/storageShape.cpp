#include "storageShape.hpp"
#include <stdexcept>
karpovich::StorageShape::StorageShape() noexcept:
  shapes_ (nullptr),
  size_(0),
  cap_(0)
{}
karpovich::StorageShape::StorageShape(const StorageShape& other):
  shapes_(nullptr),
  size_(0),
  cap_(other.cap_)
{
  if (cap_ > 0) {
    shapes_ = new Shape*[cap_];
    try {
      for (; size_ < other.size_; ++size_) {
        shapes_[size_] = other.shapes_[size_]->clone();
      }
    } catch (const std::bad_alloc&) {
      clear();
      throw;
    }
  }
}
karpovich::StorageShape& karpovich::StorageShape::operator=(const StorageShape& other) noexcept
{
  if (this != &other) {
    StorageShape temp(other);
    swap(temp);
  }
  return *this;
}
karpovich::StorageShape::StorageShape(StorageShape&& other) noexcept:
  shapes_(other.shapes_),
  size_(other.size_),
  cap_(other.cap_)
{
  other.shapes_ = nullptr;
  other.size_ = 0;
  other.cap_ = 0;
}
karpovich::StorageShape& karpovich::StorageShape::operator=(StorageShape&& other) noexcept
{
  if (this != &other) {
    clear();
    shapes_ = other.shapes_;
    size_ = other.size_;
    cap_ = other.cap_;
    other.shapes_ = nullptr;
    other.size_ = 0;
    other.cap_ = 0;
  }
  return *this;
}
karpovich::Shape* karpovich::StorageShape::clone() const
{
  return new StorageShape(*this);
}
void karpovich::StorageShape::doScale(double k) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->scale(k);
  }
}
double karpovich::StorageShape::getArea() const noexcept
{
  double total = 0;
  for (size_t i = 0; i < size_; ++i) {
    total += shapes_[i]->getArea();
  }
  return total;
}
karpovich::rectangle_t karpovich::StorageShape::getFrameRect() const noexcept
{
  return karpovich::computeTotalFrame(shapes_, size_);
}
void karpovich::StorageShape::move(point_t p) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->move(p);
  }
}
void karpovich::StorageShape::move(double dx, double dy) noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    shapes_[i]->move(dx, dy);
  }
}
void karpovich::StorageShape::append(Shape* app)
{
  add(app, size_);
}
void karpovich::StorageShape::preappend(Shape* app)
{
  add(app, 0);
}
void karpovich::StorageShape::add(Shape* app, size_t idx)
{
  if (cap_ == size_) {
    cap_ = (cap_ == 0) ? 10 : cap_ * 2;
    reserve(cap_);
  }
  if (!empty()) {
    for (size_t i = size_; i > idx; --i) {
      shapes_[i] = shapes_[i - 1];
    }
    shapes_[idx] = app;
    ++size_;
  }
}
karpovich::Shape& karpovich::StorageShape::first() noexcept
{
  return *shapes_[0];
}
karpovich::Shape& karpovich::StorageShape::last() noexcept
{
  return *shapes_[size_ - 1];
}
const karpovich::Shape& karpovich::StorageShape::firstConst() const noexcept
{
  return *shapes_[0];
}
const karpovich::Shape& karpovich::StorageShape::lastConst() const noexcept
{
  return *shapes_[size_ - 1];
}
karpovich::Shape& karpovich::StorageShape::at(size_t idx)
{
  if (idx >= size_) {
    throw std::invalid_argument("out of bounds");
  }
  return *shapes_[idx];
}
karpovich::Shape& karpovich::StorageShape::get(size_t idx) noexcept
{
  return *shapes_[idx];
}
const karpovich::Shape& karpovich::StorageShape::atConst(size_t idx) const
{
  if (idx >= size_) {
    throw std::invalid_argument("out of bounds");
  }
  return *shapes_[idx];
}
const karpovich::Shape& karpovich::StorageShape::getConst(size_t idx) const noexcept
{
  return *shapes_[idx];
}
void karpovich::StorageShape::remove(size_t k)
{
  if (k >= size_) {
    throw std::out_of_range("out of bounds");
  }
  delete shapes_[k];
  for (size_t i = k; i < size_ - 1; ++i) {
    shapes_[i] = shapes_[i + 1];
  }
  --size_;
}
void karpovich::StorageShape::dropFirst()
{
  remove(0);
}
void karpovich::StorageShape::dropLast()
{
  remove(size_ - 1);
}
void karpovich::StorageShape::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i) {
    delete shapes_[i];
  }
  delete[] shapes_;
}
size_t karpovich::StorageShape::size() noexcept
{
  return size_;
}
bool karpovich::StorageShape::empty() noexcept
{
  return !size_;
}
karpovich::StorageShape::~StorageShape()
{
  clear();
}
void karpovich::StorageShape::reserve(size_t k)
{
  if (k < size_) {
    throw std::invalid_argument("too small new capacity");
  }
  Shape** temp = new Shape*[k];
  for (size_t i = 0; i < size_; ++i) {
    temp[i] = shapes_[i];
  }
  delete[] shapes_;
  shapes_ = temp;
  cap_ = k;
}
void karpovich::StorageShape::shrink()
{
  reserve(size_);
}
size_t karpovich::StorageShape::capacity() const noexcept
{
  return cap_;
}
void karpovich::StorageShape::swap(StorageShape& other) noexcept
{
  std::swap(shapes_, other.shapes_);
  std::swap(size_, other.size_);
  std::swap(cap_, other.cap_);
}
