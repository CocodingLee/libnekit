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

#include <utility>
#include <vector>

#include "../utils/subnet.h"
#include "rule_interface.h"

namespace nekit {
namespace rule {
// TODO: Current implementation is far from efficient. The subnet block should
// be stored in binary tree to be looked up in O(log n) time and overrlapping
// and contiguous blocks should be merged.
class SubnetRule : public RuleInterface {
 public:
  SubnetRule(RuleHandler handler);

  void AddSubnet(const boost::asio::ip::address &address, int prefix);

  MatchResult Match(std::shared_ptr<utils::Session> session) override;
  std::unique_ptr<data_flow::RemoteDataFlowInterface> GetDataFlow(
      std::shared_ptr<utils::Session> session) override;

 private:
  bool LookUp(const boost::asio::ip::address &address);

  std::vector<utils::Subnet> subnets_;

  RuleHandler handler_;
};
}  // namespace rule
}  // namespace nekit
