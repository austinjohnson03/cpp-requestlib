#pragma once

#include <curl/curl.h>
#include <string>
#include <unordered_map>

enum class HttpMethod { GET, POST, PUT, DELETE };

class Request {
public:
  Request();
  ~Request();

  Request &setUrl(const std::string &url);
  Request &setMethod(HttpMethod method);
  Request &setHeader(const std::string &key, const std::string &value);
  Request &setBody(const std::string &body);
  Request &setTimeout(long seconds);
  Request &clearHeaders();
  Request &reset();

  bool send();

  int getResponseCode() const;
  std::string getResponseBody() const;
  std::string getErrorMessage() const;

private:
  CURL *curl;
  std::string url;
  HttpMethod method = HttpMethod::GET;
  std::string body;
  std::string responseBody;
  std::string errorMessage;
  char curlError[CURL_ERROR_SIZE] = {};
  long timeout = 30;
  int responseCode = 0;
  std::unordered_map<std::string, std::string> headers;

  struct HeaderList {
    curl_slist *list = nullptr;

    void append(const std::string &header) {
      list = curl_slist_append(list, header.c_str());
    }

    ~HeaderList() { curl_slist_free_all(list); }
  };

  static size_t writeCallback(char *ptr, size_t size, size_t nmemb,
                              void *userdata);
  HeaderList buildHeaderList() const;
  void applyMethod() const;
};