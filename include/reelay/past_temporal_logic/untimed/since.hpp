/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "vector"

#include "reelay/networks/basic_structure.hpp"

namespace reelay {
namespace untimed_setting {

template <typename X>
struct since : public untimed_state<X, bool> {
  using input_t = X;
  using output_t = bool;

  using node_t = untimed_node<output_t>;
  using node_ptr_t = std::shared_ptr<node_t>;

  bool value = false;

  node_ptr_t first;
  node_ptr_t second;

  since(std::vector<node_ptr_t> args) : first(args[0]), second(args[1]) {}

  void update(const input_t& args) override {
    value = second->output() || (first->output() && value);
  }

  output_t output() override { return value; }
};

}  // namespace untimed_setting
}  // namespace reelay