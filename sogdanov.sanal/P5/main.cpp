#include <iostream>
namespace sogdanov
{
  struct point_t {
    double x;
    double y;
  };
  struct rectangle_t {
    double width;
    double height;
    point_t pos;
  };
  struct Shape {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(const point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };
  struct Rectangle final : Shape {
    Rectangle(double width, double height, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    double width_;
    double height_;
    point_t pos_;
  };
  struct Xquare final : Shape {
    Xquare(double d, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(const point_t p) override;
    void scale(double k) override;
  private:
    double d_;
    point_t pos_;
  };
  struct Diamond final : Shape {
    Diamond(double diagx, double diagy, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(const point_t p) override;
    void scale(double k) override;
  private:
    double diagx_;
    double diagy_;
    point_t pos_;
  };
  void scaleFromPoint(Shape * shps[], double k, size_t size, point_t pt);
  double leftX(const rectangle_t& r);
  double rightX(const rectangle_t& r);
  double botY(const rectangle_t& r);
  double topY(const rectangle_t& r);
  rectangle_t totalFrame(Shape * shps[], size_t size);
  void output(Shape * shps[], size_t size);
}
int main()
{
  sogdanov::Rectangle rect(2.0, 8.0, {-3.0, 4.0});
  sogdanov::Xquare xq(5.0, {1.0, 3.0});
  sogdanov::Diamond dm(4.0, 8.0, {-2.0, 2.0});
  const size_t N = 3;
  sogdanov::Shape * shps[N] = {&rect, &xq, &dm};
  sogdanov::output(shps, N);
  sogdanov::point_t pt{};
  double k = 0.0;
  if (!(std::cin >> pt.x >> pt.y >> k)) {
    std::cerr << "bad arg\n";
    return 1;
  }
  sogdanov::scaleFromPoint(shps, k, N, pt);
  sogdanov::output(shps, N);
}
sogdanov::Rectangle::Rectangle(double width, double height, point_t p):
  sogdanov::Shape(), width(width_), height(height_), p(pos_)
{}
double sogdanov::Rectangle::getArea() const
{
  return width_ * height_;
}
sogdanov::rectangle_t sogdanov::Rectangle::getFrameRect() const
{
  return sogdanov::rectangle_t{width_, height_, pos_};
}
void sogdanov::Rectangle::move(const point_t p)
{
  pos_ = p;
}
void sogdanov::Rectangle::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}
void sogdanov::Rectangle::scale(double k)
{
  width_ *= k;
  height_ *= k;
}
sogdanov::Xquare::Xquare(double d, point_t p):
  d_(d), pos_(p)
{}
double sogdanov::Xquare::getArea() const
{
  return (d_ * d_) / 2.0;
}
sogdanov::rectangle_t sogdanov::Xquare::getFrameRect() const
{
  return sogdanov::rectangle_t{d_, d_, pos_};
}
void sogdanov::Xquare::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}
void sogdanov::Xquare::move(point_t p)
{
  pos_ = p;
}
void sogdanov::Xquare::scale(double k)
{
  d_ *= k;
}
sogdanov::Diamond::Diamond(double diagx, double diagy, point_t p):
  diagx_(diagx), diagy_(diagy), pos_(p);
{}
double sogdanov::Diamond::getArea() const
{
  return (diagx_ * diagy_) / 2.0;
}
sogdanov::rectangle_t sogdanov::Diamond::getFrameRect() const
{
  return sogdanov::rectangle_t{diagx_, diagy_, pos_};
}
void sogdanov::Diamond::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}
void sogdanov::Diamond::move(const point_t p)
{
  pos_ = p;
}
void sogdanov::Diamond::scale(double k)
{
  diagx *= k;
  diagy *= k;
}
void sogdanov::scaleFromPoint(Shape * shps[], double k, size_t size, point_t pt)
{
  for (size_t i = 0; i < size; ++i) {
    shps[i]->move(-pt.x, -pt.y);
    shps[i]->scale(k);
    shps[i]->move(pt.x, pt.y);
  }
}
double sogdanov::leftX(const rectangle_t& r)
{
  return r.pos.x - r.width / 2.0;
}
double sogdanov::rightX(const rectangle_t& r)
{
  return r.pos.x + r.width / 2.0;
}
double sogdanov::botY(const rectangle_t& r)
{
  return r.pos.y - r.height / 2.0;
}
double sogdanov::topY(const rectangle_t& r)
{
  return r.pos.y + r.height / 2.0;
}
sogdanov::rectangle_t sogdanov::totalFrame(Shape * shps[], size_t size)
{
  sogdanov::rectangle_t cur = shapes[0]->getFrameRect();
  double left = leftX(cur);
  double right = rightX(cur);
  double bottom = botY(cur);
  double top = topY(cur);
  for (size_t i = 1; i < size; ++i) {
    sogdanov::rectangle_t r = shapes[i]->getFrameRect();
    left = std::min(left, leftX(r));
    right = std::max(right, rightX(r));
    bottom = std::min(bottom, botY(r));
    top = std::max(top, topY(r));
  }
  sogdanov::rectangle_t total;
  total.width = right - left;
  total.height = top - bottom;
  total.pos.x = (left + right) / 2.0;
  total.pos.y = (bottom + top) / 2.0;
  return total;
}
void sogdanov::output(Shape * shps[], size_t size)
{
  double total_area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    double area = shapes[i]->getArea();
    total_area += area;
    std::cout << area << "\n";
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << frame.pos.x << "\n";
    std::cout << frame.pos.y << "\n";
    std::cout << frame.width << "\n";
    std::cout << frame.height << "\n";
  }
  rectangle_t total_frame = TotalFrame(shapes, size);
  std::cout << total_area << "\n";
  std::cout << total_frame.pos.x << "\n";
  std::cout << total_frame.pos.y << "\n";
  std::cout << total_frame.width << "\n";
  std::cout << total_frame.height << "\n";
}
