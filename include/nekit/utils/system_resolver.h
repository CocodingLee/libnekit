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

#pragma once

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "resolver_interface.h"

namespace nekit {
namespace utils {
class SystemResolver : public ResolverInterface {
 public:
  SystemResolver(Runloop* runloop, size_t thread_count);
  ~SystemResolver();

  Cancelable Resolve(std::string domain, AddressPreference preference,
                     EventHandler handler) override;

  void Stop() override;

  Runloop* GetRunloop() override;

 private:
  std::error_code ConvertBoostError(const boost::system::error_code& ec);

  utils::Runloop* runloop_;

  size_t thread_count_;
  boost::thread_group thread_group_;
  std::unique_ptr<
      boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>
      work_guard_;
  utils::Runloop resolve_runloop_;

  Cancelable lifetime_;
};
}  // namespace utils
}  // namespace nekit
