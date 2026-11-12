#include <iostream>
#include <utility>
#include <cstddef>
#include <limits>
#include <stdexcept>

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
  Ints & operator=(Ints && rhs);
  Ints operator+(const Ints & rhs) const;
  ~Ints();

  size_t getSize() const noexcept;
  void set(size_t id, int i) noexcept;
  int get(size_t id) const noexcept;
  void setSafe(size_t id, int i);
  int getSafe(size_t id) const;

  Ints append(int i);
  Ints append(Ints b);

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
  data_(new int [size])
{
  for (size_t i = 0; i < size_, ++i)
  {
    data_[i] = value;
  }
}

static Ints Ints::unitializatedInts(size_t size)
{
  Ints temp;
  temp.size_ = 0;
  temp.capacity_ = size;
  temp.data_ = new int [size_];

  return temp;
}

Ints::Ints(const Ints & rhs):
  capacity_(rhs.capacity_),
  size_(rhs.size_),
  data_(new int [size_])
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

  std::swap(size_, temp.size)
  std::swap(data_, temp.data)

  return *this;
}

Ints & Ints::operator=(Ints && rhs) noexcept
{
  if (this != &rhs)
  {
    delete [] data_;

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
    throw std::invalid_argument("Arrays have different size");
  }

  Ints result(getSize(), 0)
  for (size_t i = 0; i < getSize(); ++i)
  {
    if (rhs.get(i) > std::numeric_limits< int >::max() - get(i)
        || rhs.get(i) < std::numeric_limits< int >::min() + get(i))
      {
        throw std::invalid_argument("overflow");
      }

    result.set(i, rhs.get(i) + get(i));
  }

  return result;
}

Ints::~Ints()
{
  delete [] data_;
}

size_t Ints::getSize() const noexcept
{
  return size_;
}

void Ints::set(size_t id, int i)
{
  data_[id] = i;
}

int Ints::get(size_t id) noexcept const
{
  return data_[id];
}

void Ints::setSafe(size_t id, int i)
{
  if (id > size_)
  {
    throw std::logic_error("Invalid value");
  }

  set(id, i);
}

int Ints::getSafe(size_t id) const
{
  if (id > size_)
  {
    throw std::logic_error("Invalid value");
  }

  return get(id);
}


Ints Ints::append(int i)
{
  Ints result(*this);

  if (result.capacity_ < result.size_ + 1)
  {
    result.resize();
  }

  result.data_[size_] = i;
  ++result.size_;

  return result;
}

void Ints::resize()
{
  size_t new_cap = capacity_ == 0 ? 2 : capacity_ * 2;
  int * new_data = new int[new_cap];
  for (size_t i = 0; i < size_; ++i)
  {
    new_data[i] = data_[i];
  }
  delete[] data_;
  data_ = new_data;
  capacity_ = new_cap;
}

Ints Ints::append(Ints b)
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
  int get(size_t id);
  Points append(p_t p);
private:
  Ints data;
};

void Points::set(size_t id, p_t p)
{
  data.set(id * 2, p.x);
  data.set(id * 2 + 1, p.y);
}

int Points::get(size_t id)
{
  return data.get(id);
}

Points Points::append(p_t p)
{
  Points res(*this);
  res.data = res.data.append(p.x).append(p.y);
  return res;
}

class Plot
{
public:
  void set(int x, int y);
  int calc(int x);
  size_t solve(Ints & buf, int y);
  Points append(p_t p);
private:
  Ints data_;
};

size_t Plot::solve(Ints & buf, int y)
{
  size_t cnt = 0;
  for (size_t i = 1; i < data_.getSize(); i+=2)
  {
    if (y == data_.get(i))
    {
      buf.append(data_.get(i - 1));
      ++cnt;
    }
  }

  return cnt;
}

void Plot::set(int x, int y)
{
  data_ = data_.append(x).append(y);
}

int Plot::calc(int x)
{
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    if (data_.get(i) == x)
    {
      return data_.get(i + 1);
    }
  }
  return 0;
}

Points Plot::append(p_t p)
{
  Points res;
  for (size_t i = 0; i < data_.getSize(); i += 2)
  {
    res = res.append({data_.get(i), data_.get(i + 1)});
  }
  res = res.append(p);
  return res;
}

class Mtx
{
public:
  Mtx();
  Mtx(size_t m, size_t n);
  ~Mtx() = default;
  Mtx sum(const Mtx & oth) const;
  Ints getCol(size_t id);
  Ints getRow(size_t id);
private:
  size_t m;
  size_t n;
  Ints data;
};

Mtx::Mtx():
  m(0),
  n(0),
{}

Mtx::Mtx(size_t m, size_t n):
  m(m),
  n(n),
{
  if (m > std::numeric_limits< size_t >::max() / n)
  {
    throw std::logic_error("Owerflow");
  }

  data(n * m, 0);
}

Mtx Mtx::sum(const Mtx & oth) const
{
  Mtx res(m, n);
  for (size_t i = 0; i < data.getSize(); ++i)
  {
    if (oth.data.get(i) > std::numeric_limits< size_t >::max() - data.get(i))
    {
      throw std::logic_error("Owerflow");
    }

    res.data.set(i, oth.data.get(i) + data.get(i));
  }

  return res;
}

Ints Mtx::getCol(size_t id)
{
  Ints res(m, 0);
  for (size_t i = 0; i < m; ++i)
  {
    res.append(i * n + id);
  }

  return res;
}

Ints Mtx::getRow(size_t id)
{
  Ints res(n, 0);
  for (size_t i = 0; i < n; ++i)
  {
    res.append(id * m + i);
  }

  return res;
}

Mtx Mtx::mul(const Mtx & oth) const;
{
  //mn np = mp
  //m1n1 m2n2 = m1n2
  Mtx res(m, oth.n);
  int sm = 0;
  for (size_t i = 0; i < m; ++i)
  {
    for (size_t j = 0; j < oth.n; ++j)
    {
      Ints row = getRow(i);
      Ints col = oth.getCol(j);

      sm = 0;
      for (size_t z = 0; z < m; z++)
      {
        sm += row[z] * col[z]
      }

      res.data.set(i * n + j, sm);
    }
  }

  return res;
}

int main()
{
  try
  {
    std::cout << "Запуск программы Занятие 2_11\n";
    Ints a(10, 67);
    Ints b(10, 69);
    Ints c = a + b;
    std::cout << "Первый элемент суммы массивов: " << c.get(0) << "\n";

    Mtx m1(2, 2);
    Mtx m2(2, 2);
    Mtx m3 = m1.sum(m2);
    std::cout << "Сложение матриц выполнено\n";

    Plot p;
    p.set(1, 5);
    Ints roots;
    size_t cnt = p.solve(roots, 5);
    std::cout << "Найдено корней на графике: " << cnt << "\n";
  }
  catch (const std::exception & e)
  {
    std::cout << "Произошла ошибка: " << e.what() << "\n";
  }
  return 0;
}
