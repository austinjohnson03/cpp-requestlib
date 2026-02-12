#include <iostream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

int main(void) {
  CURL *curl;
  CURLcode res;

  std::string buffer;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://google.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::cout << buffer <<std::endl;
  }
  return 0;
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char*)contents, size * nmemb);

  return size * nmemb;
}
