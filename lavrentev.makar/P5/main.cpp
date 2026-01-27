#include <cmath>
#include <iostream>
#include <new>
#include <stdexcept>

namespace lavrentev {
  const size_t n = 3;

  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  class Shape {
  public:
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(point_t c) noexcept = 0;
    virtual void move(double d_x, double d_y) noexcept = 0;

    void scaleWithCheck(double coef);
    virtual void figureScaling(double coef) noexcept = 0;
  };

  class Rectangle final: public Shape {
  public:
    explicit Rectangle(point_t pos, double width, double height);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t c) noexcept override;
    void move(double d_x, double d_y) noexcept override;

    void figureScaling(double coef) noexcept override;

  private:
    point_t pos_;
    double width_, height_;
  };

  class Rubber final: public Shape {
  public:
    explicit Rubber(point_t pos, point_t outCenter, double rPos, double rOut);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t c) noexcept override;
    void move(double d_x, double d_y) noexcept override;

    void figureScaling(double coef) noexcept override;

  private:
    point_t pos_;
    point_t outCenter_;
    double rPos_;
    double rOut_;
  };

  class Polygon final: public Shape {
  public:
    explicit Polygon(const point_t *vertexes, size_t n);
    ~Polygon() noexcept;

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(point_t c) noexcept override;
    void move(double d_x, double d_y) noexcept override;

    void figureScaling(double coef) noexcept override;

  private:
    point_t pos_;
    size_t n_;
    point_t *vertexes_;

    point_t* copyAndValidateVertexes(const point_t *vertexes, size_t n);
    point_t calculateCenter(size_t n, const point_t *vertexes);
  };

  int polyPos(point_t &pos, size_t n, const point_t *vertexes) noexcept;
  rectangle_t fullFrame(const Shape *const *figures, size_t n) noexcept;
  void userShape(Shape **figures, point_t user_dot, double coef, size_t n);
  void printInfo(const Shape *const *figures, const size_t n) noexcept;
  void printRect(rectangle_t fig) noexcept;
}

int main()
{
  const size_t k = 7;
  const lavrentev::point_t vrtxs[] = {{1.2, 5.6}, {3.3, -4.7},
      {1.1, 9.3}, {-5.5, -3.0},
      {-7.3, -0.3}, {-2.1, 4.8}, {3.6, 8.3}};

  lavrentev::Shape *figures[lavrentev::n] = {nullptr, nullptr, nullptr};
  try {
    figures[0] = new lavrentev::Rectangle({3, 3}, 8, 5);
    figures[1] = new lavrentev::Rubber({-7, -2}, {-5, 0}, 3.5, 9);
    figures[2] = new lavrentev::Polygon(vrtxs, k);
  } catch (const std::exception&) {
    for (size_t i = 0; i < lavrentev::n; ++i) {
      delete figures[i];
    }
    return 1;
  }

  lavrentev::printInfo(figures, lavrentev::n);

  double x, y, coef;
  std::cin >> x >> y >> coef;
  if (std::cin.fail()) {
    std::cerr << "Invalid input" << '\n';
    for (size_t i = 0; i < lavrentev::n; ++i) {
      delete figures[i];
    }
    return 1;
  }

  try {
    const lavrentev::point_t user_dot = {x, y};
    lavrentev::userShape(figures, user_dot, coef, lavrentev::n);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    for (size_t i = 0; i < lavrentev::n; ++i) {
      delete figures[i];
    }
    return 2;
  }

  std::cout << "Новые данные: " << "\n\n";
  lavrentev::printInfo(figures, lavrentev::n);

  for (size_t i = 0; i < lavrentev::n; ++i) {
    delete figures[i];
  }
  return 0;
}

void lavrentev::Shape::scaleWithCheck(double coef)
{
  if (coef <= 0) {
    throw std::invalid_argument("Coef must be positive");
  }
  figureScaling(coef);
}

lavrentev::Rectangle::Rectangle(point_t pos, double width, double height):
  pos_(pos),
  width_(width),
  height_(height)
{
  if (width_ <= 0 || height_ <= 0) {
    throw std::invalid_argument("Invalid weight or height");
  }
}

double lavrentev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

lavrentev::rectangle_t lavrentev::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, pos_};
}

void lavrentev::Rectangle::move(point_t c) noexcept
{
  pos_ = c;
}

void lavrentev::Rectangle::move(double d_x, double d_y) noexcept
{
  pos_.x += d_x;
  pos_.y += d_y;
}

void lavrentev::Rectangle::figureScaling(double coef) noexcept
{
  width_ *= coef;
  height_ *= coef;
}

lavrentev::Rubber::Rubber(point_t pos, point_t outCenter, double rPos, double rOut):
  pos_(pos),
  outCenter_(outCenter),
  rPos_(rPos),
  rOut_(rOut)
{
  if (rOut_ <= 0 || rPos_ <= 0) {
    throw std::invalid_argument("Invalid value of radius");
  }
}

double lavrentev::Rubber::getArea() const noexcept
{
  return M_PI * rOut_ * rOut_ - M_PI * rPos_ * rPos_;
}

lavrentev::rectangle_t lavrentev::Rubber::getFrameRect() const noexcept
{
  return {rOut_ * 2, rOut_ * 2, outCenter_};
}

void lavrentev::Rubber::move(point_t c) noexcept
{
  pos_ = c;
}

void lavrentev::Rubber::move(double d_x, double d_y) noexcept
{
  pos_.x += d_x;
  pos_.y += d_y;
}

void lavrentev::Rubber::figureScaling(double coef) noexcept
{
  rPos_ *= coef;
  rOut_ *= coef;
}

lavrentev::point_t* lavrentev::Polygon::copyAndValidateVertexes(const point_t *vertexes, size_t n)
{
  if (n <= 2) {
    throw std::invalid_argument("Invalid amount of vertexes");
  }

  if (vertexes == nullptr) {
    throw std::invalid_argument("Vertexes array is null");
  }

  point_t* copy = new point_t[n];
  for (size_t i = 0; i < n; ++i) {
    copy[i] = vertexes[i];
  }

  return copy;
}

lavrentev::point_t lavrentev::Polygon::calculateCenter(size_t n, const point_t *vertexes)
{
  point_t pos = {};
  int k = polyPos(pos, n, vertexes);
  if (k == 1) {
    throw std::logic_error("Failed to calculate polygon center");
  }
  return pos;
}

lavrentev::Polygon::Polygon(const point_t *vertexes, size_t n):
  pos_(calculateCenter(n, vertexes)),
  n_(n),
  vertexes_(copyAndValidateVertexes(vertexes, n))
{}

lavrentev::Polygon::~Polygon() noexcept
{
  delete[] vertexes_;
}

double lavrentev::Polygon::getArea() const noexcept
{
  double buf1 = 0.0, buf2 = 0.0;
  for (size_t i = 0; i < n_ - 1; ++i) {
    buf1 += (vertexes_[i].x * vertexes_[i + 1].y);
    buf2 += (vertexes_[i].y * vertexes_[i + 1].x);
  }
  buf1 += vertexes_[n_ - 1].x * vertexes_[0].y;
  buf2 += vertexes_[n_ - 1].y * vertexes_[0].x;
  return 0.5 * std::abs(buf1 - buf2);
}

lavrentev::rectangle_t lavrentev::Polygon::getFrameRect() const noexcept
{
  point_t l_u = vertexes_[0], r_b = vertexes_[0];
  for (size_t i = 0; i < n_; ++i) {
    l_u.x = std::min(vertexes_[i].x, l_u.x);
    l_u.y = std::max(vertexes_[i].y, l_u.y);
    r_b.x = std::max(vertexes_[i].x, r_b.x);
    r_b.y = std::min(vertexes_[i].y, r_b.y);
  }
  return {r_b.x - l_u.x, l_u.y - r_b.y, {(r_b.x + l_u.x) * 0.5, (r_b.y + l_u.y) * 0.5}};
}

void lavrentev::Polygon::move(point_t c) noexcept
{
  double dx = c.x - pos_.x;
  double dy = c.y - pos_.y;

  move(dx, dy);
}

void lavrentev::Polygon::move(double d_x, double d_y) noexcept
{
  pos_.x += d_x;
  pos_.y += d_y;

  for (size_t i = 0; i < n_; ++i) {
    vertexes_[i].x += d_x;
    vertexes_[i].y += d_y;
  }
}

void lavrentev::Polygon::figureScaling(double coef) noexcept
{
  for (size_t i = 0; i < n_; ++i) {
    double d_x = vertexes_[i].x - pos_.x;
    double d_y = vertexes_[i].y - pos_.y;
    d_x *= coef;
    d_y *= coef;
    vertexes_[i].x = pos_.x + d_x;
    vertexes_[i].y = pos_.y + d_y;
  }
}

int lavrentev::polyPos(point_t &pos, size_t n, const point_t *vertexes) noexcept
{
  if (n < 3) {
    return 1;
  }

  double square = 0.0;
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    square += (vertexes[i].x * vertexes[j].y) - (vertexes[j].x * vertexes[i].y);
  }
  square *= 0.5;

  if (square != 0.0) {
    double c_x = 0.0;
    for (size_t i = 0; i < n; ++i) {
      size_t j = (i + 1) % n;
      double buf_1 = vertexes[i].x + vertexes[j].x;
      double buf_2 = vertexes[i].x * vertexes[j].y;
      double buf_3 = vertexes[j].x * vertexes[i].y;
      c_x += buf_1 * (buf_2 - buf_3);
    }
    c_x /= (6 * square);

    double c_y = 0.0;
    for (size_t i = 0; i < n; ++i) {
      size_t j = (i + 1) % n;
      double buf_1 = vertexes[i].y + vertexes[j].y;
      double buf_2 = vertexes[i].x * vertexes[j].y;
      double buf_3 = vertexes[j].x * vertexes[i].y;
      c_y += buf_1 * (buf_2 - buf_3);
    }
    c_y /= (6 * square);
    pos.x = c_x;
    pos.y = c_y;
  } else {
    return 1;
  }
  return 0;
}

lavrentev::rectangle_t lavrentev::fullFrame(const Shape *const *figures, size_t n) noexcept
{
  double left = 0.0, right = 0.0, up = 0.0, down = 0.0;
  for (size_t i = 0; i < n; ++i) {
    rectangle_t buf = figures[i]->getFrameRect();
    left = std::min(buf.pos.x - buf.width * 0.5, left);
    right = std::max(buf.pos.x + buf.width * 0.5, right);
    down = std::min(buf.pos.y - buf.height * 0.5, down);
    up = std::max(buf.pos.y + buf.height * 0.5, up);
  }
  rectangle_t ff;
  ff.height = up - down;
  ff.width = right - left;
  ff.pos.y = (up + down) * 0.5;
  ff.pos.x = (right + left) * 0.5;
  return ff;
}

void lavrentev::userShape(Shape **figures, point_t user_dot, double coef, size_t n)
{
  if (coef <= 0) {
    throw std::invalid_argument("Coef must be positive");
  }

  for (size_t i = 0; i < n; ++i) {
    point_t point1 = figures[i]->getFrameRect().pos;
    figures[i]->move(user_dot);
    point_t delta = {user_dot.x - point1.x, user_dot.y - point1.y};
    figures[i]->figureScaling(coef);
    point_t res = {user_dot.x - delta.x * coef, user_dot.y - delta.y * coef};
    figures[i]->move(res);
  }
}

void lavrentev::printInfo(const Shape *const *figures, const size_t n) noexcept
{
  double sum = 0;
  for (size_t i = 0; i < n; ++i) {
    std::cout << "Площадь (" << i + 1 << "):" << figures[i]->getArea() << '\n';
    sum += figures[i]->getArea();
  }

  std::cout << "Суммарная площадь: " << sum;
  std::cout << "\n\n";

  for (size_t i = 0; i < n; ++i) {
    std::cout << "Ограничивающий прямоугольник (" << i + 1 << "):" << '\n';
    printRect(figures[i]->getFrameRect());
  }

  std::cout << "Общий ограничивающий прямоугольник:" << '\n';
  printRect(fullFrame(figures, n));
}

void lavrentev::printRect(rectangle_t fig) noexcept
{
  std::cout << '\t' << "Центр: {" << fig.pos.x << ", " << fig.pos.y << '}' << '\n';
  std::cout << '\t' << "Длина: " << fig.width << '\n';
  std::cout << '\t' << "Высота: " << fig.height << '\n';
}
