#ifndef BOOKUI_H
#define BOOKUI_H

#include "Manager.h"
#include <imgui.h>
#include <string>
#include <vector>
#include <chrono>
#include <atomic>
#include <icu.h>

class BookUI {
public:
    BookUI(BookManager& manager);


    void render();
    void resetFavourite();
    void showNotificationMessage(const std::string& message, int duration_ms);
    void DisplayBooks(const std::vector<std::pair<std::string, std::string>>& books);

private:
    bool searchByIsbn = false;
    bool searchByName = false;
    bool searchByAuthor = false;

    void saveBookDetailsAsFavourite();
    void renderNotificationSection();

    float progress;

    BookManager& manager;
    char isbnBuffer[128] = "";
    char nameBuffer[128] = "";
    char authorBuffer[128] = "";
    std::string selectedBook;
    std::string selectedBookIsbn;
    std::string notificationMessage;
    std::atomic<bool> showNotification{ false };
    std::atomic<bool> showBookDetails{ false };

    std::chrono::steady_clock::time_point notificationStartTime;
    std::atomic<bool> existing_Error{ false };
    std::atomic<bool> show_fav{ false };

    std::atomic<bool> show_arrow_list{ false };
    std::atomic<bool> show_sort_name{ false };
    std::atomic<bool> sort_name_button{ false };
    std::atomic<bool> show_sort_isbn{ false };
    std::atomic<bool> wasFetching{ false };
    std::atomic<bool> wasUpdating{ false };
    std::atomic<bool> show_search_options{ false };  // New atomic variable for toggling search options

};

#endif // BOOKUI_H
