#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

namespace pozdnyakov {
  struct point_t {
    double x;
    double y;
  };

  struct rectangle_t {
    double width;
    double height;
    point_t pos;
  };

  class Shape {
  public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& pos) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void scale(double k) noexcept = 0;
  };

  class Rectangle final : public Shape {
  public:
    Rectangle(const point_t& center, double width, double height);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    rectangle_t frame_;
  };

  class Diamond final : public Shape {
  public:
    Diamond(const point_t& center, double diag_h, double diag_v);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    point_t center_;
    double diag_h_;
    double diag_v_;
  };

  class Triangle final : public Shape {
  public:
    Triangle(const point_t& a, const point_t& b, const point_t& c);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& pos) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    point_t p1_;
    point_t p2_;
    point_t p3_;

    point_t getCenter() const noexcept;
  };

  Rectangle::Rectangle(const point_t& center, double width, double height) :
    frame_{ width, height, center }
  {
  }

  double Rectangle::getArea() const noexcept {
    return frame_.width * frame_.height;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept {
    return frame_;
  }

  void Rectangle::move(const point_t& pos) noexcept {
    frame_.pos = pos;
  }

  void Rectangle::move(double dx, double dy) noexcept {
    frame_.pos.x += dx;
    frame_.pos.y += dy;
  }

  void Rectangle::scale(double k) noexcept {
    frame_.width *= k;
    frame_.height *= k;
  }

  Diamond::Diamond(const point_t& center, double diag_h, double diag_v) :
    center_(center),
    diag_h_(diag_h),
    diag_v_(diag_v)
  {
  }

  double Diamond::getArea() const noexcept {
    return (diag_h_ * diag_v_) / 2.0;
  }

  rectangle_t Diamond::getFrameRect() const noexcept {
    return { diag_h_, diag_v_, center_ };
  }

  void Diamond::move(const point_t& pos) noexcept {
    center_ = pos;
  }

  void Diamond::move(double dx, double dy) noexcept {
    center_.x += dx;
    center_.y += dy;
  }

  void Diamond::scale(double k) noexcept {
    diag_h_ *= k;
    diag_v_ *= k;
  }

  Triangle::Triangle(const point_t& a, const point_t& b, const point_t& c) :
    p1_(a),
    p2_(b),
    p3_(c)
  {
  }

  double Triangle::getArea() const noexcept {
    double term1 = (p2_.x - p1_.x) * (p3_.y - p1_.y);
    double term2 = (p3_.x - p1_.x) * (p2_.y - p1_.y);
    return 0.5 * std::abs(term1 - term2);
  }

  rectangle_t Triangle::getFrameRect() const noexcept {
    double min_x = std::min(p1_.x, std::min(p2_.x, p3_.x));
    double max_x = std::max(p1_.x, std::max(p2_.x, p3_.x));
    double min_y = std::min(p1_.y, std::min(p2_.y, p3_.y));
    double max_y = std::max(p1_.y, std::max(p2_.y, p3_.y));

    double width = max_x - min_x;
    double height = max_y - min_y;

    point_t rect_pos = { min_x + width / 2.0, min_y + height / 2.0 };

    return { width, height, rect_pos };
  }

  void Triangle::move(const point_t& pos) noexcept {
    point_t current_center = getCenter();
    double dx = pos.x - current_center.x;
    double dy = pos.y - current_center.y;
    move(dx, dy);
  }

  void Triangle::move(double dx, double dy) noexcept {
    p1_.x += dx;
    p1_.y += dy;
    p2_.x += dx;
    p2_.y += dy;
    p3_.x += dx;
    p3_.y += dy;
  }

  void Triangle::scale(double k) noexcept {
    point_t center = getCenter();
    p1_.x = center.x + (p1_.x - center.x) * k;
    p1_.y = center.y + (p1_.y - center.y) * k;
    p2_.x = center.x + (p2_.x - center.x) * k;
    p2_.y = center.y + (p2_.y - center.y) * k;
    p3_.x = center.x + (p3_.x - center.x) * k;
    p3_.y = center.y + (p3_.y - center.y) * k;
  }

  point_t Triangle::getCenter() const noexcept {
    return { (p1_.x + p2_.x + p3_.x) / 3.0, (p1_.y + p2_.y + p3_.y) / 3.0 };
  }

  void scaleShapeAtPoint(Shape* shape, const point_t& target, double k) {
    point_t center = shape->getFrameRect().pos;
    double dx = (center.x - target.x) * (k - 1.0);
    double dy = (center.y - target.y) * (k - 1.0);

    shape->move(dx, dy);
    shape->scale(k);
  }

  void clearShapes(Shape** shapes, size_t count) {
    for (size_t i = 0; i < count; ++i) {
      delete shapes[i];
    }
    delete[] shapes;
  }

  void printShapesInfo(const Shape* const* shapes, size_t count) {
    std::cout << std::fixed << std::setprecision(1);

    double totalArea = 0.0;
    double global_min_x = 0.0;
    double global_max_x = 0.0;
    double global_min_y = 0.0;
    double global_max_y = 0.0;
    bool first = true;

    for (size_t i = 0; i < count; ++i) {
      double area = shapes[i]->getArea();
      totalArea += area;
      rectangle_t frame = shapes[i]->getFrameRect();

      std::cout << "Shape " << i
        << ": Area=" << area
        << ", FrameCenter=(" << frame.pos.x << ", " << frame.pos.y << ")"
        << ", w=" << frame.width << ", h=" << frame.height << "\n";

      double half_w = frame.width / 2.0;
      double half_h = frame.height / 2.0;
      double left = frame.pos.x - half_w;
      double right = frame.pos.x + half_w;
      double bottom = frame.pos.y - half_h;
      double top = frame.pos.y + half_h;

      if (first) {
        global_min_x = left;
        global_max_x = right;
        global_min_y = bottom;
        global_max_y = top;
        first = false;
      }
      else {
        global_min_x = std::min(global_min_x, left);
        global_max_x = std::max(global_max_x, right);
        global_min_y = std::min(global_min_y, bottom);
        global_max_y = std::max(global_max_y, top);
      }
    }

    std::cout << "Total Area: " << totalArea << "\n";

    if (!first) {
      double global_w = global_max_x - global_min_x;
      double global_h = global_max_y - global_min_y;
      double global_cx = global_min_x + global_w / 2.0;
      double global_cy = global_min_y + global_h / 2.0;

      std::cout << "Global FrameRect: center=(" << global_cx << ", " << global_cy << ")"
        << ", w=" << global_w << ", h=" << global_h << "\n";
    }
    std::cout << "-----------------------------------------------------------\n";
  }
}

int main() {
  using namespace pozdnyakov;

  const size_t count = 3;
  Shape** shapes = new Shape * [count];

  shapes[0] = new Rectangle({ 5.0, 5.0 }, 10.0, 5.0);
  shapes[1] = new Diamond({ 20.0, 5.0 }, 10.0, 10.0);
  shapes[2] = new Triangle({ 0.0, 0.0 }, { 5.0, 10.0 }, { 10.0, 0.0 });

  std::cout << "--- Before Scaling ---\n";
  printShapesInfo(shapes, count);

  point_t targetPoint;
  std::cout << "Enter scaling center (x y): ";
  if (!(std::cin >> targetPoint.x >> targetPoint.y)) {
    std::cerr << "Invalid coordinates input.\n";
    clearShapes(shapes, count);
    return 1;
  }

  double k;
  std::cout << "Enter scaling coefficient (k >= 0): ";
  if (!(std::cin >> k)) {
    std::cerr << "Invalid input for coefficient.\n";
    clearShapes(shapes, count);
    return 1;
  }

  if (k < 0.0) {
    std::cerr << "Scaling coefficient must be non-negative.\n";
    clearShapes(shapes, count);
    return 1;
  }

  for (size_t i = 0; i < count; ++i) {
    scaleShapeAtPoint(shapes[i], targetPoint, k);
  }

  std::cout << "\n--- After Scaling ---\n";
  printShapesInfo(shapes, count);

  clearShapes(shapes, count);

  return 0;
}
