#include <iostream>
#include <utility>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <algorithm>

struct p_t
{
  int x;
  int y;
};

class Ints
{
public:
  Ints();
  Ints(size_t size, int value);
  Ints(const Ints & rhs);
  Ints(Ints && rhs) noexcept;
  Ints & operator=(const Ints & rhs);
  Ints & operator=(Ints && rhs) noexcept;
  Ints operator+(const Ints & rhs) const;
  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i);
  int get(size_t id) const;
  void setSafe(size_t id, int i);
  int getSafe(size_t id) const;

  Ints append(int i) const;
  Ints append(const Ints & b) const;

  static Ints unitializatedInts(size_t size);

private:
  void resize();
  size_t capacity_;
  size_t size_;
  int * data_;
};

Ints::Ints():
  capacity_(0),
  size_(0),
  data_(nullptr)
{}

Ints::Ints(size_t size, int value):
  capacity_(size),
  size_(size),
  data_(size > 0 ? new int[size] : nullptr)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = value;
  }
}

Ints Ints::unitializatedInts(size_t size)
{
  Ints temp;
  temp.size_ = 0;
  temp.capacity_ = size;
  temp.data_ = size > 0 ? new int[size] : nullptr;
  return temp;
}

Ints::Ints(const Ints & rhs):
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(rhs.capacity_ > 0 ? new int[rhs.capacity_] : nullptr)
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = rhs.data_[i];
  }
}

Ints::Ints(Ints && rhs) noexcept:
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(rhs.data_)
{
  rhs.capacity_ = 0;
  rhs.size_ = 0;
  rhs.data_ = nullptr;
}

Ints & Ints::operator=(const Ints & rhs)
{
  if (this == &rhs)
  {
    return *this;
  }
  Ints temp(rhs);
  std::swap(capacity_, temp.capacity_);
  std::swap(size_, temp.size_);
  std::swap(data_, temp.data_);
  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept
{
  if (this != &rhs)
  {
    delete[] data_;
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    data_ = rhs.data_;

    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.capacity_ = 0;
  }
  return *this;
}

Ints Ints::operator+(const Ints & rhs) const
{
  if (getSize() != rhs.getSize())
  {
    throw std::invalid_argument("Массивы имеют разный размер");
  }

  Ints result(getSize(), 0);
  for (size_t i = 0; i < getSize(); ++i)
  {
    if ((rhs.get(i) > 0 && get(i) > std::numeric_limits<int>::max() - rhs.get(i)) ||
        (rhs.get(i) < 0 && get(i) < std::numeric_limits<int>::min() - rhs.get(i)))
    {
      throw std::overflow_error("Переполнение");
    }
    result.set(i, rhs.get(i) + get(i));
  }
  return result;
}

Ints::~Ints()
{
  delete[] data_;
}

size_t Ints::getSize() const noexcept
{
  return size_;
}

void Ints::set(size_t id, int i)
{
  data_[id] = i;
}

int Ints::get(size_t id) const
{
  return data_[id];
}

void Ints::setSafe(size_t id, int i)
{
  if (id >= size_)
  {
    throw std::out_of_range("Недопустимый индекс");
  }
  set(id, i);
}

int Ints::getSafe(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("Недопустимый индекс");
  }
  return get(id);
}

void Ints::resize()
{
  size_t new_cap = (capacity_ == 0) ? 2 : capacity_ * 2;
  int * new_data = new int[new_cap];
  for (size_t i = 0; i < size_; ++i)
  {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_cap;
}

Ints Ints::append(int i) const
{
  Ints result(*this);
  if (result.capacity_ < result.size_ + 1)
  {
    result.resize();
  }
  result.data_[result.size_] = i;
  ++result.size_;
  return result;
}

Ints Ints::append(const Ints & b) const
{
  Ints result(*this);
  for (size_t i = 0; i < b.getSize(); ++i)
  {
    result = result.append(b.get(i));
  }
  return result;
}

class Points
{
public:
  void set(size_t id, p_t p);
  p_t get(size_t id) const;
  Points append(p_t p) const;
  size_t getCount() const noexcept { return data_.getSize() / 2; }
private:
  Ints data_;
};

void Points::set(size_t id, p_t p)
{
  data_.setSafe(id * 2, p.x);
  data_.setSafe(id * 2 + 1, p.y);
}

p_t Points::get(size_t id) const
{
  return {data_.getSafe(id * 2), data_.getSafe(id * 2 + 1)};
}

Points Points::append(p_t p) const
{
  Points res(*this);
  res.data_ = res.data_.append(p.x).append(p.y);
  return res;
}

class Plot
{
public:
  void set(int x, int y);
  int calc(int x) const;
  size_t solve(Ints & buf, int y) const;
  Points append(p_t p) const;
private:
  Ints data_;
};

void Plot::set(int x, int y)
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i) == x)
    {
      data_.set(i + 1, y);
      return;
    }
  }
  data_ = data_.append(x).append(y);
}

int Plot::calc(int x) const
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i) == x)
    {
      return data_.get(i + 1);
    }
  }
  throw std::logic_error("Координата X не найдена на графике");
}

size_t Plot::solve(Ints & buf, int y) const
{
  size_t cnt = 0;
  for (size_t i = 1; i < data_.getSize(); i += 2)
  {
    if (y == data_.get(i))
    {
      buf = buf.append(data_.get(i - 1));
      ++cnt;
    }
  }
  return cnt;
}

Points Plot::append(p_t p) const
{
  Points pts;
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    pts = pts.append({data_.get(i), data_.get(i + 1)});
  }
  pts = pts.append(p);
  return pts;
}

class Mtx
{
public:
  Mtx();
  Mtx(size_t m, size_t n);
  ~Mtx() = default;

  void set(size_t r, size_t c, int val) { data.setSafe(r * n + c, val); }
  int get(size_t r, size_t c) const { return data.getSafe(r * n + c); }

  Mtx sum(const Mtx & oth) const;
  Mtx mul(const Mtx & oth) const;
  Mtx transpose() const;
  Mtx add_row(int init) const;
  Ints getCol(size_t id) const;
  Ints getRow(size_t id) const;

  void print() const;

private:
  size_t m;
  size_t n;
  Ints data;
};

Mtx::Mtx():
  m(0),
  n(0),
  data()
{}

Mtx::Mtx(size_t m, size_t n):
  m(m),
  n(n),
  data(m * n, 0)
{
  if (n != 0 && m > std::numeric_limits<size_t>::max() / n)
  {
    throw std::logic_error("Переполнение при создании матрицы");
  }
}

Mtx Mtx::sum(const Mtx & oth) const
{
  if (m != oth.m || n != oth.n)
  {
    throw std::invalid_argument("Матрицы имеют разные размеры");
  }
  Mtx res(m, n);
  res.data = data + oth.data;
  return res;
}

Ints Mtx::getCol(size_t id) const
{
  if (id >= n) throw std::out_of_range("Индекс столбца вне диапазона");
  Ints res;
  for (size_t i = 0; i < m; ++i)
  {
    res = res.append(data.get(i * n + id));
  }
  return res;
}

Ints Mtx::getRow(size_t id) const
{
  if (id >= m) throw std::out_of_range("Индекс строки вне диапазона");
  Ints res;
  for (size_t i = 0; i < n; ++i)
  {
    res = res.append(data.get(id * n + i));
  }
  return res;
}

Mtx Mtx::mul(const Mtx & oth) const
{
  if (n != oth.m)
  {
    throw std::invalid_argument("Несовместимые размеры матриц для умножения");
  }
  Mtx res(m, oth.n);
  for (size_t i = 0; i < m; ++i)
  {
    Ints row = getRow(i);
    for (size_t j = 0; j < oth.n; ++j)
    {
      Ints col = oth.getCol(j);
      int sm = 0;
      for (size_t z = 0; z < n; ++z)
      {
        sm += row.get(z) * col.get(z);
      }
      res.data.set(i * oth.n + j, sm);
    }
  }
  return res;
}

Mtx Mtx::transpose() const
{
  Mtx res(n, m);
  for (size_t i = 0; i < m; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      res.set(j, i, get(i, j));
    }
  }
  return res;
}

Mtx Mtx::add_row(int init) const
{
  Mtx res(m + 1, n);
  Ints new_row(n, init);
  res.data = data.append(new_row);
  return res;
}

void Mtx::print() const
{
  for (size_t i = 0; i < m; ++i)
  {
    for (size_t j = 0; j < n; ++j)
    {
      std::cout << get(i, j) << "\t";
    }
    std::cout << "\n";
  }
}

int main()
{
  try
  {
    std::cout << "--- Тест массива чисел ---\n";
    Ints a(5, 10);
    Ints b(5, 20);
    Ints c = a + b;
    std::cout << "Результат сложения первого элемента: " << c.get(0) << "\n\n";

    std::cout << "--- Тест матриц ---\n";
    Mtx m1(2, 3);
    m1.set(0, 0, 1); m1.set(0, 1, 2); m1.set(0, 2, 3);
    m1.set(1, 0, 4); m1.set(1, 1, 5); m1.set(1, 2, 6);

    Mtx m2 = m1.transpose();
    Mtx m3 = m1.mul(m2);
    std::cout << "Результат умножения исходной матрицы на транспонированную:\n";
    m3.print();

    std::cout << "\n--- Тест графика ---\n";
    Plot p;
    p.set(1, 100);
    p.set(2, 200);
    p.set(3, 100);
    
    Ints roots;
    size_t count = p.solve(roots, 100);
    std::cout << "Найдено корней для значения 100: " << count << " (координаты: ";
    for (size_t i = 0; i < roots.getSize(); ++i)
    {
      std::cout << roots.get(i) << (i + 1 < roots.getSize() ? ", " : "");
    }
    std::cout << ")\n";
  }
  catch (const std::exception & e)
  {
    std::cerr << "Ошибка: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
