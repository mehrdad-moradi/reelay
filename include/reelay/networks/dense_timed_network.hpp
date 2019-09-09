/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/interval_set.hpp"
#include "reelay/lexical_cast.hpp"
#include "reelay/networks/basic_structure.hpp"

namespace reelay {

template <typename X, typename Y, typename T>
struct dense_timed_network : dense_timed_state<X, Y, T> {
  using time_t = T;
  using input_t = X;
  using output_t = Y;

  using type = dense_timed_network<input_t, output_t, time_t>;

  using node_t = dense_timed_node<output_t, time_t>;
  using state_t = dense_timed_state<input_t, output_t, time_t>;

  time_t previous = 0;
  time_t current = 0;

  input_t prevargs;

  std::shared_ptr<node_t> output_node;
  std::vector<std::shared_ptr<state_t>> states;

  dense_timed_network(std::shared_ptr<node_t> n,
                      std::vector<std::shared_ptr<state_t>> ss)
      : output_node(n), states(ss) {}

  void update(const input_t& args) {
    time_t now = boost::lexical_cast<time_t>(args.at("time"));
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t& args, time_t now) {
    this->previous = this->current;
    update(this->prevargs, args, this->previous, now);
    this->current = now;
    this->prevargs = args;
  }

  void update(const input_t& pargs,
              const input_t& args,
              time_t previous,
              time_t now) noexcept override {
    for (const auto& state : this->states) {
      state->update(pargs, args, previous, now);
    }
  }

  output_t output() {
    return this->output_node->output(this->previous, this->current);
  }

};

}  // namespace reelay