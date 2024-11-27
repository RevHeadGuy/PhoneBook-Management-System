#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Structure for a contact
struct Contact {
    string name;
    string number;
    string email;
    Contact* prev;  // Pointer to the previous node
    Contact* next;  // Pointer to the next node
};

// Head and tail pointers for the doubly linked list
Contact* head = nullptr;
Contact* tail = nullptr;

// Function Prototypes
void addContact();
void displayContacts();
void searchContacts();
void modifyContact();
void deleteContact();
void loadFromFile();
void saveToFile();
bool isValidPhoneNumber(const string& number);
bool isValidEmail(const string& email);
void clearInputBuffer();
void freeMemory();

// Function to add a contact to the doubly linked list
void addContact() {
    Contact* newContact = new Contact();
    newContact->prev = nullptr;
    newContact->next = nullptr;

    cout << "Enter contact name: ";
    getline(cin >> ws, newContact->name);

    cout << "Enter phone number (10 digits): ";
    string phone;
    getline(cin, phone);
    while (!isValidPhoneNumber(phone)) {
        cout << "Invalid phone number. Please enter again: ";
        getline(cin, phone);
    }
    newContact->number = phone;

    cout << "Enter email (e.g., example@domain.com): ";
    string email;
    getline(cin, email);
    while (!isValidEmail(email)) {
        cout << "Invalid email. Please enter again: ";
        getline(cin, email);
    }
    newContact->email = email;

    if (head == nullptr) {
        head = tail = newContact;
    } else {
        tail->next = newContact;
        newContact->prev = tail;
        tail = newContact;
    }

    cout << "Contact added successfully." << endl;
}

// Function to display all contacts
void displayContacts() {
    if (head == nullptr) {
        cout << "Phone book is empty." << endl;
        return;
    }

    cout << left << setw(20) << "Name" << setw(20) << "Number" << setw(30) << "Email" << endl;
    cout << string(70, '-') << endl;

    Contact* current = head;
    while (current != nullptr) {
        cout << left << setw(20) << current->name
             << setw(20) << current->number
             << setw(30) << current->email << endl;
        current = current->next;
    }
}

// Function to search for a contact
void searchContacts() {
    if (head == nullptr) {
        cout << "Phone book is empty." << endl;
        return;
    }

    string query;
    cout << "Enter name or number to search: ";
    getline(cin >> ws, query);

    Contact* current = head;
    bool found = false;

    cout << left << setw(20) << "Name" << setw(20) << "Number" << setw(30) << "Email" << endl;
    cout << string(70, '-') << endl;

    while (current != nullptr) {
        if (current->name.find(query) != string::npos || current->number.find(query) != string::npos) {
            cout << left << setw(20) << current->name
                 << setw(20) << current->number
                 << setw(30) << current->email << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No contacts found matching the query." << endl;
    }
}

// Function to modify a contact
void modifyContact() {
    if (head == nullptr) {
        cout << "Phone book is empty." << endl;
        return;
    }

    string name;
    cout << "Enter the name of the contact to modify: ";
    getline(cin >> ws, name);

    Contact* current = head;
    while (current != nullptr && current->name != name) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "No contact found with the given name." << endl;
        return;
    }

    cout << "Current contact details:" << endl;
    cout << "Name: " << current->name << endl;
    cout << "Number: " << current->number << endl;
    cout << "Email: " << current->email << endl;

    cout << "Enter new name (leave blank to keep current): ";
    string newName;
    getline(cin >> ws, newName);
    if (!newName.empty()) current->name = newName;

    cout << "Enter new number (leave blank to keep current): ";
    string newNumber;
    getline(cin, newNumber);
    if (!newNumber.empty() && isValidPhoneNumber(newNumber)) current->number = newNumber;

    cout << "Enter new email (leave blank to keep current): ";
    string newEmail;
    getline(cin, newEmail);
    if (!newEmail.empty() && isValidEmail(newEmail)) current->email = newEmail;

    cout << "Contact modified successfully." << endl;
}

// Function to delete a contact
void deleteContact() {
    if (head == nullptr) {
        cout << "Phone book is empty." << endl;
        return;
    }

    string name;
    cout << "Enter the name of the contact to delete: ";
    getline(cin >> ws, name);

    Contact* current = head;
    while (current != nullptr && current->name != name) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << "No contact found with the given name." << endl;
        return;
    }

    if (current == head) {
        head = current->next;
        if (head != nullptr) head->prev = nullptr;
    } else {
        current->prev->next = current->next;
    }

    if (current == tail) {
        tail = current->prev;
        if (tail != nullptr) tail->next = nullptr;
    }

    delete current;
    cout << "Contact deleted successfully." << endl;
}

// Function to load contacts from a file
void loadFromFile() {
    ifstream file("contacts.txt");
    if (!file.is_open()) {
        cout << "Unable to open file. Starting with an empty phone book." << endl;
        return;
    }

    while (!file.eof()) {
        Contact* newContact = new Contact();
        getline(file, newContact->name);
        if (newContact->name.empty()) {
            delete newContact;
            break;
        }
        getline(file, newContact->number);
        getline(file, newContact->email);
        newContact->prev = nullptr;
        newContact->next = nullptr;

        if (head == nullptr) {
            head = tail = newContact;
        } else {
            tail->next = newContact;
            newContact->prev = tail;
            tail = newContact;
        }
    }
    file.close();
    cout << "Contacts loaded from file." << endl;
}

// Function to save contacts to a file
void saveToFile() {
    ofstream file("contacts.txt");
    if (!file.is_open()) {
        cout << "Unable to open file for saving." << endl;
        return;
    }

    Contact* current = head;
    while (current != nullptr) {
        file << current->name << endl;
        file << current->number << endl;
        file << current->email << endl;
        current = current->next;
    }
    file.close();
    cout << "Contacts saved to file." << endl;
}

// Function to validate phone numbers
bool isValidPhoneNumber(const string& number) {
    return number.length() == 10 && all_of(number.begin(), number.end(), ::isdigit);
}

// Function to validate email addresses
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return atPos != string::npos && dotPos != string::npos && dotPos > atPos;
}

// Function to clear the input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to free dynamically allocated memory
void freeMemory() {
    Contact* current = head;
    while (current != nullptr) {
        Contact* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

// Main function
int main() {
    loadFromFile();

    int choice;
    while (true) {
        cout << "\nPhone Book Management System\n";
        cout << "1. Add Contact\n";
        cout << "2. Display Contacts\n";
        cout << "3. Search Contacts\n";
        cout << "4. Modify Contact\n";
        cout << "5. Delete Contact\n";
        cout << "6. Save and Exit\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 6: ";
            clearInputBuffer();
        }

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                displayContacts();
                break;
            case 3:
                searchContacts();
                break;
            case 4:
                modifyContact();
                break;
            case 5:
                deleteContact();
                break;
            case 6:
                saveToFile();
                freeMemory();
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}
