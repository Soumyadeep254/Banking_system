#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;
#define MIN_BALANCE 1000

class InsufficientFunds {};

class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }

    void deposit(float amount);
    void withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream& operator<<(ofstream& ofs, Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
    friend ostream& operator<<(ostream& os, Account& acc);
};

long Account::NextAccountNumber = 0;

class Bank {
private:
    map<long, Account> accounts;

public:
    Bank();
    Account openAccount(string fname, string lname, float balance);
    Account balanceEnquiry(long accountNumber);
    Account deposit(long accountNumber, float amount);
    Account withdraw(long accountNumber, float amount);
    void closeAccount(long accountNumber);
    void showAllAccounts();
    ~Bank();
};

Account::Account(string fname, string lname, float balance) {
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::deposit(float amount) {
    balance += amount;
}

void Account::withdraw(float amount) {
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber) {
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return NextAccountNumber;
}

ofstream& operator<<(ofstream& ofs, Account& acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc) {
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream& operator<<(ostream& os, Account& acc) {
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccNo() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

Bank::Bank() {
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile) {
        // cout<<"Error in Opening! File Not Found!!"<<endl;
        return;
    }
    while (!infile.eof()) {
        infile >> account;
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccountNumber(account.getAccNo());
    infile.close();
}

Account Bank::openAccount(string fname, string lname, float balance) {
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));

    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceEnquiry(long accountNumber) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    return itr->second;
}

Account Bank::deposit(long accountNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accountNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr->second.withdraw(amount);
    return itr->second;
}

void Bank::closeAccount(long accountNumber) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    cout << "Account Deleted: " << itr->second;
    accounts.erase(accountNumber);
}

void Bank::showAllAccounts() {
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        cout << "Account " << itr->first << endl << itr->second << endl;
    }
}

Bank::~Bank() {
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr->second;
    }
    outfile.close();
}

int main() {
    Bank b;
    Account acc;

    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "***Banking System***" << endl;
    do {
        cout << "\n\tSelect one option below ";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter First Name: ";
                cin >> fname;
                cout << "Enter Last Name: ";
                cin >> lname;
                cout << "Enter initial Balance: ";
                cin >> balance;
                acc = b.openAccount(fname, lname, balance);
                cout << endl << "Congratulations! Account is Created" << endl;
                cout << acc;
                break;

            case 2:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                acc = b.balanceEnquiry(accountNumber);
                cout << endl << "Your Account Details" << endl;
                cout << acc;
                break;

            case 3:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = b.deposit(accountNumber, amount);
                cout << endl << "Amount Deposited" << endl;
                cout << acc;
                break;

            case 4:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = b.withdraw(accountNumber, amount);
                cout << endl << "Amount Withdrawn" << endl;
                cout << acc;
                break;

            case 5:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.closeAccount(accountNumber);
                cout << endl << "Account is Closed" << endl;
                break;

            case 6:
                b.showAllAccounts();
                break;

            case 7:
                break;

            default:
                cout << "\nEnter correct choice";
                exit(0);
        }
    } while (choice != 7);

    return 0;
}
