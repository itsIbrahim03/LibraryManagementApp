#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "ListFetcher.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

BookListFetcher::BookListFetcher(const std::string& name, const std::string& author,const std::string& isbn)
    : name(name), author(author), isbn(isbn) {}

void BookListFetcher::fetchBookList() {
    error = "";
    try {
        httplib::SSLClient cli("openlibrary.org");

        std::string url = "/search.json?";
        if (!name.empty()) {
            url += "title=" + httplib::detail::encode_url(name) + "&";
        }
        if (!author.empty()) {
            url += "author=" + httplib::detail::encode_url(author) + "&";
        }
        if (!isbn.empty()) {
            // Perform search by ISBN
            searchByISBN(isbn);
        }
        if (url.back() == '&') {
            url.pop_back(); // Remove trailing '&'
        }
        url += "&format=json";

        auto res = cli.Get(url.c_str());

        std::lock_guard<std::mutex> lock(mtx);
        displayedBooks.clear(); // Clear previous results

        if (res) {
            if (res->status == 200) {
                nlohmann::json jsonData = nlohmann::json::parse(res->body);

                if (jsonData.contains("docs") && !jsonData["docs"].empty()) {
                    for (const auto& item : jsonData["docs"]) {
                        std::string title = item.value("title", "N/A");
                        std::string isbn = item.value("isbn", std::vector<std::string>()).empty() ? "N/A" : item["isbn"][0]; // Check if ISBN exists

                        displayedBooks.push_back({ title, isbn });
                    }
                    errorFlag.store(false); // Books found
                }
                else {
                    errorFlag.store(true); // No books found
                    error += "No Books Found.";
                }
            }
            else {
                error += "Failed to fetch book data, status code: " + std::to_string(res->status), "";
                errorFlag.store(true);
            }
        }
        else {
            auto err = res.error();
            error += "Failed to connect to the server. Error: " + httplib::to_string(err), "";
            errorFlag.store(true);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        error += "An exception occurred: " + std::string(e.what()), "";
        errorFlag.store(true);
    }
}

bool BookListFetcher::exist_error() const {
    std::lock_guard<std::mutex> lock(mtx);
    return errorFlag;
}

std::string BookListFetcher::errorType() const {
    std::lock_guard<std::mutex> lock(mtx);
    return error;
}

std::vector<std::pair<std::string, std::string>> BookListFetcher::getDisplayedBooks() const {
    std::lock_guard<std::mutex> lock(mtx);
    return displayedBooks;
}

void BookListFetcher::searchByISBN(const std::string& isbn) {
    httplib::SSLClient cli("openlibrary.org");

    std::string url = "/search.json?isbn=" + httplib::detail::encode_url(isbn) + "&format=json";

    auto res = cli.Get(url.c_str());

    std::lock_guard<std::mutex> lock(mtx);

    if (res) {
        if (res->status == 200) {
            nlohmann::json jsonData = nlohmann::json::parse(res->body);

            if (jsonData.contains("docs") && !jsonData["docs"].empty()) {
                for (const auto& item : jsonData["docs"]) {
                    std::string title = item.value("title", "N/A");
                    std::string isbn = item.value("isbn", std::vector<std::string>()).empty() ? "N/A" : item["isbn"][0]; // Check if ISBN exists

                    displayedBooks.push_back({ title, isbn });
                }
                errorFlag.store(false); // Books found
            }
            else {
                errorFlag.store(true); // No books found
                error += "No Books Found by ISBN.";
            }
        }
        else {
            error += "Failed to fetch book data by ISBN, status code: " + std::to_string(res->status);
            errorFlag.store(true);
        }
    }
    else {
        auto err = res.error();
        error += "Failed to connect to the server for ISBN. Error: " + httplib::to_string(err);
        errorFlag.store(true);
    }
}
