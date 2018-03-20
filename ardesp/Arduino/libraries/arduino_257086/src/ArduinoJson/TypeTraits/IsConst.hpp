// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that return the type T without the const modifier
template <typename T>
struct IsConst {
  static const bool value = false;
};

template <typename T>
struct IsConst<const T> {
  static const bool value = true;
};
}
}
