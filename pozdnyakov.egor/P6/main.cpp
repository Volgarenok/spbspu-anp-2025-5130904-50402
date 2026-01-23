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
    virtual void move(const point_t& pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;

    virtual void scale(double k)
    {
      if (k < 0.0) {
        throw std::invalid_argument("Scaling coefficient must be non-negative");
      }
      doScale(k);
    }
    virtual Shape* clone() const = 0;

    void unsafeScale(double k) noexcept
    {
      doScale(k);
    }

  private:
    virtual void doScale(double k) noexcept = 0;
  };

  class Rectangle final : public Shape
  {
  public:
    Rectangle(const point_t& center, double width, double height) noexcept :
      frame_{ width, height, center }
    {
    }

    double getArea() const noexcept override
    {
      return frame_.width * frame_.height;
    }

    rectangle_t getFrameRect() const noexcept override
    {
      return frame_;
    }

    void move(const point_t& pos) noexcept override
    {
      frame_.pos = pos;
    }

    void move(double dx, double dy) noexcept override
    {
      frame_.pos.x += dx;
      frame_.pos.y += dy;
    }

    Shape* clone() const override
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

  class Diamond final : public Shape
  {
  public:
    Diamond(const point_t& center, double diag_h, double diag_v) noexcept :
      center_(center),
      diag_h_(diag_h),
      diag_v_(diag_v)
    {
    }

    double getArea() const noexcept override
    {
      return (diag_h_ * diag_v_) / 2.0;
    }

    rectangle_t getFrameRect() const noexcept override
    {
      return { diag_h_, diag_v_, center_ };
    }

    void move(const point_t& pos) noexcept override
    {
      center_ = pos;
    }

    void move(double dx, double dy) noexcept override
    {
      center_.x += dx;
      center_.y += dy;
    }

    Shape* clone() const override
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

  class Triangle final : public Shape
  {
  public:
    Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept :
      p1_(a), p2_(b), p3_(c)
    {
    }

    double getArea() const noexcept override
    {
      double term1 = (p2_.x - p1_.x) * (p3_.y - p1_.y);
      double term2 = (p3_.x - p1_.x) * (p2_.y - p1_.y);
      return 0.5 * std::abs(term1 - term2);
    }

    rectangle_t getFrameRect() const noexcept override
    {
      double min_x = std::min({ p1_.x, p2_.x, p3_.x });
      double max_x = std::max({ p1_.x, p2_.x, p3_.x });
      double min_y = std::min({ p1_.y, p2_.y, p3_.y });
      double max_y = std::max({ p1_.y, p2_.y, p3_.y });

      return { max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0} };
    }

    void move(const point_t& pos) noexcept override
    {
      point_t center = getFrameRect().pos;
      double dx = pos.x - center.x;
      double dy = pos.y - center.y;
      move(dx, dy);
    }

    void move(double dx, double dy) noexcept override
    {
      p1_.x += dx; p1_.y += dy;
      p2_.x += dx; p2_.y += dy;
      p3_.x += dx; p3_.y += dy;
    }

    Shape* clone() const override
    {
      return new Triangle(*this);
    }

  private:
    point_t p1_, p2_, p3_;

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
}
