#include "request.hpp"

#include <curl/curl.h>

Request::Request() { curl = curl_easy_init(); }

Request::~Request() { curl_easy_cleanup(curl); }

Request &Request::setUrl(const std::string &url) {
  this->url = url;
  return *this;
}

Request &Request::setMethod(HttpMethod method) {
  this->method = method;
  return *this;
}

Request &Request::setHeader(const std::string &key, const std::string &value) {
  headers[key] = value;
  return *this;
}

Request &Request::setBody(const std::string &body) {
  this->body = body;
  return *this;
}

Request &Request::setTimeout(long seconds) {
  this->timeout = seconds;
  return *this;
}

Request &Request::clearHeaders() {
  headers.clear(); //  Clears header map
  return *this;
}

Request &Request::reset() {
  url.clear();
  body.clear();
  responseBody.clear();
  errorMessage.clear();
  curlError[0] = '\0';
  method = HttpMethod::GET;
  timeout = 30;
  responseCode = 0;
  clearHeaders();
  return *this;
}

bool Request::send() {
  HeaderList headerList = buildHeaderList();

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList.list);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlError);
  applyMethod();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

  CURLcode res = curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

  if (res != CURLE_OK) {
    errorMessage = curlError[0] ? curlError : curl_easy_strerror(res);
    return false;
  }

  return true;
}

Request::HeaderList Request::buildHeaderList() const {
  HeaderList headerList;

  for (const auto &[key, value] : headers) {
    headerList.append(key + ": " + value);
  }

  return headerList;
}

void Request::applyMethod() const {
  switch (method) {
  case HttpMethod::GET:
    break;
  case HttpMethod::POST:
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    break;
  case HttpMethod::PUT:
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    break;
  case HttpMethod::DELETE:
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    break;
  }
}

int Request::getResponseCode() const { return responseCode; }

std::string Request::getResponseBody() const { return responseBody; }

std::string Request::getErrorMessage() const { return errorMessage; }

size_t Request::writeCallback(char *ptr, size_t size, size_t nmemb,
                              void *userdata) {
  std::string *response = static_cast<std::string *>(userdata);
  response->append(ptr, size * nmemb);
  return size * nmemb;
}
