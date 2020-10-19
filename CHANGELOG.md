# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## master

## 0.6.0 - 2020-10-19
### Added
- Add `make_array` for creating an array with a function that constructs the elements.

## 0.5.0 - 2020-10-13
### Added
- Add `error::last_os_error()` overloads to create an error from `errno`.

## 0.4.0 - 2020-09-01
### Added
- Add Catch2 string serialization for `result<T, E>` and `error`.
- Add [[nodiscard]] attribute to `result<T, E>` to prevent silently ignored errors.
