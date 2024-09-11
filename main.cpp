#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>


using namespace std;


struct Contact 
{
    string name;
    string number;
    string email;
};


const int MAX_CONTACTS = 1000;
Contact contacts[MAX_CONTACTS];
int numContacts = 0;

//Function Prototypes
void addContact();
void displayContacts();
void searchContacts();
void modifyContact();
void deleteContact();
void loadFromFile();
void saveToFile();
bool isValidPhoneNumber(const string &number);
bool isValidEmail(const string &email);
void clearInputBuffer();

void addContact()
 {
    if (numContacts >= MAX_CONTACTS)
     {
        cout << "Phone book is full. Cannot add more contacts." << endl;
        return;
    }
    
    Contact newContact;

    cout << "Enter contact name: ";
    getline(cin >> ws, newContact.name);
    
    cout << "Enter phone number: ";
    string phone;
    getline(cin, phone);
    while (!isValidPhoneNumber(phone)) 
    {
        cout << "Invalid phone number. Please enter again: ";
        getline(cin, phone);
    }
    newContact.number = phone;

    cout << "Enter email: ";
    string email;
    getline(cin, email);
    while (!isValidEmail(email)) 
    {
        cout << "Invalid email. Please enter again: ";
        getline(cin, email);
    }
    newContact.email = email;

    contacts[numContacts++] = newContact;
    cout << "Contact added successfully." << endl;
}


void displayContacts()
 {
    if (numContacts == 0)
     {
        cout << "Phone book is empty." << endl;
        return;
    }
    
    cout << left << setw(20) << "Name" << setw(20) << "Number" << setw(30) << "Email" << endl;
    cout << string(70, '-') << endl;
    
    for (int i = 0; i < numContacts; i++)
     {
        cout << left << setw(20) << contacts[i].name
             << setw(20) << contacts[i].number
             << setw(30) << contacts[i].email << endl;
    }
}


void searchContacts()
 {
    string query;
    bool found = false;
    
    cout << "Enter name or number to search: ";
    getline(cin >> ws, query);
    
    cout << left << setw(20) << "Name" << setw(20) << "Number" << setw(30) << "Email" << endl;
    cout << string(70, '-') << endl;
    
    for (int i = 0; i < numContacts; i++) 
    {
        if (contacts[i].name.find(query) != string::npos ||
            contacts[i].number.find(query) != string::npos) 
        {
            cout << left << setw(20) << contacts[i].name
                 << setw(20) << contacts[i].number
                 << setw(30) << contacts[i].email << endl;
            found = true;
        }
    }
    
    if (!found) 
    {
        cout << "No contacts found matching the query." << endl;
    }
}


void modifyContact() {
    string name;
    bool found = false;
    
    cout << "Enter name of contact to modify: ";
    getline(cin >> ws, name);
    
    for (int i = 0; i < numContacts; i++)
     {
        if (contacts[i].name == name)
         {
            cout << "Current contact details:" << endl;
            cout << "Name: " << contacts[i].name << endl;
            cout << "Number: " << contacts[i].number << endl;
            cout << "Email: " << contacts[i].email << endl;

            cout << "Enter new name (leave blank to keep current): ";
            string newName;
            getline(cin >> ws, newName);
            if (!newName.empty()) contacts[i].name = newName;

            cout << "Enter new number (leave blank to keep current): ";
            string newNumber;
            getline(cin, newNumber);
            if (!newNumber.empty() && isValidPhoneNumber(newNumber)) contacts[i].number = newNumber;

            cout << "Enter new email (leave blank to keep current): ";
            string newEmail;
            getline(cin, newEmail);
            if (!newEmail.empty() && isValidEmail(newEmail)) contacts[i].email = newEmail;

            cout << "Contact modified successfully." << endl;
            found = true;
            break;
        }
    }
    
    if (!found) 
    {
        cout << "No contact found with the given name." << endl;
    }
}


void deleteContact()
 {
    string name;
    bool found = false;
    
    cout << "Enter name of contact to delete: ";
    getline(cin >> ws, name);
    
    for (int i = 0; i < numContacts; i++) 
    {
        if (contacts[i].name == name) {
            for (int j = i; j < numContacts - 1; j++)
             {
                contacts[j] = contacts[j + 1];
            }
            numContacts--;
            cout << "Contact deleted successfully." << endl;
            found = true;
            break;
        }
    }
    
    if (!found) 
    {
        cout << "No contact found with the given name." << endl;
    }
}


void loadFromFile() {
    ifstream file("contacts.txt");
    
    if (file.is_open()) 
    {
        file >> numContacts;
        file.ignore(numeric_limits<streamsize>::max(), '\n'); 
        
        for (int i = 0; i < numContacts; i++) 
        {
            getline(file, contacts[i].name);
            getline(file, contacts[i].number);
            getline(file, contacts[i].email);
        }
        
        file.close();
        cout << "Contacts loaded from file." << endl;
    } 
    else {
        cout << "Unable to open file." << endl;
    }
}


void saveToFile() {
    ofstream file("contacts.txt");
    
    if (file.is_open()) 
    {
        file << numContacts << endl;
        
        for (int i = 0; i < numContacts; i++) 
        {
            file << contacts[i].name << endl;
            file << contacts[i].number << endl;
            file << contacts[i].email << endl;
        }
        
        file.close();
        cout << "Contacts saved to file." << endl;
    }
     
     else {
        cout << "Unable to open file." << endl;
    }
}


bool isValidPhoneNumber(const string &number)
 {
    // Check if the phone number contains only digits and has a length of 10
    return (number.length() == 10 && all_of(number.begin(), number.end(), ::isdigit));
}


bool isValidEmail(const string &email) 
{
    // Simple email validation (not comprehensive)
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos && dotPos > atPos);
}


void clearInputBuffer() 
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


int main() {

    int choice;
    
    loadFromFile();
    
    while (true) {
        cout << "\nPhone Book Management System" << endl;
        cout << "1. Add Contact" << endl;
        cout << "2. Display Contacts" << endl;
        cout << "3. Search Contacts" << endl;
        cout << "4. Modify Contact" << endl;
        cout << "5. Delete Contact" << endl;
        cout << "6. Save and Exit" << endl;
        cout << "Enter your choice: ";
        
        while (!(cin >> choice))
         {
            cout << "Invalid input. Please enter a number between 1 and 6: ";
            clearInputBuffer();
        }
        
        switch (choice) 
        {
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
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}