#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

class Database {
private:
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

public:
    Database(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host, user, password);
        con->setSchema(database);
    }

    void addBook(const std::string& title, const std::string& author, int year) {
        sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO books (name, author, year, status) VALUES (?, ?, ?, ?)");
        pstmt->setString(1, title);
        pstmt->setString(2, author);
        pstmt->setInt(3, year);
        pstmt->setInt(4, 1); // Assuming status 1 by default
        pstmt->execute();
        delete pstmt;
    }

    void deleteBook(int id) {
        sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM books WHERE id = ?");
        pstmt->setInt(1, id);
        pstmt->execute();
        delete pstmt;
    }

    void editBook(int id, const std::string& title, const std::string& author, int year) {
        sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE books SET name = ?, author = ?, year = ? WHERE id = ?");
        pstmt->setString(1, title);
        pstmt->setString(2, author);
        pstmt->setInt(3, year);
        pstmt->setInt(4, id);
        pstmt->execute();
        delete pstmt;
    }

    void viewBooks() {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM books");
        while (res->next()) {
            int id = res->getInt("id");
            std::string title = res->getString("name");
            std::string author = res->getString("author");
            int year = res->getInt("year");
            int status = res->getInt("status");

            std::cout << "ID: " << id << ", Title: " << title << ", Author: " << author << ", Year: " << year << ", Status: " << status << std::endl;
        }
        delete res;
        delete stmt;
    }
};
const int MAX_BOOKS = 100;

struct Book {
    std::string title;
    std::string author;
    int year;
};

class Library {
private:
    Book books[MAX_BOOKS];
    int bookCount;

public:
    Library() : bookCount(0) {}

    void addBook(const Book& book) {
        if (bookCount < MAX_BOOKS) {
            books[bookCount++] = book;
            std::cout << "Book added successfully." << std::endl;
        } else {
            std::cout << "Library is full. Cannot add more books." << std::endl;
        }
    }

    void viewBooks() {
        if (bookCount == 0) {
            std::cout << "Library is empty." << std::endl;
            return;
        }

        std::cout << "Books in the library:" << std::endl;
        for (int i = 0; i < bookCount; ++i) {
            std::cout << "Title: " << books[i].title << ", Author: " << books[i].author << ", Year: " << books[i].year << std::endl;
        }
    }

    void editBook(int index, const Book& book) {
        if (index >= 0 && index < bookCount) {
            books[index] = book;
            std::cout << "Book edited successfully." << std::endl;
        } else {
            std::cout << "Invalid book index." << std::endl;
        }
    }

    void deleteBook(int index) {
        if (index >= 0 && index < bookCount) {
            for (int i = index; i < bookCount - 1; ++i) {
                books[i] = books[i + 1];
            }
            bookCount--;
            std::cout << "Book deleted successfully." << std::endl;
        } else {
            std::cout << "Invalid book index." << std::endl;
        }
    }
};

int main() {
    Library library;
    int choice;

    while (true) {
        std::cout << "\nLibrary Management System\n";
        std::cout << "1. Add a Book\n";
        std::cout << "2. View Books\n";
        std::cout << "3. Edit a Book\n";
        std::cout << "4. Delete a Book\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Clear the input buffer
      if (std::cin.fail()) {
            std::cin.clear();  // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the input buffer
        }

        switch (choice) {
            case 1: {
                Book book;
                std::cout << "Enter book title: ";
                std::cin >> book.title;
                std::cout << "Enter author: ";
                std::cin >> book.author;
                std::cout << "Enter publication year: ";
                std::cin >> book.year;
                library.addBook(book);
                break;
            }
            case 2: {
                library.viewBooks();
                break;
            }
            case 3: {
                int index;
                std::cout << "Enter the index of the book to edit: ";
                std::cin >> index;
                Book book;
                std::cout << "Enter updated book title: ";
                std::cin >> book.title;
                std::cout << "Enter updated author: ";
                std::cin >> book.author;
                std::cout << "Enter updated publication year: ";
                std::cin >> book.year;
                library.editBook(index, book);
                break;
            }
            case 4: {
                int index;
                std::cout << "Enter the index of the book to delete: ";
                std::cin >> index;
                library.deleteBook(index);
                break;
            }
            case 5:
                return 0;
            default:
             std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    return 0;

int main() {
    // Initialize the database with your actual connection details
    Database db("localhost", "your_user", "your_password", "your_database");

    int choice;

    while (true) {
        // ... (your existing code remains the same)

        switch (choice) {
            case 1: {
                Book book;
                std::cout << "Enter book title: ";
                std::cin >> book.title;
                std::cout << "Enter author: ";
                std::cin >> book.author;
                std::cout << "Enter publication year: ";
                std::cin >> book.year;

                // Add the book to the database
                db.addBook(book.title, book.author, book.year);
                break;
            }
            case 2: {
                // View books from the database
                db.viewBooks();
                break;
            }
            case 3: {
                int index;
                std::cout << "Enter the index of the book to edit: ";
                std::cin >> index;
                Book book;
                std::cout << "Enter updated book title: ";
                std::cin >> book.title;
                std::cout << "Enter updated author: ";
                std::cin >> book.author;
                std::cout << "Enter updated publication year: ";
                std::cin >> book.year;

                // Edit the book in the database
                db.editBook(index, book.title, book.author, book.year);
                break;
            }
            case 4: {
                int index;
                std::cout << "Enter the index of the book to delete: ";
                std::cin >> index;

                // Delete the book from the database
                db.deleteBook(index);
                break;
            }
            case 5:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}
