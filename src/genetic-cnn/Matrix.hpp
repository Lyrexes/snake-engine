#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <vector>
#include <boost/contract.hpp>
#include <type_traits>
#include "random.hpp"


using Random = effolkronium::random_static;

template<class T, class A=std::allocator<T> >
class Matrix {
  using value_type     = T;
  using iterator       = value_type*;
  using const_iterator = const value_type*;
  using Container      = std::vector<T, A>;
  static_assert(std::is_arithmetic_v<value_type>);

private:
  std::size_t m_stride{0};
  Container m_data{};

public:
  Matrix()=default;
  Matrix(std::size_t width, std::size_t height) {
    resize(width, height);
  }
  Matrix(const std::vector<T>& vector) 
    :
    m_data(vector),
    m_stride(vector.size())
  {}
  Matrix(const std::vector<T> &&vector) noexcept
    :
    m_data(vector),
    m_stride(vector.size())
  {}
  Matrix(const std::initializer_list<T>& vector)
    :
    m_data(std::move(vector)),
    m_stride(vector.size())
  {}
  Matrix(const std::initializer_list<T>&& vector) noexcept
    :
    m_data(vector),
    m_stride(vector.size())
  {}
  Matrix(const Matrix& other)
    :
    m_data(other.m_data),
    m_stride(other.width)
  {}
  Matrix(const Matrix&& other) noexcept
    :
    m_data(std::move(other.m_data)),
    m_stride(other.width)
  {
    other.m_stride = 0;
    other.m_data.clear();
  }
  Matrix& operator=(const Matrix& other) {
    m_data  = other.m_data;
    m_stride = other.m_width;
    other.m_stride = 0;
    return *this;
  }
  Matrix& operator=(const Matrix&& other) {
    m_data  = std::move(other.m_data);
    m_stride = other.width;
    other.m_stride = 0;
    other.m_data.clear();
    return *this;
  }
  void resize(std::size_t width, std::size_t height) {
    if(width == 0)
      height = 0;
    m_data.resize(width * height);
    m_stride = width;
  }
  std::size_t rows() const noexcept {
    return m_stride ? static_cast<std::size_t>(m_data.size() / m_stride) : 0;
  }
  std::size_t columns() const noexcept {
    return m_stride;
  }
  T& at(int row, int column) const {
    boost::contract::check c = boost::contract::function()
      .precondition([&] {
        BOOST_CONTRACT_ASSERT(row * m_stride < m_data.size());
        BOOST_CONTRACT_ASSERT(column < m_stride);
      })
    ;
    return m_data[row * m_stride + column];
  }
  std::vector<T, A> row(int row) const {
    boost::contract::check c = boost::contract::function()
      .precondition([&] {
        BOOST_CONTRACT_ASSERT(row * m_stride < m_data.size());
      })
    ;
    const auto first = m_data.begin() + row * m_stride;
    const auto last  = first + m_stride;
    return std::vector<T, A>(first, last);
  }
  std::vector<T, A> column(int column) const {
    boost::contract::check c = boost::contract::function()
      .precondition([&] {
        BOOST_CONTRACT_ASSERT(column < m_stride);
      })
    ;
    auto temp = std::vector<T, A>();
    for(int i = 0; i < m_data.size(); i+= m_stride) {
      temp.push_back(m_data[i+column]);
    }
    return temp;
  }
  template<typename Functor>
  void forEach(Functor functor) {
    for(auto &&el : m_data)
      functor(el);
  }
  void fill(T value) {
    for(auto &el : m_data)
      el = value;
  }
  template<class Distribution>
  void fillRandom(T min , T max){
    for(auto &el : m_data) 
      el = Random::get<Distribution>(min, max);
  }
  template<class Distribution>
  void addRandom(T min, T max) {
    for(auto &el : m_data)
      el += Random::get<Distribution>(min, max);
  }
  bool empty() const noexcept {
    return m_data.empty();
  }
  template<class Stream>
  friend Stream& operator<<(Stream& s, Matrix const& value) {
    s << "<Matrix at " << &value << " dimensions "
      << value.rows() << 'x' << value.columns();
    if (!value.empty()) {
      bool first = true;
      typedef typename Container::const_iterator Iter;
      Iter i = value.m_data.begin(), end = value.m_data.end();
      while (i != end) {
        s << (first ? " [" : "] [");
        first = false;
        s << *i;
        ++i;
        for (int b = value.m_stride - 1; b; --b) {
          s  << " " <<*i;
          ++i;
        }
      }
      s << "]";
    }
    s << '>';
    return s;
  }
};





#endif 
