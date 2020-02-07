/*
 * Copyright (c) 2019-2020 Dogan Ulus
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
namespace discrete_timed_data_setting {

template <typename X, typename T>
struct previous : public discrete_timed_state<X, data_set_t, T> {
  using time_t = T;
  using input_t = X;
  using value_t = data_set_t;
  using output_t = data_set_t;

  using node_t = discrete_timed_node<output_t, time_t>;
  using state_t = discrete_timed_state<input_t, output_t, time_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  data_mgr_t manager;

  node_ptr_t first;

  data_set_t prev_value;
  data_set_t value;

  explicit previous(const data_mgr_t &mgr, const std::vector<node_ptr_t> &args)
      : manager(mgr), first(args[0]) {
    prev_value = mgr->zero();
    value = mgr->zero();
  }

  explicit previous(const kwargs &kw)
      : previous(reelay::any_cast<data_mgr_t>(kw.at("manager")),
                 reelay::any_cast<std::vector<node_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &, time_t now) override {
    prev_value = value;
    value = first->output(now);
  }

  output_t output(time_t now) override { return prev_value; }
};

} // namespace discrete_timed_setting
} // namespace reelay