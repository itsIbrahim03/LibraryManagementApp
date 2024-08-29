#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "DetailsFetcher.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

BookDetailsFetcher::BookDetailsFetcher(const std::string& isbn)
    : isbn(isbn) {}

void BookDetailsFetcher::fetchBookDetails() {
    try {
        httplib::SSLClient cli("openlibrary.org");

        // Construct the URL for fetching book details
        std::string url = "/api/books?bibkeys=ISBN:" + isbn + "&format=json&jscmd=data";
        auto res = cli.Get(url.c_str());

        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex once here

        if (res) {
            if (res->status == 200) {
                nlohmann::json jsonData = nlohmann::json::parse(res->body);
                rawJsonData = jsonData.dump(4);  // Format the JSON data with indentation (4 spaces)

                // Assign the entire JSON string to bookDetails for detailed display                
                if (jsonData.contains("ISBN:" + isbn) && jsonData["ISBN:" + isbn].is_object()) {
                    auto& book = jsonData["ISBN:" + isbn];
                    std::string title = book.contains("title") && book["title"].is_string() ? book["title"].get<std::string>() : "N/A";
                    std::string author = book.contains("authors") && book["authors"].is_array() ? book["authors"][0]["name"].get<std::string>() : "N/A";
                    std::string summary = book.contains("abstract") && book["abstract"].is_string() ? book["abstract"].get<std::string>() : "No summary available";
                    std::string rating = book.contains("rating") && book["rating"].is_number() ? std::to_string(book["rating"].get<double>()) : "N/A";
                    // Format the details for display
                    bookDetails = "Title: " + title + "\n"
                        "Author: " + author + "\n"
                        "abstract: " + summary + "\n"
                        "Rating: " + rating + "\n";
                }
            }
            else {
                rawJsonData = "Failed to fetch book data, status code: " + std::to_string(res->status);
                bookDetails = rawJsonData;
            }
        }
        else {
            auto err = res.error();
            bookDetails = "Failed to connect to the server. Error: " + httplib::to_string(err);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        bookDetails = "An exception occurred: " + std::string(e.what());
    }
}




std::string BookDetailsFetcher::getBookDetails() const {
    std::lock_guard<std::mutex> lock(mtx);
    return bookDetails;
}

std::string BookDetailsFetcher::getRawJsonData() const {
    std::lock_guard<std::mutex> lock(mtx);
    return rawJsonData;
}
