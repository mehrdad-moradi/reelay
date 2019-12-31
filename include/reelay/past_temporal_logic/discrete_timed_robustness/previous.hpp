/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "memory"
#include "vector"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/networks/discrete_timed_network.hpp"

namespace reelay {
namespace discrete_timed_robustness_setting {

template <typename X, typename V, typename T>
struct previous : public discrete_timed_state<X, V, T> {
  using time_t = T;
  using input_t = X;
  using output_t = V;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  output_t prev_value = -std::numeric_limits<output_t>::max();
  output_t value = -std::numeric_limits<output_t>::max();

  node_ptr_t first;

  explicit previous(const std::vector<node_ptr_t> &args) : first(args[0]) {}

  explicit previous(const kwargs &kw)
      : previous(std::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args, time_t now) override {
    prev_value = value;
    value = first->output(now);
  }

  output_t output(time_t now) override { return prev_value; }
};

} // namespace discrete_timed_setting
} // namespace reelay