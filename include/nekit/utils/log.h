// MIT License

// Copyright (c) 2017 Zhuhao Wang

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// This file is only for libnekit project. Do not use it in the
// application.

#pragma once

#include <boost/log/utility/manipulators/add_value.hpp>

#include "../config.h"
#include "logger.h"

#define NELOGGER nekit::utils::default_logger()
#define NECHANNEL "Default"

#define NELOG(NELEVEL) NELOG_INTERNAL(NELOGGER, NECHANNEL, NELEVEL)
#define NELOGT(NELEVEL)                                                  \
  NELOG_INTERNAL(NELOGGER, NECHANNEL, NELEVEL) << boost::log::add_value( \
      NEKIT_BOOST_LOG_TRACK_ID_ATTR_NAME, this->GetTrackId())

class NullLogger {
 public:
  NullLogger() {}

  template <typename Val>
  NullLogger& operator<<(const Val&) {
    return *this;
  }
};

#define NENOLOG \
  while (0) NullLogger()

#define NETRACE NELOG(nekit::utils::LogLevel::Trace)
#define NEDEBUG NELOG(nekit::utils::LogLevel::Debug)
#define NEINFO NELOG(nekit::utils::LogLevel::Info)
#define NEWARN NELOG(nekit::utils::LogLevel::Warning)
#define NEERROR NELOG(nekit::utils::LogLevel::Error)
#define NEFATAL NELOG(nekit::utils::LogLevel::Fatal)

#define NETRACET NELOGT(nekit::utils::LogLevel::Trace)
#define NEDEBUGT NELOGT(nekit::utils::LogLevel::Debug)
#define NEINFOT NELOGT(nekit::utils::LogLevel::Info)
#define NEWARNT NELOGT(nekit::utils::LogLevel::Warning)
#define NEERRORT NELOGT(nekit::utils::LogLevel::Error)
#define NEFATALT NELOGT(nekit::utils::LogLevel::Fatal)

#define NELOG_INTERNAL(NELOGGER_, NECHANNEL_, NELEVEL_) \
  BOOST_LOG_CHANNEL_SEV(NELOGGER_, NECHANNEL_, NELEVEL_)
