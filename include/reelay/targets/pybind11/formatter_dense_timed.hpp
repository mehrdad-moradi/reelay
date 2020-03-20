/*
 * Copyright (c) 2019-2020 Dogan Ulus
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

 #pragma once

#include "reelay/intervals.hpp"
#include "reelay/pydict.hpp"

namespace reelay {

template <typename T>
struct formatter_dense_timed {

  using time_t = T;
  using value_t = bool;
  using input_t = pybind11::dict;
  using output_t = pybind11::list;

  using interval = reelay::interval<time_t>;
  using interval_set = reelay::interval_set<time_t>;

  const std::string t_name;
  const std::string y_name;
 
  value_t lastval = false;

  explicit formatter_dense_timed(const std::string t_str = "time",
                                 const std::string y_str = "value")
      : t_name(t_str), y_name(y_str) {}

  output_t format(const interval_set& result, time_t previous, time_t now){
    if (now == 0) {
      return _init_0(result, previous, now);
    } else if (previous == 0) {
      return _init_1(result, previous, now);
    } else {
      return _format(result, previous, now);
    }
  }

  output_t _init_0(const interval_set& result, time_t previous, time_t now){
    // This code takes care of the special case of time zero
    // HERE: time_t now = 0
    return pybind11::list();
  }

  output_t _init_1(const interval_set& result, time_t previous, time_t now){
    // This code takes care of the special case of the first segment
    // The variable `lastval` is meaningless for the first segment
    // HERE: The first segment ranges from previous=0 to now

    auto vresult = pybind11::list();

    if (result.empty()) {
      vresult.append(pydict(pybind11::arg(t_name.c_str()) = previous,
                            pybind11::arg(y_name.c_str()) = false));
      lastval = false;
    } else {
      if (result.begin()->lower() != previous) {
        vresult.append(pydict(pybind11::arg(t_name.c_str()) = previous,
                              pybind11::arg(y_name.c_str()) = false));
        lastval = false;
      }
      for (const auto &intv : result) {
        vresult.append(pydict(pybind11::arg(t_name.c_str()) = intv.lower(),
                              pybind11::arg(y_name.c_str()) = true));
        lastval = true;

        if (intv.upper() != now) {
          vresult.append(pydict(pybind11::arg(t_name.c_str()) = intv.upper(),
                                pybind11::arg(y_name.c_str()) = false));
          lastval = false;
        }
      }
    }

    return vresult;
  }

  output_t _format(const interval_set& result, time_t previous, time_t now){
    // This code constitutes the main operation such that 
    //   + do output two events per interval in the set
    //   + take care empty interval set (meaning all false over the period)
    //   + do not output if it is the same value with the last value (lastval)
    //   + do not output for the current point (now) as it may be not finished

    auto vresult = pybind11::list();

    if (result.empty()) {
      if (lastval) {
        vresult.append(pydict(pybind11::arg(t_name.c_str()) = previous,
                              pybind11::arg(y_name.c_str()) = false));
        lastval = false;
      }
    } else {
      for (const auto &intv : result) {
        if (not lastval) {
          vresult.append(pydict(pybind11::arg(t_name.c_str()) = intv.lower(),
                                pybind11::arg(y_name.c_str()) = true));
          lastval = true;
        }
        if (intv.upper() != now) {
          vresult.append(pydict(pybind11::arg(t_name.c_str()) = intv.upper(),
                                pybind11::arg(y_name.c_str()) = false));
          lastval = false;
        }
      }
    }

    return vresult;
  }
};

} //namespace reelay