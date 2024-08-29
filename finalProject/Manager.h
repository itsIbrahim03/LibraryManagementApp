#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include "ListFetcher.h"
#include "DetailsFetcher.h"

class BookManager {
public:
    // Constructor
    BookManager();

    // Method to update books based on search criteria
    void FetchBookList(const std::string& name, const std::string& author, const std::string& isbn);

    // Method to fetch book details based on ISBN
    void fetchBookDetails(const std::string& isbn);

    // Get the list of displayed books
    std::vector<std::pair<std::string, std::string>> getDisplayedBooks() const;

    // Get the ISBN of the selected book
    std::string getSelectedBook() const;

    // Set the ISBN of the selected book
    void selectBook(const std::string& isbn);

    // Get the details of the selected book
    std::string getBookDetails() const;

    // Get the raw JSON data of the selected book
    std::string getRawJsonData() const;

    // Clear all book data
    void clear();

    // Check if books are being updated
    bool isBooksBeingUpdated() const;

    // Check if book details are being fetched
    bool isDetailsBeingFetched() const;

    bool errorExisting() const;

    std::string getErrorType() const;
    std::atomic<bool> isUpdatingBooks;
    std::atomic<bool> isFetchingDetails;


    void addSearchToHistory(const std::string& title, const std::string& isbn);
    std::vector<std::pair<std::string, std::string>>& getBooksHistory();
    void clearHistory();
    std::vector<std::pair<std::string, std::string>> displayedBooks;
    std::vector<std::pair<std::string, std::string>> sortDisplayedBooksByName(std::vector<std::pair<std::string, std::string>>& displayedBooks);

private:
    mutable std::mutex mtx; // Mutex for thread-safe access to shared data

    // Atomic flags to indicate if operations are in progress

    std::atomic<bool> errorFlag{ false }; // Atomic boolean for status

    // Unique pointers for fetcher objects
    std::unique_ptr<BookListFetcher> listFetcher;
    std::unique_ptr<BookDetailsFetcher> detailsFetcher;

    // Data members to store books and details
    std::string selectedBook;
    std::string bookDetails;
    std::string rawJsonData; // Added member to store raw JSON data
    std::string errorType;

    std::vector<std::pair<std::string, std::string>> booksHistory;
};

#endif // BOOKMANAGER_H
