#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class BankAccount {
private:
    int accountNo;
    string holder;
    int pin;
    double balance;
    vector<string> history;

public:
    BankAccount(int number, string name, int secretPin, double openingBalance)
        : accountNo(number), holder(name), pin(secretPin), balance(openingBalance) {}

    int getAccountNo() const { return accountNo; }
    string getHolder() const { return holder; }
    double getBalance() const { return balance; }
    bool verifyPin(int enteredPin) const { return enteredPin == pin; }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Amount must be greater than zero.\n";
            return;
        }
        balance += amount;
        history.push_back("Deposit: +" + to_string(amount));
        cout << "Deposit successful.\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Amount must be greater than zero.\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        history.push_back("ATM Withdrawal: -" + to_string(amount));
        cout << "Withdrawal successful.\n";
        return true;
    }

    bool transferTo(BankAccount& receiver, double amount) {
        if (amount <= 0 || amount > balance || receiver.accountNo == accountNo) {
            cout << "Invalid transfer.\n";
            return false;
        }
        balance -= amount;
        receiver.balance += amount;
        history.push_back("Transfer to " + to_string(receiver.accountNo) +
                          ": -" + to_string(amount));
        receiver.history.push_back("Transfer from " + to_string(accountNo) +
                                   ": +" + to_string(amount));
        cout << "Transfer successful.\n";
        return true;
    }

    bool changePin(int oldPin, int newPin) {
        if (!verifyPin(oldPin) || newPin < 1000 || newPin > 9999) {
            cout << "PIN change failed. Use a valid 4-digit PIN.\n";
            return false;
        }
        pin = newPin;
        cout << "PIN changed successfully.\n";
        return true;
    }

    void showBalance() const {
        cout << "Account: " << accountNo << " | Holder: " << holder
             << " | Balance: Rs. " << fixed << setprecision(2) << balance << '\n';
    }

    void showHistory() const {
        cout << "\n--- Transaction History ---\n";
        if (history.empty()) cout << "No transactions yet.\n";
        for (const string& item : history) cout << item << '\n';
    }
};

int main() {
    BankAccount accounts[] = {
        BankAccount(1001, "DemoUser", 1234, 5000.00),
        BankAccount(1002, "SampleUser", 5678, 3000.00)
    };
    const int accountCount = 2;
    int choice;

    do {
        cout << "\n=== CORE BANKING SYSTEM / ATM ===\n"
             << "1. Login and check balance\n"
             << "2. Deposit\n"
             << "3. ATM withdrawal\n"
             << "4. Transfer\n"
             << "5. Change ATM PIN\n"
             << "6. Transaction history\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;

        if (choice == 0) break;
        if (choice < 1 || choice > 6) {
            cout << "Invalid choice.\n";
            continue;
        }

        int accountNo, enteredPin;
        cout << "Account number (demo: 1001 or 1002): ";
        cin >> accountNo;
        int index = -1;
        for (int i = 0; i < accountCount; ++i)
            if (accounts[i].getAccountNo() == accountNo) index = i;

        if (index == -1) {
            cout << "Account not found.\n";
            continue;
        }

        cout << "ATM PIN: ";
        cin >> enteredPin;
        if (!accounts[index].verifyPin(enteredPin)) {
            cout << "Incorrect PIN.\n";
            continue;
        }

        if (choice == 1) accounts[index].showBalance();
        else if (choice == 2 || choice == 3) {
            double amount;
            cout << "Amount: Rs. ";
            cin >> amount;
            if (choice == 2) accounts[index].deposit(amount);
            else accounts[index].withdraw(amount);
        } else if (choice == 4) {
            int receiverNo;
            double amount;
            cout << "Receiver account number: ";
            cin >> receiverNo;
            cout << "Amount: Rs. ";
            cin >> amount;
            int receiverIndex = -1;
            for (int i = 0; i < accountCount; ++i)
                if (accounts[i].getAccountNo() == receiverNo) receiverIndex = i;
            if (receiverIndex == -1) cout << "Receiver account not found.\n";
            else accounts[index].transferTo(accounts[receiverIndex], amount);
        } else if (choice == 5) {
            int newPin;
            cout << "New 4-digit PIN: ";
            cin >> newPin;
            accounts[index].changePin(enteredPin, newPin);
        } else if (choice == 6) {
            accounts[index].showHistory();
        }
    } while (true);

    cout << "Thank you. Demo session ended.\n";
    return 0;
}
