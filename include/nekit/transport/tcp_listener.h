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

#include <system_error>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "listener_interface.h"

namespace nekit {
namespace transport {

class TcpListener : public ListenerInterface, private boost::noncopyable {
 public:
  enum class ErrorCode { NoError = 0 };

  TcpListener(utils::Runloop* runloop, DataFlowHandler handler);

  std::error_code Bind(std::string ip, uint16_t port);
  std::error_code Bind(boost::asio::ip::address ip, uint16_t port);

  void Accept(EventHandler handler) override;

  void Close() override;

  utils::Runloop* GetRunloop() override;

 private:
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  utils::Runloop* runloop_;

  DataFlowHandler handler_;
};

std::error_code make_error_code(TcpListener::ErrorCode ec);

}  // namespace transport
}  // namespace nekit

namespace std {
template <>
struct is_error_code_enum<nekit::transport::TcpListener::ErrorCode>
    : public true_type {};
}  // namespace std
