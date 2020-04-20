/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "string"

#include "reelay/common.hpp"
#include "reelay/networks/basic_structure.hpp"
#include "reelay/unordered_data.hpp"

namespace reelay {
namespace untimed_data_setting {

template <typename X>
struct atomic_nested : public untimed_state<X, data_set_t> {
  using input_t = X;
  using output_t = data_set_t;

  using node_t = untimed_node<output_t>;
  using state_t = untimed_state<input_t, output_t>;

  using node_ptr_t = std::shared_ptr<node_t>;
  using state_ptr_t = std::shared_ptr<state_t>;

  using function_t = std::function<data_set_t(const input_t &)>;

  data_mgr_t manager;
  data_set_t value;

  std::vector<std::string> path;
  std::vector<state_ptr_t> states;

  explicit atomic_nested(const data_mgr_t &mgr,
                         const std::vector<std::string> &p,
                         const std::vector<state_ptr_t> &ptrs)
      : manager(mgr), path(p), states(ptrs) {}

  explicit atomic_nested(const kwargs &kw)
      : atomic_nested(
            reelay::any_cast<data_mgr_t>(kw.at("manager")),
            reelay::any_cast<std::vector<std::string>>(kw.at("path")),
            reelay::any_cast<std::vector<state_ptr_t>>(kw.at("args"))) {}

  void update(const input_t &args) override {

    input_t deep_args = args;
    for (const auto &key : this->path){
      if (not datafield<input_t>::contains(deep_args, key)) {
        return; // Do nothing if the key does not exist 
      }
      deep_args = datafield<input_t>::at(deep_args, key);
    }
    for (const auto &state : this->states) {
      state->update(deep_args);
    }
    value = states[0]->output();
    for (std::size_t i = 1; i < states.size(); i++) {
      value *= states[i]->output();
    }
  }

  output_t output() override { return value; }
};

} // namespace discrete_timed_setting
} // namespace reelay