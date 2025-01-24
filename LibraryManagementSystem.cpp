#include <iostream>
using namespace std;

class Book {
    string title, author, ISBN;
    int count;

public:
    Book(string t = "", string a = "", string num = "", int c = 0)
        : title(t), author(a), ISBN(num), count(c) {
    }

    int getcount() const {
        return count;
    }

    void setcount(int k) {
        count = k;
    }

    string gettitle() const {
        return title;
    }

    string getauthor() const {
        return author;
    }

    string getISBN() const {
        return ISBN;
    }

    void print() {
        cout << "Book details :" << endl;
        cout << "Title:" << title << endl;
        cout << "Author :" << author << endl;
        cout << "ISBN : " << ISBN << endl;
        cout << "count :" << count << endl;
    }

    ~Book() {}
};

class Member {
    string name;
    Book* books_checked_out;
    int id;
    int numofbookscheckedout;

public:
    Member(string n = "", int i = 0)
        : name(n), id(i), numofbookscheckedout(0) {
        books_checked_out = new Book[10];
    }

    void setnobookcheckout(int count) {
        numofbookscheckedout = count;
    }

    void setbooks_checked_out(Book* newBooks) {
        books_checked_out = newBooks;
    }

    Book* getbooks_checked_out() {
        return books_checked_out;
    }

    int getnobookcheckout() const {
        return numofbookscheckedout;
    }

    string getname() const {
        return name;
    }

    int getID() const {
        return id;
    }

    Member& operator=(const Member& other) {
        if (this != &other) {
            name = other.name;
            id = other.id;
            numofbookscheckedout = other.numofbookscheckedout;

            delete[] books_checked_out;

            books_checked_out = new Book[numofbookscheckedout];
            for (int i = 0; i < numofbookscheckedout; ++i) {
                books_checked_out[i] = other.books_checked_out[i];
            }
        }
        return *this;
    }

    ~Member() {
        if (books_checked_out != nullptr) {
            delete[] books_checked_out;
            books_checked_out = nullptr;
        }
    }

    void show() {
        cout << "Member details :" << endl;
        cout << "Name:" << name << endl;
        cout << "ID : " << id << endl;
    }

};

class Library {
    Book* books;
    Member* members;
    int numbook;
    int numMem;

public:
    Library()
    {
        members = nullptr;
        books = nullptr;
        numbook = 0;
        numMem = 0;
    }


    void addbooks(const Book& obj) {

        for (int i = 0; i < numbook; i++) {
            if (books[i].getISBN() == obj.getISBN()) {
                cout << "Error: A book with ISBN " << obj.getISBN() << " already exists." << endl;
                return;
            }
        }


        Book* newbooks = new Book[numbook + 1];
        for (int i = 0; i < numbook; i++) {
            newbooks[i] = books[i];
        }
        newbooks[numbook] = obj;
        delete[] books;
        books = newbooks;
        numbook++;
    }

    void printbooks() {
        for (int i = 0; i < numbook; i++) {
            cout << "Book details:" << endl;
            cout << "Title: " << books[i].gettitle() << endl;
            cout << "Author: " << books[i].getauthor() << endl;
            cout << "ISBN: " << books[i].getISBN() << endl;
            cout << "Count :" << books[i].getcount() << endl;
            cout << endl;
        }
    }

    void addmembers(const Member& ob) {
        Member* newMem = new Member[numMem + 1];
        for (int i = 0; i < numMem; i++) {
            newMem[i] = members[i];
        }
        newMem[numMem] = ob;
        delete[] members;
        members = newMem;
        numMem++;
    }

    void checkoutBook(int memberId, string ISBN, int decrementCount) {
        Member* member = nullptr;
        for (int i = 0; i < numMem; i++) {
            if (members[i].getID() == memberId) {
                member = &members[i];
                break;
            }
        }

        Book* book = nullptr;
        for (int i = 0; i < numbook; i++) {
            if (books[i].getISBN() == ISBN) {
                book = &books[i];
                break;
            }
        }

        if (member && book) {
            int maxCheckoutLimit = book->getcount();
            if (member->getnobookcheckout() >= maxCheckoutLimit) {
                cout << "Maximum limit of checked-out books reached for the member." << endl;
            }
            else if (book->getcount() < decrementCount) {
                cout << "The requested number of books is not available." << endl;
            }
            else {
                Book* checkedOutBooks = new Book[member->getnobookcheckout() + decrementCount];
                for (int i = 0; i < member->getnobookcheckout(); ++i) {
                    checkedOutBooks[i] = member->getbooks_checked_out()[i];
                }
                for (int i = 0; i < decrementCount; ++i) {
                    checkedOutBooks[member->getnobookcheckout() + i] = *book;
                }

                delete[] member->getbooks_checked_out();
                member->setbooks_checked_out(checkedOutBooks);
                member->setnobookcheckout(member->getnobookcheckout() + decrementCount);

                book->setcount(book->getcount() - decrementCount);

                cout << "Books checked out successfully." << endl;
            }
        }
        else {
            cout << "Invalid member ID or book ISBN." << endl;
        }
    }


    void returnBook(int memberId, string ISBN, int returnCount) {
        Member* member = nullptr;
        for (int i = 0; i < numMem; i++) {
            if (members[i].getID() == memberId) {
                member = &members[i];
                break;
            }
        }

        Book* book = nullptr;
        for (int i = 0; i < numbook; i++) {
            if (books[i].getISBN() == ISBN) {
                book = &books[i];
                break;
            }
        }

        if (member && book) {
            int borrowedCount = 0;
            for (int i = 0; i < member->getnobookcheckout(); ++i) {
                if (member->getbooks_checked_out()[i].getISBN() == ISBN) {
                    borrowedCount++;
                }
            }

            if (borrowedCount < returnCount) {
                cout << "Error: The member did not borrow " << returnCount << " books with ISBN " << ISBN << "." << endl;
                return;
            }

            Book* updatedCheckedOutBooks = new Book[member->getnobookcheckout() - returnCount];
            int updatedCount = 0;

            cout << "Returned books by " << member->getname() << " (ID: " << memberId << "):" << endl;

            for (int i = 0; i < member->getnobookcheckout(); ++i) {
                if (member->getbooks_checked_out()[i].getISBN() == ISBN && updatedCount < returnCount) {
                    cout << "Title: " << member->getbooks_checked_out()[i].gettitle() << endl;
                    cout << "Author: " << member->getbooks_checked_out()[i].getauthor() << endl;
                    cout << "ISBN: " << member->getbooks_checked_out()[i].getISBN() << endl;
                    cout << endl;
                    updatedCount++;
                }
                else {
                    updatedCheckedOutBooks[i - updatedCount] = member->getbooks_checked_out()[i];
                }
            }

            delete[] member->getbooks_checked_out();

            member->setbooks_checked_out(updatedCheckedOutBooks);
            member->setnobookcheckout(member->getnobookcheckout() - returnCount);

            book->setcount(book->getcount() + returnCount);

            cout << "Books returned successfully." << endl;
        }
        else {
            cout << "Invalid member ID or book ISBN." << endl;
        }
    }


    void deletebooks(string isbn) {
        for (int i = 0; i < numbook; i++) {
            if (books[i].getISBN() == isbn) {
                for (int j = i; j < numbook - 1; j++) {
                    books[j] = books[j + 1];
                }
                numbook--;
                cout << "Book with ISBN " << isbn << " removed from the library." << endl;
                cout << "New count is " << numbook << endl;
                return;
            }
        }
        cout << "Book with ISBN " << isbn << " not found in the library." << endl;
    }

    void deletemembers(int id) {
        for (int i = 0; i < numMem; i++) {
            if (members[i].getID() == id) {
                for (int j = i; j < numMem - 1; j++) {
                    members[j] = members[j + 1];
                }
                numMem--;
                cout << "Member with ID " << id << " removed from the library." << endl;
                return;
            }
        }
        cout << "Member with ID " << id << " not found in the library." << endl;
    }

    void searchBook() {
        string title;
        cout << "Enter book title: ";
        cin.ignore();
        cin >> title;

        bool found = false;
        for (int i = 0; i < numbook; i++) {
            if (books[i].gettitle() == title) {
                books[i].print();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Book not found." << endl;
        }
    }

    void searchMember() {
        int memberId;
        cout << "Enter member ID: ";
        cin >> memberId;

        bool found = false;
        for (int i = 0; i < numMem; i++) {
            if (members[i].getID() == memberId) {
                members[i].show();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Member not found." << endl;
        }
    }

    void displayCheckedOutBooks(int memberId) const {
        for (int i = 0; i < numMem; ++i) {
            if (members[i].getID() == memberId) {
                cout << "Books checked out by " << members[i].getname() << " (ID: " << memberId << "):" << endl;
                Book* checkedOutBooks = members[i].getbooks_checked_out();
                for (int j = 0; j < members[i].getnobookcheckout(); ++j) {
                    cout << "Title: " << checkedOutBooks[j].gettitle() << endl;
                    cout << "Author: " << checkedOutBooks[j].getauthor() << endl;
                    cout << "ISBN: " << checkedOutBooks[j].getISBN() << endl;
                    cout << endl;
                }
                return;
            }
        }
        cout << "Member not found." << endl;
    }

    void printmembers() {
        for (int i = 0; i < numMem; i++) {
            cout << "Member details :" << endl;
            cout << "Name:" << members[i].getname() << endl;
            cout << "ID : " << members[i].getID() << endl;
            cout << endl;
        }
    }

    ~Library() {
        if (books != nullptr) {
            delete[] books;
            books = nullptr;
        }
        if (members != nullptr) {
            delete[] members;
            members = nullptr;
        }
    }

    int getNumBooks() const {
        return numbook;
    }

    int getNumMembers() const {
        return numMem;
    }


};

int main() {
    cout << "\n\n********  WELCOME TO E-LIBRARY  ******** \n\n" << endl;
    Library l1;

    bool run = true;
    while (run) {
        cout << "Please enter the number corresponding to your desired functionality:" << endl;
        cout << "1. Add a book" << endl;
        cout << "2. Print all books" << endl;
        cout << "3. Add a member" << endl;
        cout << "4. Print all members" << endl;
        cout << "5. Delete a book" << endl;
        cout << "6. Delete a Member" << endl;
        cout << "7. Checkout Book" << endl;
        cout << "8. Display Checkout Books " << endl;
        cout << "9. Search Book " << endl;
        cout << "10. Search Member " << endl;
        cout << "11. Return and Print a Book" << endl;
        cout << "0. Quit" << endl;
        int choice;
        cout << "Enter Choice: ";
        cin >> choice;
        if (choice == 0) {
            run = false;
        }
        else if (choice == 1) {
            int no;
            cout << "Enter the number of Books you want to add: ";
            cin >> no;
            while (no <= 0) {
                cout << "Invalid input. Please enter a valid positive integer: ";
                cin >> no;
            }
            for (int i = 0; i < no; i++) {
                cout << "Enter the book details:" << endl;
                string title, author, isbn;
                int count;
                cin.ignore();
                cout << "Title: ";
                cin >> title;
                cout << "Author: ";
                cin >> author;
                cout << "ISBN: ";
                cin >> isbn;
                cout << "Count: ";
                cin >> count;
                while (count <= 0) {
                    cout << "Invalid input. Please enter a valid positive integer: ";
                    cin >> count;
                }
                l1.addbooks(Book(title, author, isbn, count));
            }
        }
        else if (choice == 2) {
            l1.printbooks();
        }
        else if (choice == 3) {
            int no;
            cout << "Enter the number of Members you want to add: ";
            cin >> no;
            while (no <= 0) {
                cout << "Invalid input. Please enter a valid positive integer: ";
                cin >> no;
            }
            string name;
            int id;
            for (int i = 0; i < no; i++) {
                cout << "Enter the Member details:" << endl;
                cout << "Name: ";
                cin >> name;
                cout << "ID: ";
                cin >> id;
                while (id <= 0) {
                    cout << "Invalid input. Please enter a valid positive integer : ";
                    cin >> id;
                }

                l1.addmembers(Member(name, id));
            }
        }
        else if (choice == 4) {
            l1.printmembers();
        }
        else if (choice == 5) {
            string isbn;
            cout << "Enter the ISBN of the book you want to delete: ";
            cin >> isbn;
            l1.deletebooks(isbn);
        }
        else if (choice == 6) {
            int id;
            cout << "Enter the ID of the member you want to delete: ";
            cin >> id;
            while (id <= 0) {
                cout << "Invalid input. Please enter a valid positive integer : ";
                cin >> id;
            }

            l1.deletemembers(id);
        }
        else if (choice == 7) {
            int memberId, count;
            string ISBN;
            cout << "Enter the Member ID: ";
            cin >> memberId;
            cout << "Enter the ISBN of the book: ";
            cin >> ISBN;
            cout << "Enter the count of the book: ";
            cin >> count;
            while (count <= 0) {
                cout << "Invalid input. Please enter a valid positive integer: ";
                cin >> count;
            }
            l1.checkoutBook(memberId, ISBN, count);
        }
        else if (choice == 8) {
            int memberId;
            cout << "Enter the Member ID: ";
            cin >> memberId;
            while (memberId <= 0) {
                cout << "Invalid input. Please enter a valid positive integer : ";
                cin >> memberId;
            }

            l1.displayCheckedOutBooks(memberId);
        }
        else if (choice == 9) {
            l1.searchBook();
        }
        else if (choice == 10) {
            l1.searchMember();
        }
        else if (choice == 11) {
            int memberId, count;
            string ISBN;
            cout << "Enter the Member ID: ";
            cin >> memberId;
            cout << "Enter the ISBN of the book: ";
            cin >> ISBN;
            cout << "Enter the count of the book: ";
            cin >> count;
            while (count <= 0) {
                cout << "Invalid input. Please enter a valid positive integer: ";
                cin >> count;
            }
            l1.returnBook(memberId, ISBN, count);
        }

        else {
            cout << "Invalid choice. Please enter a number between 0 and 11." << endl;
        }
    }

    return 0;
}
