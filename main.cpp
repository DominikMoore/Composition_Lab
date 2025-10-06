//Dominik Moore
//6 October 2025
//Data Structures
//Inheritance + Composition
//This program will make a sample banking system and create polymorphic account types. Users will be able to
// create, deposit, and withdraw money from their accounts, with transaction history tracking added.
//added the transaction history

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>

class Transaction {
public:
    std::string type;       // this is the deposit or witdraw
    double amount;          // Amount changed during transaction
    std::string timestamp;  // keeps track of the time

    Transaction(const std::string& t, double a)
        : type(t), amount(a)
    {
        // keeps track of the time
        time_t now = time(0);
        timestamp = ctime(&now);
        if (!timestamp.empty() && timestamp.back() == '\n') timestamp.pop_back(); // gets rid of the newline area and or part
    }

    void printTransaction() const {
        std::cout << "(" << timestamp << ") " << type << " of $" << amount << std::endl;
    }
};

class BankAccount {
private:
    std::string* accountNumber;
    std::string* accountHolderName;
    double balance;
    std::vector<Transaction> transactions;  //holds transaction history

public:
    //Constructors
    BankAccount() {
        accountNumber = new std::string("000000");
        accountHolderName = new std::string("?");
        balance = 0.0;
    }

    BankAccount(const std::string& accNum, const std::string& name, double initialBalance) {
        accountNumber = new std::string(accNum);
        accountHolderName = new std::string(name);
        balance = initialBalance;
    }

    //Rule of Three
    BankAccount(const BankAccount& other) {
        accountNumber = new std::string(*other.accountNumber);
        accountHolderName = new std::string(*other.accountHolderName);
        balance = other.balance;
        transactions = other.transactions;
    }

    BankAccount& operator=(const BankAccount& other) {
        if (this != &other) {
            delete accountNumber;
            delete accountHolderName;
            accountNumber = new std::string(*other.accountNumber);
            accountHolderName = new std::string(*other.accountHolderName);
            balance = other.balance;
            transactions = other.transactions;
        }
        return *this;
    }

    virtual ~BankAccount() {
        delete accountNumber;
        delete accountHolderName;
    }

    //Getters
    std::string getAccountNumber() const { return *accountNumber; }
    std::string getAccountHolderName() const { return *accountHolderName; }
    double getBalance() const { return balance; }

    //Mutators
    void setAccountHolderName(const std::string& newName) { *accountHolderName = newName; }

    //Deposit & Withdraw
    void deposit(double amount) { *this += amount; }

    virtual void withdraw(double amount) { *this -= amount; }

    //Operator Overloads
    BankAccount& operator+=(double amount) {
        if (amount > 0) {
            balance += amount;
            transactions.emplace_back("Deposit", amount);
        } else {
            std::cout << "Deposit must be positive." << std::endl;
        }
        return *this;
    }

    BankAccount& operator-=(double amount) {
        if (amount <= 0) std::cout << "Withdrawal must be positive." << std::endl;
        else if (amount > balance) std::cout << "Insufficient funds." << std::endl;
        else {
            balance -= amount;
            transactions.emplace_back("Withdrawal", amount);
        }
        return *this;
    }

    //Comparison Operators
    bool operator==(const BankAccount& other) const { return balance == other.balance; }
    bool operator<(const BankAccount& other) const { return balance < other.balance; }
    bool operator>(const BankAccount& other) const { return balance > other.balance; }

    //Static Utility
    static void printAccount(const BankAccount& account) {
        std::cout << "Account Number: " << account.getAccountNumber()
                  << "\nHolder: " << account.getAccountHolderName()
                  << "\nBalance: $" << account.getBalance() << std::endl;
    }

    static BankAccount createAccountFromInput() {
        std::string accNum, holder;
        double initialBalance;
        std::cout << "Enter Account Number: ";
        std::cin >> accNum;
        std::cin.ignore();
        std::cout << "Enter Account Holder Name: ";
        std::getline(std::cin, holder);
        std::cout << "Enter Initial Balance: ";
        std::cin >> initialBalance;
        return BankAccount(accNum, holder, initialBalance);
    }

    //Print Transaction History
    void printTransactionHistory() const {
        std::cout << "\n--- Transaction History for " << *accountHolderName << " ---" << std::endl;
        if (transactions.empty()) {
            std::cout << "No transactions recorded." << std::endl;
        } else {
            for (const auto& t : transactions)
                t.printTransaction();
        }
        std::cout << "----------------------------------------" << std::endl;
    }
};

//checkings
class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(const std::string& accNum, const std::string& name, double initialBalance, double fee)
        : BankAccount(accNum, name, initialBalance), transactionFee(fee) {}

    void withdraw(double amount) override {
        BankAccount::withdraw(amount + transactionFee);
        std::cout << "Transaction fee of $" << transactionFee << " applied." << std::endl;
    }
};

//svings
class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& accNum, const std::string& name, double initialBalance, double rate)
        : BankAccount(accNum, name, initialBalance), interestRate(rate) {}

    void calculateInterest() {
        double interest = getBalance() * interestRate;
        deposit(interest);
        std::cout << "Interest of $" << interest << " added." << std::endl;
    }
};

//main
int main() {
    std::vector<std::unique_ptr<BankAccount>> accounts;
    int input;

    do {
        std::cout << std::endl << "*********** DPZ BANKING ***********" << std::endl;
        std::cout << "1. Create Checking Account" << std::endl;
        std::cout << "2. Create Savings Account" << std::endl;
        std::cout << "3. Deposit" << std::endl;
        std::cout << "4. Withdraw" << std::endl;
        std::cout << "5. Calculate Interest (Savings Only)" << std::endl;
        std::cout << "6. Print Accounts" << std::endl;
        std::cout << "7. Show Transaction History" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "Please Pick an Option: ";
        std::cin >> input;

        switch (input) {
        case 1: {
            std::string accNum, holder;
            double balance, fee;
            std::cout << "Enter Account Number: "; std::cin >> accNum; std::cin.ignore();
            std::cout << "Enter Account Holder Name: "; std::getline(std::cin, holder);
            std::cout << "Enter Initial Balance: "; std::cin >> balance;
            std::cout << "Enter Transaction Fee: "; std::cin >> fee;
            accounts.push_back(std::make_unique<CheckingAccount>(accNum, holder, balance, fee));
            std::cout << "Checking account created!" << std::endl;
            break;
        }
        case 2: {
            std::string accNum, holder;
            double balance, rate;
            std::cout << "Enter Account Number: "; std::cin >> accNum; std::cin.ignore();
            std::cout << "Enter Account Holder Name: "; std::getline(std::cin, holder);
            std::cout << "Enter Initial Balance: "; std::cin >> balance;
            std::cout << "Enter Interest Rate in decimal: "; std::cin >> rate;
            accounts.push_back(std::make_unique<SavingsAccount>(accNum, holder, balance, rate));
            std::cout << "Savings account created!" << std::endl;
            break;
        }
        case 3: {
            if (accounts.empty()) { std::cout << "No accounts exist." << std::endl; break; }
            std::string accNum; double amount;
            std::cout << "Enter Account Number: "; std::cin >> accNum;
            bool found = false;
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    std::cout << "Enter Amount to Deposit: "; std::cin >> amount;
                    acc->deposit(amount);
                    BankAccount::printAccount(*acc);
                    found = true;
                    break;
                }
            }
            if (!found) std::cout << "Account not found." << std::endl;
            break;
        }
        case 4: {
            if (accounts.empty()) { std::cout << "No accounts exist." << std::endl; break; }
            std::string accNum; double amount;
            std::cout << "Enter Account Number: "; std::cin >> accNum;
            bool found = false;
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    std::cout << "Enter Amount to Withdraw: "; std::cin >> amount;
                    acc->withdraw(amount);
                    BankAccount::printAccount(*acc);
                    found = true;
                    break;
                }
            }
            if (!found) std::cout << "Account not found." << std::endl;
            break;
        }
        case 5: {
            for (auto& acc : accounts) {
                if (auto sav = dynamic_cast<SavingsAccount*>(acc.get())) {
                    sav->calculateInterest();
                }
            }
            break;
        }
        case 6: {
            for (auto& acc : accounts) {
                BankAccount::printAccount(*acc);
                std::cout << "*************" << std::endl;
            }
            break;
        }
        case 7: {
            if (accounts.empty()) { std::cout << "No accounts exist." << std::endl; break; }
            std::string accNum;
            std::cout << "Enter Account Number: "; std::cin >> accNum;
            bool found = false;
            for (auto& acc : accounts) {
                if (acc->getAccountNumber() == accNum) {
                    acc->printTransactionHistory();
                    found = true;
                    break;
                }
            }
            if (!found) std::cout << "Account not found." << std::endl;
            break;
        }
        case 8:
            std::cout << "Exiting DPZ Banking. Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid input." << std::endl;
        }

    } while (input != 8);

    return 0;
}
