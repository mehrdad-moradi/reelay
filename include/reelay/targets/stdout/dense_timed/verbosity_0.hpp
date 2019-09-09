/*
 * Copyright (c) 2019 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "reelay/interval_set.hpp"
#include "reelay/settings.hpp"
// #include "reelay/targets/stdout/dense_timed/formatter.hpp"

namespace reelay {
namespace dense_timed_setting {

template <typename X, typename T>
struct stdout_formatter_verbosity_0
    : public dense_timed_state<X, std::string, T> {
  using time_t = T;
  using input_t = X;
  using output_t = std::string;

  using interval_t = reelay::interval<time_t>;
  using interval_set_t = reelay::interval_set<time_t>;

  using network_t = dense_timed_network<input_t, interval_set_t, time_t>;
  using network_ptr_t = std::shared_ptr<network_t>;

  using strings_t = std::vector<std::string>;

  network_ptr_t network;
  strings_t columm_names;

  stdout_formatter_verbosity_0(network_ptr_t netptr, strings_t names)
    : network(netptr), columm_names(names) {}

  void update(const input_t &args) { network->update(args); }
  void update(const input_t &args, time_t now) { network->update(args, now); }
  void update(const input_t &pargs, const input_t &args, time_t previous,
              time_t now) override {
    network->update(pargs, args, previous, now);
  }

  std::string header(const std::vector<std::string>& columm_names) override {
    return std::string();
  }
  std::string output() {
    std::ostringstream buffer;

    interval_set_t result = network->output();

    for (const auto& intv : result) {
      buffer << "Fails between times " << intv.lower() << " and "
             << intv.upper() << std::endl;
    }

    return buffer.str();
  }
};

}  // namespace dense_timed_setting
}  // namespace reelay