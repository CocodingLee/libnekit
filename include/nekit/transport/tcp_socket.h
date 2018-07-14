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

#include <functional>
#include <system_error>
#include <vector>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "../data_flow/local_data_flow_interface.h"
#include "../data_flow/remote_data_flow_interface.h"
#include "tcp_connector.h"
#include "tcp_listener.h"

namespace nekit {
namespace transport {

class TcpSocket final : public data_flow::LocalDataFlowInterface,
                        public data_flow::RemoteDataFlowInterface {
 public:
  explicit TcpSocket(std::shared_ptr<utils::Session> session);
  ~TcpSocket();

  utils::Cancelable Read(utils::Buffer&&, DataEventHandler) override
      __attribute__((warn_unused_result));
  utils::Cancelable Write(utils::Buffer&&, EventHandler) override
      __attribute__((warn_unused_result));

  utils::Cancelable CloseWrite(EventHandler) override
      __attribute__((warn_unused_result));

  const data_flow::FlowStateMachine& StateMachine() const override;

  data_flow::DataFlowInterface* NextHop() const override;

  data_flow::DataType FlowDataType() const override;

  std::shared_ptr<utils::Session> Session() const override;

  boost::asio::io_context* io() override;

  utils::Cancelable Open(EventHandler) override
      __attribute__((warn_unused_result));

  utils::Cancelable Continue(EventHandler) override
      __attribute__((warn_unused_result));

  utils::Cancelable Connect(EventHandler) override
      __attribute__((warn_unused_result));

  std::shared_ptr<utils::Endpoint> ConnectingTo() override;

  friend class TcpListener;

 private:
  explicit TcpSocket(boost::asio::ip::tcp::socket&& socket,
                     std::shared_ptr<utils::Session> session);

  std::error_code ConvertBoostError(const boost::system::error_code&) const;

  boost::asio::ip::tcp::socket socket_;
  std::unique_ptr<TcpConnector> connector_;
  std::shared_ptr<utils::Session> session_;
  std::shared_ptr<utils::Endpoint> connect_to_;
  std::unique_ptr<std::vector<boost::asio::const_buffer>> write_buffer_;
  std::unique_ptr<std::vector<boost::asio::mutable_buffer>> read_buffer_;
  utils::Cancelable read_cancelable_, write_cancelable_, report_cancelable_,
      connect_cancelable_;

  data_flow::FlowStateMachine state_machine_;
};
}  // namespace transport
}  // namespace nekit
