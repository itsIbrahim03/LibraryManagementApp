#include "Manager.h"
#include <thread> // For std::thread
#include <iostream>
#include <chrono>
#include <atomic> // For std::atomic
#include <algorithm>
// Constructor
BookManager::BookManager()
    : isUpdatingBooks(false), isFetchingDetails(false), errorFlag(false) {}

// Method to update books based on criteria
void BookManager::FetchBookList(const std::string& name, const std::string& author, const std::string& isbn) {
    clear(); // Clear previous state

    // Set flag to indicate that books are being updated
    isUpdatingBooks.store(true);

    // Create a new BookListFetcher with the provided criteria
    listFetcher = std::make_unique<BookListFetcher>(name, author, isbn);

    // Launch data fetching in a separate thread
    std::thread([this]() {
        listFetcher->fetchBookList();

        std::lock_guard<std::mutex> lock(mtx);
        displayedBooks = listFetcher->getDisplayedBooks();

        // Update noBooksFoundFlag based on the fetcher
        errorFlag.store(listFetcher->exist_error());

        errorType = listFetcher->errorType();

        // Reset the flag
        isUpdatingBooks.store(false);
        }).detach(); // Detach thread so it runs independently
}

// Method to fetch book details based on ISBN
void BookManager::fetchBookDetails(const std::string& isbn) {
    // Clear previous details
    {
        std::lock_guard<std::mutex> lock(mtx);
        bookDetails.clear();
        rawJsonData.clear(); // Clear previous raw JSON data
    }

    // Set flag to indicate that details are being fetched
    isFetchingDetails.store(true);

    // Create a new BookDetailsFetcher with the provided ISBN
    detailsFetcher = std::make_unique<BookDetailsFetcher>(isbn);

    // Launch data fetching in a separate thread
    std::thread([this, isbn]() {
        detailsFetcher->fetchBookDetails();

        std::lock_guard<std::mutex> lock(mtx);
        bookDetails = detailsFetcher->getBookDetails();
        rawJsonData = detailsFetcher->getRawJsonData(); // Store raw JSON data

        // Reset the flag
        isFetchingDetails.store(false);
        }).detach(); // Detach thread so it runs independently
}

// Get the displayed books
std::vector<std::pair<std::string, std::string>> BookManager::getDisplayedBooks() const {
    std::lock_guard<std::mutex> lock(mtx);
    return displayedBooks;
}

// Get the selected book's ISBN
std::string BookManager::getSelectedBook() const {
    std::lock_guard<std::mutex> lock(mtx);
    return selectedBook;
}

// Set the selected book's ISBN
void BookManager::selectBook(const std::string& isbn) {
    std::lock_guard<std::mutex> lock(mtx);
    selectedBook = isbn;
}

// Get the details of the selected book
std::string BookManager::getBookDetails() const {
    std::lock_guard<std::mutex> lock(mtx);
    return bookDetails;
}

// Get the raw JSON data of the selected book
std::string BookManager::getRawJsonData() const {
    std::lock_guard<std::mutex> lock(mtx);
    return rawJsonData;
}

// Clear all book data
void BookManager::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    displayedBooks.clear();
    selectedBook.clear();
    bookDetails.clear();
    rawJsonData.clear(); // Clear raw JSON data
    errorFlag.store(false); // Reset no books found flag
}

// Check if books are being updated
bool BookManager::isBooksBeingUpdated() const {
    return isUpdatingBooks.load();
}

// Check if book details are being fetched
bool BookManager::isDetailsBeingFetched() const {
    return isFetchingDetails.load();
}

bool BookManager::errorExisting() const {
    return errorFlag.load();
}

std::string BookManager::getErrorType() const {
    return errorType;
}

void BookManager::addSearchToHistory(const std::string& title, const std::string& isbn) {
    booksHistory.push_back({ title,isbn });
}
std::vector < std::pair < std::string, std::string>>& BookManager::getBooksHistory() {
    return booksHistory;
}
void BookManager::clearHistory() {
    booksHistory.clear();
}

std::vector<std::pair<std::string, std::string>> BookManager::sortDisplayedBooksByName(std::vector<std::pair<std::string, std::string>>& displayedBooks) {
    // Create a copy of the input vector
    std::vector<std::pair<std::string, std::string>> sortedBooks = displayedBooks;

    // Sort the copy
    std::sort(sortedBooks.begin(), sortedBooks.end(),
        [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
            return a.first < b.first; // Compare the first elements of the pairs
        });

    return sortedBooks; // Return the sorted vector
}

