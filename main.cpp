#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string RESET = "\033[0m";        
const string BG_BLUE = "\033[44m";    
const string BG_GREEN = "\033[42m";   
const string BG_RED = "\033[41m";     
const string BG_YELLOW = "\033[43m";  
const string BOLD = "\033[1m";      
struct Contact {
    string name;
    string number;
    string email;
    Contact* prev;  
    Contact* next; 
};
Contact* head = nullptr;
Contact* tail = nullptr;

void add();
void display();
void searchcontact();
void modifycontact();
void deletecontact();
void loadfromfile();
void savetofile();
bool isValidPhoneNumber(const string& number);
bool isValidEmail(const string& email);
void freeMemory();

void add() {
    Contact* newContact = new Contact(); 
    newContact->prev = nullptr;
    newContact->next = nullptr;
    cout << "Enter name: ";
    getline(cin >> ws, newContact->name);
    cout << "Enter phone number (10 digits): ";
    getline(cin, newContact->number);
    while (!isValidPhoneNumber(newContact->number)) {
        cout << "Invalid phone number. Please enter again: ";
        getline(cin, newContact->number);
    }
    cout << "Enter email: ";
    getline(cin, newContact->email);
    while (!isValidEmail(newContact->email)) {
        cout << "Invalid email. Please enter again: ";
        getline(cin, newContact->email);
    }
    if (head == nullptr) {
        head = tail = newContact;
    } else {
        tail->next = newContact;
        newContact->prev = tail;
        tail = newContact;
    }
    cout << BG_GREEN << "Contact added successfully!\n" << RESET;
}
void display() {
    if (head == nullptr) {
        cout << "No contacts found.\n";
        return;
    }
    Contact* current = head;
    cout << "\nContacts:\n";
    while (current != nullptr) {
        cout << "Name: " << current->name << endl;
        cout << "Phone: " << current->number << endl;
        cout << "Email: " << current->email << endl;
        cout << "------------------\n";
        current = current->next;
    }
}
void searchcontacts() {
    if (head == nullptr) {
        cout << "No contacts found.\n";
        return;
    }
    string query;
    cout << "Enter name or phone number to search: ";
    getline(cin >> ws, query);
    Contact* current = head;
    bool found = false;
    while (current != nullptr) {
        if (current->name == query || current->number == query) {
            cout << "Contact Found:\n";
            cout << "Name: " << current->name << endl;
            cout << "Phone: " << current->number << endl;
            cout << "Email: " << current->email << endl;
            found = true;
            break;
        }
        current = current->next;
    }
    if (!found) {
        cout << "No contact found matching the query.\n";
    }
}
void modifycontact() {
    if (head == nullptr) {
        cout << "No contacts found.\n";
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
        cout << "No contact found with the given name.\n";
        return;
    }
    cout << "Current details:\n";
    cout << "Name: " << current->name << endl;
    cout << "Phone: " << current->number << endl;
    cout << "Email: " << current->email << endl;
    cout << "Enter new name (leave blank to keep current): ";
    string newName;
    getline(cin >> ws, newName);
    if (!newName.empty()) current->name = newName;
    cout << "Enter new phone number (leave blank to keep current): ";
    string newNumber;
    getline(cin, newNumber);
    if (!newNumber.empty() && isValidPhoneNumber(newNumber)) current->number = newNumber;
    cout << "Enter new email (leave blank to keep current): ";
    string newEmail;
    getline(cin, newEmail);
    if (!newEmail.empty() && isValidEmail(newEmail)) current->email = newEmail;
    cout << "Contact updated successfully.\n";
}
void deletecontact() {
    if (head == nullptr) {
        cout << "No contacts found.\n";
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
        cout << BG_RED << "No contact found with the given name.\n" << RESET;
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
    cout << "Contact deleted successfully.\n";
}
void loadfromfile() {
    ifstream file("contacts.rtf");
    if (!file.is_open()) {
        cout << "No saved contacts found. Starting fresh.\n";
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
}
void savetofile() {
    ofstream file("contacts.txt");
    if (!file.is_open()) {
        cout << "Error saving contacts.\n";
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
    cout << "Contacts saved.\n";
}
bool isValidPhoneNumber(const string& number) {
    return number.length() == 10 && all_of(number.begin(), number.end(), ::isdigit);
}
bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return atPos != string::npos && dotPos != string::npos && dotPos > atPos;
}
void freeMemory() {
    while (head != nullptr) {
        Contact* toDelete = head;
        head = head->next;
        delete toDelete;
    }
}
int main() {
    loadfromfile();
    int choice;
    while (true) {
       cout << BOLD << BG_BLUE << "\nPhone Book Management System" << RESET << endl;
       cout << BG_GREEN << "1. Add Contact" << RESET << endl;
cout << BG_GREEN << "2. Display Contacts" << RESET << endl;
cout << BG_GREEN << "3. Search Contacts" << RESET << endl;
cout << BG_GREEN << "4. Modify Contact" << RESET << endl;
cout << BG_GREEN << "5. Delete Contact" << RESET << endl;
cout << BG_RED << "6. Save and Exit" << RESET << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: add(); break;
            case 2: display(); break;
            case 3: searchcontacts(); break;
            case 4: modifycontact(); break;
            case 5: deletecontact(); break;
            case 6:
                savetofile();
                freeMemory();
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
