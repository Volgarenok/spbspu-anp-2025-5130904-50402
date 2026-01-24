#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <new>
#include <limits>
#include <iomanip>

namespace pozdnyakov
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  class Shape
  {
  public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t &pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    virtual void scale(double k)
    {
      if (k < 0.0) {
        throw std::invalid_argument("Scaling coefficient must be non-negative");
      }
      doScale(k);
    }

    virtual Shape *clone() const = 0;

    void unsafeScale(double k) noexcept
    {
      doScale(k);
    }

  private:
    virtual void doScale(double k) noexcept = 0;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t &center, double width, double height) noexcept:
      frame_{width, height, center}
    {}

    double getArea() const noexcept override
    {
      return frame_.width * frame_.height;
    }

    rectangle_t getFrameRect() const noexcept override
    {
      return frame_;
    }

    void move(const point_t &pos) noexcept override
    {
      frame_.pos = pos;
    }

    void move(double dx, double dy) noexcept override
    {
      frame_.pos.x += dx;
      frame_.pos.y += dy;
    }

    Shape *clone() const override
    {
      return new Rectangle(*this);
    }

  private:
    rectangle_t frame_;

    void doScale(double k) noexcept override
    {
      frame_.width *= k;
      frame_.height *= k;
    }
  };

  class Diamond final: public Shape
  {
  public:
    Diamond(const point_t &center, double diag_h, double diag_v) noexcept:
      center_(center),
      diag_h_(diag_h),
      diag_v_(diag_v)
    {}

    double getArea() const noexcept override
    {
      return (diag_h_ * diag_v_) / 2.0;
    }

    rectangle_t getFrameRect() const noexcept override
    {
      return {diag_h_, diag_v_, center_};
    }

    void move(const point_t &pos) noexcept override
    {
      center_ = pos;
    }

    void move(double dx, double dy) noexcept override
    {
      center_.x += dx;
      center_.y += dy;
    }

    Shape *clone() const override
    {
      return new Diamond(*this);
    }

  private:
    point_t center_;
    double diag_h_;
    double diag_v_;

    void doScale(double k) noexcept override
    {
      diag_h_ *= k;
      diag_v_ *= k;
    }
  };

  class Triangle final: public Shape
  {
  public:
    Triangle(const point_t &a, const point_t &b, const point_t &c) noexcept:
      p1_(a),
      p2_(b),
      p3_(c)
    {}

    double getArea() const noexcept override
    {
      double term1 = (p2_.x - p1_.x) * (p3_.y - p1_.y);
      double term2 = (p3_.x - p1_.x) * (p2_.y - p1_.y);
      return 0.5 * std::abs(term1 - term2);
    }

    rectangle_t getFrameRect() const noexcept override
    {
      double min_x = std::min({p1_.x, p2_.x, p3_.x});
      double max_x = std::max({p1_.x, p2_.x, p3_.x});
      double min_y = std::min({p1_.y, p2_.y, p3_.y});
      double max_y = std::max({p1_.y, p2_.y, p3_.y});

      return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
    }

    void move(const point_t &pos) noexcept override
    {
      point_t center = getFrameRect().pos;
      double dx = pos.x - center.x;
      double dy = pos.y - center.y;
      move(dx, dy);
    }

    void move(double dx, double dy) noexcept override
    {
      p1_.x += dx;
      p1_.y += dy;
      p2_.x += dx;
      p2_.y += dy;
      p3_.x += dx;
      p3_.y += dy;
    }

    Shape *clone() const override
    {
      return new Triangle(*this);
    }

  private:
    point_t p1_;
    point_t p2_;
    point_t p3_;

    void doScale(double k) noexcept override
    {
      point_t center = getFrameRect().pos;
      p1_.x = center.x + (p1_.x - center.x) * k;
      p1_.y = center.y + (p1_.y - center.y) * k;
      p2_.x = center.x + (p2_.x - center.x) * k;
      p2_.y = center.y + (p2_.y - center.y) * k;
      p3_.x = center.x + (p3_.x - center.x) * k;
      p3_.y = center.y + (p3_.y - center.y) * k;
    }
  };

  class CompositeShape final: public Shape
  {
  public:
    CompositeShape():
      count_(0),
      capacity_(0),
      shapes_(nullptr)
    {}

    CompositeShape(const CompositeShape &other):
      count_(other.count_),
      capacity_(other.count_),
      shapes_(new Shape *[other.count_])
    {
      for (size_t i = 0; i < count_; ++i) {
        shapes_[i] = other.shapes_[i]->clone();
      }
    }

    CompositeShape(CompositeShape &&other) noexcept:
      count_(other.count_),
      capacity_(other.capacity_),
      shapes_(other.shapes_)
    {
      other.count_ = 0;
      other.capacity_ = 0;
      other.shapes_ = nullptr;
    }

    ~CompositeShape() override
    {
      for (size_t i = 0; i < count_; ++i) {
        delete shapes_[i];
      }
      delete[] shapes_;
    }

    CompositeShape &operator=(const CompositeShape &other)
    {
      if (this != &other) {
        CompositeShape temp(other);
        swap(temp);
      }
      return *this;
    }

    CompositeShape &operator=(CompositeShape &&other) noexcept
    {
      if (this != &other) {
        CompositeShape temp(std::move(other));
        swap(temp);
      }
      return *this;
    }

    void addShape(Shape *shape)
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

    void removeShape(size_t index)
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

    double getArea() const noexcept override
    {
      double total = 0.0;
      for (size_t i = 0; i < count_; ++i) {
        total += shapes_[i]->getArea();
      }
      return total;
    }

    rectangle_t getFrameRect() const noexcept override
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

    void move(const point_t &pos) noexcept override
    {
      point_t center = getFrameRect().pos;
      move(pos.x - center.x, pos.y - center.y);
    }

    void move(double dx, double dy) noexcept override
    {
      for (size_t i = 0; i < count_; ++i) {
        shapes_[i]->move(dx, dy);
      }
    }

    void scale(double k) override
    {
      Shape::scale(k);
    }

    Shape *clone() const override
    {
      return new CompositeShape(*this);
    }

    size_t getCount() const noexcept
    {
      return count_;
    }

  private:
    size_t count_;
    size_t capacity_;
    Shape **shapes_;

    void doScale(double k) noexcept override
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

    void swap(CompositeShape &other) noexcept
    {
      std::swap(count_, other.count_);
      std::swap(capacity_, other.capacity_);
      std::swap(shapes_, other.shapes_);
    }
  };

  void printInfo(const Shape &shape, const std::string &name)
  {
    std::cout << name << ":\n";
    std::cout << "  Area: " << shape.getArea() << "\n";
    rectangle_t frame = shape.getFrameRect();
    std::cout << "  Frame: pos(" << frame.pos.x << ", " << frame.pos.y << "), w=" << frame.width
              << ", h=" << frame.height << "\n\n";
  }
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

    composite.scale(2.0);
    printInfo(composite, "Composite scaled x2");

    CompositeShape copy = composite;
    copy.move(100.0, 100.0);

    printInfo(composite, "Original");
    printInfo(copy, "Copy");
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed. " << e.what() << "\n";
    return 2;
  } catch (const std::exception &e) {
    std::cerr << "Error. " << e.what() << "\n";
    return 1;
  }

  return 0;
}
