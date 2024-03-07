#pragma once

#include "Logger.hpp"

#define BIT(x) (1 << (x))

#define CG_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>...); }