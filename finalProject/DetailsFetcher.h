#ifndef BOOKDETAILSFETCHER_H
#define BOOKDETAILSFETCHER_H

#include <string>
#include <mutex>
#include <atomic>

class BookDetailsFetcher {
public:
    BookDetailsFetcher(const std::string& isbn);
    void fetchBookDetails();
    std::string getBookDetails() const;
    std::string getRawJsonData() const;

private:
    std::string isbn;
    std::string bookDetails;
    std::string rawJsonData; // Add this line to store raw JSON data
    mutable std::mutex mtx;
};

#endif // BOOKDETAILSFETCHER_H
