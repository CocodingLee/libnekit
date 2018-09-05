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

#include "nekit/utils/maxmind.h"

namespace nekit {
namespace utils {

std::string MaxmindErrorCategory::Description(const utils::Error& error) const {
  return MMDB_strerror(error.ErrorCode());
}

std::string MaxmindErrorCategory::DebugDescription(
    const utils::Error& error) const {
  return Description(error);
}

MaxmindLookupResult::MaxmindLookupResult(MMDB_lookup_result_s result)
    : result_{result} {};

CountryIsoCode MaxmindLookupResult::country_iso_code() {
  if (!result_.found_entry) {
    return CountryIsoCode::XX;
  }

  MMDB_entry_data_s data;
  int status =
      MMDB_get_value(&result_.entry, &data, "country", "iso_code", NULL);

  if (status == MMDB_LOOKUP_PATH_DOES_NOT_MATCH_DATA_ERROR) {
    return CountryIsoCode::XX;
  }

  assert(status == MMDB_SUCCESS);
  assert(data.has_data);
  assert(data.type == MMDB_DATA_TYPE_UTF8_STRING);

  std::string code(data.utf8_string, data.data_size);
  assert(code.size() == 2);

  return CountryIsoCodeFromString(code);
}

bool Maxmind::Initalize(std::string db_file) {
  return MMDB_open(db_file.c_str(), 0, &GetMmdb()) == MMDB_SUCCESS;
}

utils::Result<MaxmindLookupResult> Maxmind::Lookup(const std::string& ip) {
  int gai_error, mmdb_error;
  MMDB_lookup_result_s result =
      MMDB_lookup_string(&GetMmdb(), ip.c_str(), &gai_error, &mmdb_error);

  assert(gai_error == MMDB_SUCCESS);
  if (mmdb_error != MMDB_SUCCESS) {
    return MakeErrorResult(
        Error(MaxmindErrorCategory::GlobalMaxmindErrorCategory(), mmdb_error));
  }

  return result;
}

utils::Result<MaxmindLookupResult> Maxmind::Lookup(
    const boost::asio::ip::address& ip) {
  return Lookup(boost::asio::ip::tcp::endpoint(ip, 0));
}

template <typename Protocol>
utils::Result<MaxmindLookupResult> Maxmind::Lookup(
    const boost::asio::ip::basic_endpoint<Protocol>& endpoint) {
  int mmdb_error;
  MMDB_lookup_result_s result =
      MMDB_lookup_sockaddr(&GetMmdb(), endpoint.data(), &mmdb_error);

  if (mmdb_error != MMDB_SUCCESS) {
    return MakeErrorResult(
        Error(MaxmindErrorCategory::GlobalMaxmindErrorCategory(), mmdb_error));
  }

  return result;
}

MMDB_s& Maxmind::GetMmdb() {
  static MMDB_s mmdb_;
  return mmdb_;
}
}  // namespace utils
}  // namespace nekit
