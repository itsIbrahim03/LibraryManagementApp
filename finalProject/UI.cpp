#include "UI.h"
#include "Manager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>

namespace fs = std::filesystem;
std::vector<std::pair<std::string, std::string>> books;
std::vector<std::pair<std::string, std::string>> sorted_books_name;
std::vector<std::pair<std::string, std::string>> fav_books;
BookUI::BookUI(BookManager& manager) : manager(manager) {}

void BookUI::render() {

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowSize(ImVec2(1100, 1000));

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(250, 10, 20, 150);
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;
    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

    ImGui::Begin("Library Management", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::SetWindowFontScale(1.33f);

    // Search by ISBN
    ImGui::Checkbox("Search by ISBN", &searchByIsbn);
    ImGui::InputTextWithHint("##isbn", "Enter ISBN:", isbnBuffer, IM_ARRAYSIZE(isbnBuffer), searchByIsbn ? ImGuiInputTextFlags_CharsDecimal : ImGuiInputTextFlags_ReadOnly);

    // Next search fields
    ImGui::Checkbox("Search by Title", &searchByName);
    ImGui::InputTextWithHint("##Title", "Enter Book Title:", nameBuffer, IM_ARRAYSIZE(nameBuffer), searchByName ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly);

    ImGui::Checkbox("Search by Author", &searchByAuthor);
    ImGui::InputTextWithHint("##author", "Enter Author:", authorBuffer, IM_ARRAYSIZE(authorBuffer), searchByAuthor ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly);

    ImGui::Separator();
    ImGui::Text("Options:");

    if (ImGui::Button("Search", ImVec2(150, 30))) {
        books.clear();
        sorted_books_name.clear();
        progress = 0.0f;
        manager.FetchBookList(
            searchByName ? nameBuffer : "",
            searchByAuthor ? authorBuffer : "",
            searchByIsbn ? isbnBuffer : ""
        );

        showBookDetails = false; // Hide details when performing a new search
        selectedBook.clear();    // Clear selected book
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(150, 30))) {
        // Clear all search buffers and disable search toggles
        isbnBuffer[0] = '\0';
        nameBuffer[0] = '\0';
        authorBuffer[0] = '\0';
        searchByIsbn = false;
        searchByName = false;
        searchByAuthor = false;
        showBookDetails = false; // Hide details
        selectedBook.clear();    // Clear selected book
    }

    books = manager.getDisplayedBooks();

    ImGui::SameLine(0, 555);

    // Favourite Button
    if (ImGui::Button("Favourites", ImVec2(150, 30))) {
        show_fav.store(!show_fav.load());
    }

    ImGui::Separator();
    ImGui::Text("Filtering Options");

    // Custom styling for the arrow button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black background
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));    // White text

    // Render the arrow button
    ImGui::SameLine();
    if (ImGui::ArrowButton("filter", ImGuiDir_Down)) {
        show_sort_name.store(!show_sort_name);
    }

    // Restore previous style
    ImGui::PopStyleColor(2);

    if (show_sort_name) {
        if (ImGui::Button("Sort A to Z")) {
            // Sort A to Z
            sorted_books_name = books;  // Copy the original books vector
            std::sort(sorted_books_name.begin(), sorted_books_name.end(), [](const auto& a, const auto& b) {
                return a.first < b.first; // Sort A to Z
                });
        }

        if (ImGui::Button("Sort Z to A")) {
            // Sort Z to A
            sorted_books_name = books;  // Copy the original books vector
            std::sort(sorted_books_name.begin(), sorted_books_name.end(), [](const auto& a, const auto& b) {
                return a.first > b.first; // Sort Z to A
                });
        }

    }

    if (manager.isUpdatingBooks) {
        ImGui::Text("LOADING, PLEASE WAIT.......");

        if (!wasUpdating) {
            progress = 0.0f;
            wasUpdating = true;
        }

        // Draw a gradient progress bar
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImVec2 size = ImGui::GetContentRegionAvail();

        size.y = 20; // Set the height of the progress bar

        ImColor color_start = ImColor(0.2f, 0.8f, 0.2f);  // Green
        ImColor color_end = ImColor(0.8f, 0.2f, 0.2f);    // Red

        draw_list->AddRectFilledMultiColor(p, ImVec2(p.x + size.x * progress, p.y + size.y), color_start, color_end, color_end, color_start);
        draw_list->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), ImColor(255, 255, 255)); // Optional: Add border

        progress = fmod(progress + 0.01f, 1.0f); // Loop the progress value between 0 and 1
        ImGui::Dummy(size); // Move the cursor to avoid overlap

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (sort_name_button) {
        sorted_books_name = manager.sortDisplayedBooksByName(books);
        sort_name_button.store(!sort_name_button.load());
    }

    ImGui::Text("Books:");
    ImGui::BeginChild("BooksList", ImVec2(0, 400), true, ImGuiWindowFlags_HorizontalScrollbar);

    if (!sorted_books_name.empty()) {
        DisplayBooks(sorted_books_name);
    }
    else if (!books.empty()) {
        DisplayBooks(books);
    }
    else {
        ImGui::Text("No books to display.");
    }

    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Text("Book Details");

    if (showBookDetails && !selectedBook.empty()) {
        ImGui::BeginChild("BookDetails", ImVec2(0, 140), true);

        ImGui::Text("Title: %s", selectedBook.c_str());
        ImGui::Separator();

        std::string bookDetails = manager.getBookDetails();
        if (manager.isFetchingDetails) {
            ImGui::Text("Loading....");
            if (!wasFetching) {
                progress = 0.0f;
                wasFetching = true;
            }

            if (progress < 1.0f) {
                progress += 0.01f;
            }
            else {
                progress = 1.0f;
            }

            // Draw a gradient progress bar
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImVec2 size = ImGui::GetContentRegionAvail();

            size.y = 20; // Set the height of the progress bar

            ImColor color_start = ImColor(0.2f, 0.8f, 0.2f);  // Green
            ImColor color_end = ImColor(0.8f, 0.2f, 0.2f);    // Red

            draw_list->AddRectFilledMultiColor(p, ImVec2(p.x + size.x * progress, p.y + size.y), color_start, color_end, color_end, color_start);
            draw_list->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), ImColor(255, 255, 255)); // Optional: Add border

            ImGui::Dummy(size); // Move the cursor to avoid overlap

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }


        ImGui::TextWrapped("%s", bookDetails.c_str());
        ImGui::Separator();

        ImGui::SameLine();
        if (ImGui::Button("Add to Favourite")) {
            saveBookDetailsAsFavourite();
            fav_books.push_back({ selectedBook,selectedBookIsbn });
        }

        ImGui::EndChild();
    }

    ImGui::Separator();
    ImGui::Text("status");
    renderNotificationSection();
    if (existing_Error) {
        showNotificationMessage(manager.getErrorType(), 2000);
    }
    ImGui::End(); // End the main window

    bool showFavWindow = show_fav.load();
    if (showFavWindow) {
        ImGui::SetNextWindowPos(ImVec2(1100, 0));  // Adjust the position based on your UI
        ImGui::SetNextWindowSize(ImVec2(420, 420)); // Set the size of the popup window
        ImGui::Begin("Favourite Books", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(1.33f);

        if (!fav_books.empty()) {
            for (const auto& FavItem : fav_books) {
                ImGui::Text("%s", FavItem.first.c_str());
            }
        }
        else {
            ImGui::Text("No favourite books available.");
        }

        // Add some space before the button
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);  // Adjust this value to fine-tune the position
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 160);  // Adjust this value to fine-tune the position

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));   // Red background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); // Lighter red when hovered
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));  // Darker red when active
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Brighter white text

        if (ImGui::Button("Reset Favourite", ImVec2(150, 30))) {
            ImGui::OpenPopup("Confirm Reset");
        }

        ImGui::PopStyleColor(4);

        if (ImGui::BeginPopupModal("Confirm Reset", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete all favourites?");
            ImGui::Separator();

            if (ImGui::Button("Yes", ImVec2(120, 0))) {
                resetFavourite();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::End();
        show_fav.store(showFavWindow); // Store the updated value back to the atomic variable
    }
}
void BookUI::resetFavourite() {
    fs::path favDir = fs::current_path() / "favourite";

    try {
        if (fs::exists(favDir) && fs::is_directory(favDir)) {
            fs::remove_all(favDir);
            fs::create_directory(favDir);  // Recreate the folder to keep the structure intact
            fav_books.clear();  // Clear the in-memory list of favourites
            showNotificationMessage("All favourites have been reset.", 3000);
        }
        else {
            showNotificationMessage("No favourites directory found.", 3000);
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error resetting favourites: " << e.what() << std::endl;
        showNotificationMessage("Failed to reset favourites.", 5000);
    }
}

void BookUI::showNotificationMessage(const std::string& message, int duration_ms) {
    notificationMessage = message;
    notificationStartTime = std::chrono::steady_clock::now();
    showNotification = true;
}

std::string sanitizeFilename(const std::string& filename) {
    std::string sanitized = filename;
    std::replace(sanitized.begin(), sanitized.end(), ' ', '_');
    sanitized.erase(std::remove_if(sanitized.begin(), sanitized.end(),
        [](char c) { return !std::isalnum(c) && c != '_'; }),
        sanitized.end());
    return sanitized;
}

void BookUI::saveBookDetailsAsFavourite() {
    std::string bookTitle = selectedBook;

    if (bookTitle.empty()) {
        showNotificationMessage("No book selected.", 3000);
        return;
    }

    fs::path favDir = fs::current_path() / "favourite";
    fs::path favFile = favDir / (sanitizeFilename(bookTitle) + ".json");

    if (!fs::exists(favDir)) {
        fs::create_directory(favDir);
    }

    if (fs::exists(favFile)) {
        showNotificationMessage("This book is already in your favourites.", 3000);
        return;
    }

    nlohmann::json bookJson;
    bookJson["title"] = bookTitle;
    bookJson["details"] = manager.getBookDetails();
    bookJson["json_data"] = manager.getRawJsonData();

    std::ofstream file(favFile);
    if (file.is_open()) {
        file << bookJson.dump(4, '\t');
        file.close();
        showNotificationMessage("The Book has been added to your favourites list.", 3000);
    }
    else {
        showNotificationMessage("Failed to save book.", 5000);
    }
}

void BookUI::renderNotificationSection() {
    if (showNotification) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red text color
        ImGui::TextWrapped("%s", notificationMessage.c_str());
        ImGui::PopStyleColor();

        if (std::chrono::steady_clock::now() - notificationStartTime > std::chrono::milliseconds(3000)) {
            showNotification = false;
        }
    }
}

void BookUI::DisplayBooks(const std::vector<std::pair<std::string, std::string>>& books) {
    // Define a fixed width for buttons
    float buttonWidth = ImGui::GetContentRegionAvail().x; // Adjust if needed

    for (const auto& book : books) {
        ImGui::PushID(book.first.c_str());

        // Set the button color style (optional, if you want custom styling)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black background
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));    // White text

        // Render the button with a fixed width
        if (ImGui::Button(book.first.c_str(), ImVec2(buttonWidth, 0))) {
            if (selectedBook != book.first) {
                selectedBook = book.first;  // Set the selected book
                selectedBookIsbn = book.second;
                manager.selectBook(selectedBook);
                manager.fetchBookDetails(selectedBookIsbn); // Fetch details for the selected book
                manager.addSearchToHistory(book.first, book.second);
                showBookDetails = true; // Show details for the selected book
            }
            else {
                showBookDetails = !showBookDetails; // Toggle visibility if the same book is clicked
            }
        }

        // Restore previous style
        ImGui::PopStyleColor(2); // Pop both the Button and Text style colors

        ImGui::PopID();
    }
}
