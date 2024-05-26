#pragma once

#include <memory>

#include "Object.h"
#include "Runtime.h"

template <typename T, typename T2>
bool inline isinstance(std::shared_ptr<T2> a) { return dynamic_cast<T *>(a.get()) != nullptr; }

template <typename T, typename T2>
bool inline isinstance(T2 *a) { return dynamic_cast<T *>(a) != nullptr; }

objptr_t exec(std::string, std::string, Runtime *, bool verbose = false);
objptr_t exec(std::string, std::string, bool verbose = false);

void shell(bool verbose = false);