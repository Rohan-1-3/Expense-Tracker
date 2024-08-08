#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <windows.h>
using namespace std;

void textAnimation(const char* str, int delay = 10) {
    int i = 0;
    while (str[i] != '\0') {
        std::cout << str[i];
        i++;
        Sleep(delay); 
    }
}

class LoadingScreen {
public:
    void showLoadingBar() {
        const int barWidth = 50;
        cout << "Loading: [";
        for (int i = 0; i < barWidth; ++i) {
            cout << " ";
        }
        cout << "]\rLoading: [";
        cout.flush();

        for (int i = 0; i < barWidth; ++i) {
            // this_thread::sleep_for(chrono::milliseconds(40));
            cout << "#";
            cout.flush();
        }
        cout << "]" << endl;
    }

   void showIntro() {
        const char* introText =
            "\n\n"
            "  ______                                  _______             _             \n"
            " |  ____|                                |__   __|           | |            \n"
            " | |__  __  ___ __   ___ _ __  ___  ___     | |_ __ __ _  ___| | _____ _ __ \n"
            " |  __| \\ \\/ / '_ \\ / _ \\ '_ \\/ __|/ _ \\    | | '__/ _` |/ __| |/ / _ \\ '__|\n"
            " | |____ >  <| |_) |  __/ | | \\__ \\  __/    | | | | (_| | (__|   <  __/ |   \n"
            " |______/_/\\_\\ .__/ \\___|_| |_|___/\\___|    |_|_|  \\__,_|\\___|_|\\_\\___|_|   \n"
            "             | |                                                            \n"
            "             |_|                                                            \n"
            "\n"
            "                              Rohan Shrestha\n"
            "                              Miken Baniya\n"
            "==========================================================================\n";

        textAnimation(introText, 10);
        cout << endl;
    }
};

class Expenses {
public:
    enum Category {
        FOOD,
        SOCIAL_LIFE,
        TRANSPORT,
        APPAREL,
        EDUCATION,
        HEALTH,
        OTHER
    };

private:
    string date;
    float amount;
    Category category;
    string note;
    string currency;

public:
    string getCurrentDate() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

public:
    Expenses() : date(""), amount(0), category(OTHER), note(""), currency("Rs") {}

    void setCategory(Category cat) {
        category = cat;
    }

    string getCategoryName() const {
        switch (category) {
            case FOOD: return "Food";
            case SOCIAL_LIFE: return "Social Life";
            case TRANSPORT: return "Transport";
            case APPAREL: return "Apparel";
            case EDUCATION: return "Education";
            case HEALTH: return "Health";
            case OTHER: return "Other";
            default: return "Unknown";
        }
    }

    void setDate(string temp = "") {
        if (temp != "") {
            date = temp;
            return;
        }
        string input;
        cout << "Enter the date (YYYY-MM-DD) or press Enter to use today's date: ";
        getline(cin, input);

        if (input.empty()) {
            date = getCurrentDate();
        } else {
            date = input;
        }
    }

    void setAmount(float temp = 0) {
        if (temp != 0) {
            amount = temp;
            return;
        }
        cout << "Enter the amount: ";
        while (!(cin >> amount)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid amount: ";
        }
        cin.ignore(); // To ignore the newline character left in the buffer
    }

    void setCategory() {
        int choice;
        cout << "Select a category:" << endl;
        cout << "1. Food" << endl;
        cout << "2. Social Life" << endl;
        cout << "3. Transport" << endl;
        cout << "4. Apparel" << endl;
        cout << "5. Education" << endl;
        cout << "6. Health" << endl;
        cout << "7. Other" << endl;
        cout << "Enter your choice (1-7): ";
        while (!(cin >> choice) || choice < 1 || choice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 7: ";
        }
        cin.ignore(); // To ignore the newline character left in the buffer

        switch (choice) {
            case 1: setCategory(FOOD); break;
            case 2: setCategory(SOCIAL_LIFE); break;
            case 3: setCategory(TRANSPORT); break;
            case 4: setCategory(APPAREL); break;
            case 5: setCategory(EDUCATION); break;
            case 6: setCategory(HEALTH); break;
            case 7: setCategory(OTHER); break;
            default: setCategory(OTHER); break; // Default to OTHER if invalid input
        }
    }

    void setNote(string temp = "") {
        if (temp != "") {
            note = temp;
            return;
        }
        cout << "Enter any notes: ";
        getline(cin, note);
    }

    void displayDetails() const {
        cout << "Date: " << (date.empty() ? "Not provided" : date) << endl;
        cout << "Amount: " << currency << " " << amount << endl;
        cout << "Category: " << getCategoryName() << endl;
        cout << "Note: " << note << endl;
    }

    float getAmount() const {
        return amount;
    }

    string getNote() const {
        return note;
    }

    string getCurrrency() const {
        return currency;
    }

    void saveToFile(ofstream &outFile) const {
        size_t dateSize = date.size();
        size_t noteSize = note.size();
        outFile.write(reinterpret_cast<const char*>(&dateSize), sizeof(dateSize));
        outFile.write(date.c_str(), dateSize);
        outFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        outFile.write(reinterpret_cast<const char*>(&category), sizeof(category));
        outFile.write(reinterpret_cast<const char*>(&noteSize), sizeof(noteSize));
        outFile.write(note.c_str(), noteSize);
    }

    void loadFromFile(ifstream &inFile) {
        size_t dateSize;
        size_t noteSize;
        inFile.read(reinterpret_cast<char*>(&dateSize), sizeof(dateSize));
        date.resize(dateSize);
        inFile.read(&date[0], dateSize);
        inFile.read(reinterpret_cast<char*>(&amount), sizeof(amount));
        inFile.read(reinterpret_cast<char*>(&category), sizeof(category));
        inFile.read(reinterpret_cast<char*>(&noteSize), sizeof(noteSize));
        note.resize(noteSize);
        inFile.read(&note[0], noteSize);
    }
};

class ExpenseManager {
private:
    static const int MAX_EXPENSES = 100;
    Expenses expenses[MAX_EXPENSES];
    int expenseCount;

    void saveToFile() const {
        ofstream outFile("expenses.dat", ios::binary);
        if (!outFile) {
            throw runtime_error("Failed to open expenses.dat for writing.");
        }
        outFile.write(reinterpret_cast<const char*>(&expenseCount), sizeof(expenseCount));
        for (int i = 0; i < expenseCount; ++i) {
            expenses[i].saveToFile(outFile);
        }
    }

    void loadFromFile() {
        ifstream inFile("expenses.dat", ios::binary);
        if (inFile.is_open()) {
            inFile.read(reinterpret_cast<char*>(&expenseCount), sizeof(expenseCount));
            for (int i = 0; i < expenseCount; ++i) {
                expenses[i].loadFromFile(inFile);
            }
        }
    }

public:
    ExpenseManager() : expenseCount(0) {
        try {
            loadFromFile();
        } catch (const exception &e) {
            cout << "Error loading from file: " << e.what() << endl;
        }
    }

    ~ExpenseManager() {
        try {
            saveToFile();
        } catch (const exception &e) {
            cout << "Error saving to file: " << e.what() << endl;
        }
    }

    void addExpense() {
        if (expenseCount < MAX_EXPENSES) {
            try {
                Expenses exp;
                exp.setDate();
                exp.setAmount();
                exp.setCategory();
                exp.setNote();
                expenses[expenseCount++] = exp;
                cout << "==========================================================================" << endl;
                cout << "Expense added successfully." << endl;
            } catch (const exception &e) {
                cout << "Error adding expense: " << e.what() << endl;
            }
        } else {
            cout << "Expense limit reached. Cannot add more expenses." << endl;
        }
        cout << "==========================================================================" << endl;
    }

    void showAllExpenses() const {
        if (expenseCount == 0) {
            cout << "No expenses to display." << endl;
            return;
        }

        cout << "+----+------------+--------+--------------+---------------------+" << endl;
        cout << "| No | Date       | Amount | Category     | Note                |" << endl;
        cout << "+----+------------+--------+--------------+---------------------+" << endl;

        for (int i = 0; i < expenseCount; ++i) {
            cout << "| " << setw(2) << i + 1 << " | "
                 << setw(10) << expenses[i].getCurrentDate() << " | "
                 << setw(6) << expenses[i].getAmount() << " | "
                 << setw(12) << expenses[i].getCategoryName() << " | "
                 << setw(19) << expenses[i].getNote() << " |" << endl;
        }

        cout << "+----+------------+--------+--------------+---------------------+" << endl;
    }

    void showExpense(int index) const {
        if (index >= 0 && index < expenseCount) {
            cout << "\nExpense " << (index + 1) << ":" << endl;
            expenses[index].displayDetails();
        } else {
            cout << "Invalid expense index." << endl;
        }
    }

    void updateExpense(int index) {
        cout << "==========================================================================" << endl;
        if (index >= 0 && index < expenseCount) {
            Expenses &exp = expenses[index];
            string input;
            float inputFloat;
            int choice;

            cout << "Updating Expense " << (index + 1) << endl;

            cout << "\nCurrent Details:" << endl;
            exp.displayDetails();

            cout << "\nEnter the new date (YYYY-MM-DD) or press Enter to keep " << exp.getCurrentDate() << ": ";
            getline(cin, input);
            if (!input.empty()) {
                exp.setDate(input);
            }

            cout << "Enter the new amount or press Enter to keep " << exp.getCurrrency() << " " << exp.getAmount() << ": ";
            getline(cin, input);
            if (!input.empty()) {
                inputFloat = stof(input);
                exp.setAmount(inputFloat);
            }

            cout << "Select a new category (1-7) or press Enter to keep " << exp.getCategoryName() << ": " << endl;
            cout << "1. Food" << endl;
            cout << "2. Social Life" << endl;
            cout << "3. Transport" << endl;
            cout << "4. Apparel" << endl;
            cout << "5. Education" << endl;
            cout << "6. Health" << endl;
            cout << "7. Other" << endl;
            cout << "Enter your choice: ";
            getline(cin, input);
            if (!input.empty()) {
                choice = stoi(input);
                switch (choice) {
                    case 1: exp.setCategory(Expenses::FOOD); break;
                    case 2: exp.setCategory(Expenses::SOCIAL_LIFE); break;
                    case 3: exp.setCategory(Expenses::TRANSPORT); break;
                    case 4: exp.setCategory(Expenses::APPAREL); break;
                    case 5: exp.setCategory(Expenses::EDUCATION); break;
                    case 6: exp.setCategory(Expenses::HEALTH); break;
                    case 7: exp.setCategory(Expenses::OTHER); break;
                    default: cout << "Invalid choice. Keeping the previous category." << endl; break;
                }
            }

            cout << "Enter the new note or press Enter to keep \"" << exp.getNote() << "\": ";
            getline(cin, input);
            if (!input.empty()) {
                exp.setNote(input);
            }
            cout << "==========================================================================" << endl;
            cout << "Expense updated successfully." << endl;
        } else {
            cout << "Invalid expense index." << endl;
        }
        cout << "==========================================================================" << endl;
    }

    void removeExpense(int index) {
        if (index >= 0 && index < expenseCount) {
            for (int i = index; i < expenseCount - 1; ++i) {
                expenses[i] = expenses[i + 1];
            }
            --expenseCount;
            cout << "==========================================================================" << endl;
            cout << "Expense removed successfully." << endl;
        } else {
            cout << "Invalid expense index." << endl;
        }
        cout << "==========================================================================" << endl;
    }

    void displayOptions() const {
        cout << "==========================================================================" << endl;
        cout << "+----+----------------------------+" << endl;
        cout << "| SN | Option                     |" << endl;
        cout << "+----+----------------------------+" << endl;
        cout << "| 1  | Add Expenses               |" << endl;
        cout << "| 2  | Show All Expenses          |" << endl;
        cout << "| 3  | Show An Expense            |" << endl;
        cout << "| 4  | Update an Expense          |" << endl;
        cout << "| 5  | Remove an Expense          |" << endl;
        cout << "| 6  | Exit                       |" << endl;
        cout << "+----+----------------------------+" << endl;
        cout << "==========================================================================" << endl;
    }

    bool checkOptions(int ch) const {
        return ch >= 1 && ch <= 6;
    }

    void run() {
        int ch;
        LoadingScreen ls;
        ls.showLoadingBar();
        // this_thread::sleep_for(chrono::seconds(1));
        ls.showIntro();
        // this_thread::sleep_for(chrono::seconds(1));

        do {
            displayOptions();
            cout << "Enter a choice: ";
            while (!(cin >> ch) || !checkOptions(ch)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid option (1-6): ";
            }
            cin.ignore(); // To ignore the newline character left in the buffer

            try {
                switch (ch) {
                    case 1: addExpense(); break;
                    case 2: showAllExpenses(); break;
                    case 3: {
                        int index;
                        cout << "Enter expense number to show: ";
                        cin >> index;
                        cin.ignore();
                        showExpense(index - 1);
                        break;
                    }
                    case 4: {
                        int index;
                        cout << "Enter expense number to update: ";
                        cin >> index;
                        cin.ignore();
                        updateExpense(index - 1);
                        break;
                    }
                    case 5: {
                        int index;
                        cout << "Enter expense number to remove: ";
                        cin >> index;
                        cin.ignore();
                        removeExpense(index - 1);
                        break;
                    }
                    case 6: cout << "Exiting..." << endl; break;
                    default: cout << "Invalid option. Please try again." << endl; break;
                }
            } catch (const exception &e) {
                cout << "Error: " << e.what() << endl;
            }
        } while (ch != 6);
    }
};

int main() {
    try {
        ExpenseManager tracker;
        tracker.run();
    } catch (const exception &e) {
        cout << "An error occurred: " << e.what() << endl;
    }
    return 0;
}
