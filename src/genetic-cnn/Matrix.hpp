#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "random.hpp"
#include <boost/contract.hpp>
#include <gsl/gsl_assert>
#include <type_traits>
#include <vector>

using Random = effolkronium::random_static;
namespace ct = boost::contract;

template<class T, class A = std::allocator<T>>
class Matrix {
  using value_type = T;
  using Container = std::vector<T, A>;
  static_assert(std::is_arithmetic_v<value_type>);

private:
  std::size_t m_stride{0};
  Container m_data{};

public:
  Matrix() = default;
  Matrix(std::size_t rows, std::size_t columns) {
	resize(rows, columns);
  }
  explicit Matrix(std::vector<T> vector) noexcept
	  : m_data(std::move(vector)),
		m_stride(1) {}
  Matrix(std::initializer_list<T> vector) noexcept
	  : m_data(std::move(vector)),
		m_stride(1) {}
  Matrix(const Matrix &other)
	  : m_data(other.m_data),
		m_stride(other.m_stride) {}
  Matrix(Matrix &&other) noexcept
	  : m_data(std::move(other.m_data)),
		m_stride(other.m_stride) {
	other.m_stride = 0;
	other.m_data.clear();
  }
  Matrix &operator=(const Matrix &other) {
	m_data = other.m_data;
	m_stride = other.m_stride;
	return *this;
  }
  Matrix &operator=(Matrix &&other) noexcept {
	m_data = std::move(other.m_data);
	m_stride = other.m_stride;
	other.m_stride = 0;
	other.m_data.clear();
	return *this;
  }
  void resize(std::size_t rows, std::size_t columns) {
	if (rows == 0)
	  columns = 0;
	m_data.resize(rows * columns);
	m_stride = columns;
  }
  constexpr std::size_t rows() const noexcept {
	return m_stride ? static_cast<std::size_t>(m_data.size() / m_stride) : 0;
  }
  constexpr std::size_t columns() const noexcept {
	return m_stride;
  }
  T &at(int row, int column) {
	ct::check c = boost::contract::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(row * m_stride < m_data.size());
						BOOST_CONTRACT_ASSERT(column < m_stride);
					  });
	return m_data[row * m_stride + column];
  }
  T at(int row, int column) const {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(row * m_stride < m_data.size());
						BOOST_CONTRACT_ASSERT(column < m_stride);
					  });
	return m_data[row * m_stride + column];
  }
  std::vector<T, A> row(int row) const {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(row * m_stride < m_data.size());
					  });
	const auto first = m_data.begin() + row * m_stride;
	const auto last = first + m_stride;
	return std::vector<T, A>(first, last);
  }
  std::vector<T, A> column(int column) const {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(column < m_stride);
					  });
	auto temp = std::vector<T, A>();
	for (int i = 0; i < m_data.size(); i += m_stride) {
	  temp.push_back(m_data[i + column]);
	}
	return temp;
  }
  template<typename Functor>
  void forEach(const Functor &functor) {
	for (auto &&el : m_data)
	  functor(el);
  }
  void fill(T value) {
	for (auto &el : m_data)
	  el = value;
  }
  template<typename Distribution,
		   std::enable_if_t<!std::is_void_v<Distribution>, int> = 0>
  void fillRandom(T min, T max) {
	for (auto &el : m_data) {
	  el = Random::get<Distribution>(min, max);
	}
  }
  template<typename Distribution,
		   std::enable_if_t<std::is_void_v<Distribution>, int> = 0>
  void fillRandom(T min, T max) {
	for (auto &el : m_data) {
	  el = Random::get<>(min, max);
	}
  }
  template<class Distribution,
		   std::enable_if_t<!std::is_void_v<Distribution>, int> = 0>
  void addRandom(T min, T max) {
	for (auto &el : m_data)
	  el += Random::get<Distribution>(min, max);
  }
  template<class Distribution,
		   std::enable_if_t<std::is_void_v<Distribution>, int> = 0>
  void addRandom(T min, T max) {
	for (auto &el : m_data)
	  el += Random::get<>(min, max);
  }
  bool empty() const noexcept {
	return m_data.empty();
  }
  std::size_t rawSize() const {
	return m_data.size();
  }
  template<class Stream>
  friend Stream &operator<<(Stream &s, Matrix const &value) {
	s << "<Matrix at " << &value << " dimensions "
	  << value.rows() << 'x' << value.columns() << '>'
	  << std::endl
	  << '[' << ' ';
	for (int i = 0; i < value.rawSize(); ++i) {
	  s << value.m_data[i] << std::scientific << ' ';
	  if (i != 0 && (i + 1) % value.columns() == 0) {
		s << ']' << std::endl;
		if (i + 1 < value.rawSize())
		  s << '[' << ' ';
	  }
	}
	return s;
  }
  Matrix multiply(const Matrix &other) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(columns() == other.rows());
					  });
	Matrix result(rows(), other.columns());
	for (int i = 0; i < this->rows(); ++i) {
	  for (int j = 0; j < other.columns(); ++j) {
		result.at(i, j) = 0;
		for (int k = 0; k < this->columns(); k++) {
		  result.at(i, j) += this->at(i, k) * other.at(k, j);
		}
	  }
	}
	return result;
  }
  Matrix operator*(const Matrix &other) {
	ct::check c = ct::function()
					  .precondition([&] {
						std::cout << columns() << " x " << other.rows() << std::endl;
						BOOST_CONTRACT_ASSERT(this->columns() == other.rows());
					  });
	Matrix result(this->rows(), other.columns());
	for (int i = 0; i < this->rows(); ++i) {
	  for (int j = 0; j < other.columns(); ++j) {
		result.at(i, j) = 0;
		for (int k = 0; k < this->columns(); k++) {
		  result.at(i, j) += this->at(i, k) * other.at(k, j);
		}
	  }
	}
	return result;
  }
  void operator*=(const Matrix &other) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(this->columns() == other.rows());
					  });
	Matrix result(rows(), other.columns());
	for (int i = 0; i < this->rows(); ++i) {
	  for (int j = 0; j < other.columns(); ++j) {
		result.at(i, j) = 0;
		for (int k = 0; k < this->columns(); k++) {
		  result.at(i, j) += this->at(i, k) * other.at(k, j);
		}
	  }
	}
	*this = std::move(result);
  }
  Matrix transpose() {
	Matrix transposed(columns(), rows());
	for (std::size_t row = 0; row < rows(); ++row) {
	  for (std::size_t column = 0; column < columns(); ++column) {
		transposed.at(column, row) = this->at(row, column);
	  }
	}
	return transposed;
  }
  Matrix operator+(const Matrix &other) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(rows() == other.rows());
						BOOST_CONTRACT_ASSERT(columns() == other.columns());
					  });
	Matrix result(rows(), columns());
	for (std::size_t row = 0; row < rows(); ++row) {
	  for (std::size_t col = 0; col < columns(); ++col) {
		result.at(row, col) = this->at(row, col) + other.at(row, col);
	  }
	}
	return result;
  }
  void operator+=(const Matrix &other) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(rows() == other.rows());
						BOOST_CONTRACT_ASSERT(columns() == other.columns());
					  });
	for (std::size_t row = 0; row < rows(); ++row) {
	  for (std::size_t col = 0; col < columns(); ++col) {
		this->at(row, col) += other.at(row, col);
	  }
	}
  }
};
#endif
