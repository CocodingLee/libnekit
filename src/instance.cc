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

#include "nekit/instance.h"

#include <boost/assert.hpp>

#include "nekit/utils/error.h"
#include "nekit/utils/log.h"
#include "nekit/utils/system_resolver.h"

#undef NECHANNEL
#define NECHANNEL "Instance"

namespace nekit {

Instance::Instance(std::string name) : name_{name} {}

void Instance::AddProxyManager(std::unique_ptr<ProxyManager> &&proxy_manager) {
  BOOST_ASSERT(proxy_manager->GetRunloop() == GetRunloop());

  proxy_managers_.emplace_back(std::move(proxy_manager));
}

void Instance::Run() {
  BOOST_ASSERT(ready_);

  BOOST_LOG_SCOPED_THREAD_ATTR(
      "Instance", boost::log::attributes::constant<std::string>(name_));

  for (auto &manager : proxy_managers_) {
    manager->Run();
  }

  NEINFO << "Start running instance.";
  runloop_.Run();

  NEINFO << "Instance stopped.";
}

void Instance::Stop() {
  runloop_.Stop();
  for (auto &manager : proxy_managers_) {
    manager->Stop();
  }
  ready_ = false;
}

utils::Runloop *Instance::GetRunloop() { return &runloop_; }

}  // namespace nekit
