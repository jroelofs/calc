#ifndef CALC_ERROROR_H
#define CALC_ERROROR_H

namespace calc {

template <typename T> class Erroneous {
public:
  Erroneous(const T &val) : value(val) {}
  Erroneous(T &&val) : value(std::move(val)) {}
  T value;
};

template <typename T>
class ErrorOr {
public:
  using Error = std::string;

  ErrorOr() : value(), containsError(false) {}
  ErrorOr(const T &rhs) : containsError(false) { new (&value) T(rhs); }
  ErrorOr(const ErrorOr<T> &rhs) : containsError(rhs.containsError) {
    if (containsError) {
      new (&error) Error(rhs.error);
    } else {
      new (&value) T(rhs.value);
    }
  }
  explicit ErrorOr(T &&rhs) : containsError(false) {
    new (&value) T(std::forward<T>(rhs));
  }
  ErrorOr(const Erroneous<Error> &err) : containsError(true) {
    new (&error) Error(err.value);
  }

  ~ErrorOr() {
    if (containsError) {
      error.~Error();
    } else {
      value.~T();
    }
  }

  ErrorOr &operator=(ErrorOr rhs) {
    this->~ErrorOr();

    containsError = rhs.containsError;

    if (containsError) {
      new (&error) Error(rhs.error);
    } else {
      new (&value) T(rhs.value);
    }

    return *this;
  }

  bool hasValue() const { return !containsError; }
  bool hasError() const { return containsError; }
  explicit operator bool() const { return !containsError; }
  const Error &getError() const { assert(containsError); return error; }

  T &operator*() {
    assert(!containsError);
    return value;
  }
  const T &operator*() const {
    assert(!containsError);
    return value;
  }

  T *operator->() {
    assert(!containsError);
    return &value;
  }
  const T *operator->() const {
    assert(!containsError);
    return &value;
  }

  static ErrorOr makeError(const Error &err) { return ErrorOr(Erroneous(err)); }

  union {
    T value;
    Error error;
  };
  bool containsError;
};

template<typename T>
ErrorOr<T> operator*(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs * *rhs;
}

template<typename T>
ErrorOr<T> operator/(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs - *rhs;
}

template<typename T>
ErrorOr<T> operator+(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs + *rhs;
}

template<typename T>
ErrorOr<T> operator-(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return lhs;
  }

  if (rhs.hasError()) {
    return rhs;
  }

  return *lhs - *rhs;
}

template<typename T>
bool operator==(const ErrorOr<T> &lhs, const T &rhs) {
  return lhs.hasValue() && *lhs == rhs;
}

template<typename T>
bool operator==(const T &lhs, const ErrorOr<T> &rhs) {
  return rhs.hasValue() && lhs == *rhs;
}

template<typename T>
bool operator==(const ErrorOr<T> &lhs, const ErrorOr<T> &rhs) {
  if (lhs.hasError()) {
    return rhs.hasError();
  } else {
    return !rhs.hasError() && *lhs == *rhs;
  }
}

} // namespace calc

#endif