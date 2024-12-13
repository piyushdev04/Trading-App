#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

// Structure for storing stock data
struct Stock {
    string symbol;
    string name;
    double price;
};

// Structure for storing transaction history
struct Transaction {
    string symbol;
    string type; // "buy" or "sell"
    int quantity;
    double price;
    double total;
};

// Function to load stock data from CSV
void loadStockData(const string& filename, map<string, Stock>& stocks) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string symbol, name, priceStr;
        double price;

        getline(ss, symbol, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        price = stod(priceStr);

        stocks[symbol] = {symbol, name, price};
    }
    file.close();
}

// Function to display stock information
void displayStocks(const map<string, Stock>& stocks) {
    cout << "Available Stocks: \n";
    for (const auto& pair : stocks) {
        const Stock& stock = pair.second;
        cout << stock.symbol << " (" << stock.name << ") - $" << stock.price << endl;
    }
}

// Function to display transaction history
void displayHistory(const vector<Transaction>& history) {
    cout << "\nTransaction History:\n";
    for (const auto& transaction : history) {
        cout << transaction.type << " " << transaction.symbol << " - Quantity: " << transaction.quantity
             << " at $" << transaction.price << " - Total: $" << transaction.total << endl;
    }
}

// Function to handle buying stocks
void buyStock(map<string, Stock>& stocks, vector<Transaction>& history, double& balance) {
    string symbol;
    int quantity;

    cout << "Enter stock symbol to buy: ";
    cin >> symbol;

    if (stocks.find(symbol) == stocks.end()) {
        cout << "Stock not found!" << endl;
        return;
    }

    cout << "Enter quantity to buy: ";
    cin >> quantity;

    double totalCost = stocks[symbol].price * quantity;

    if (balance >= totalCost) {
        balance -= totalCost;
        history.push_back({"buy", symbol, quantity, stocks[symbol].price, totalCost});
        cout << "Bought " << quantity << " of " << symbol << " for $" << totalCost << endl;
    } else {
        cout << "Insufficient balance!" << endl;
    }
}

// Function to handle selling stocks
void sellStock(map<string, Stock>& stocks, vector<Transaction>& history, double& balance) {
    string symbol;
    int quantity;

    cout << "Enter stock symbol to sell: ";
    cin >> symbol;

    if (stocks.find(symbol) == stocks.end()) {
        cout << "Stock not found!" << endl;
        return;
    }

    cout << "Enter quantity to sell: ";
    cin >> quantity;

    double totalCost = stocks[symbol].price * quantity;

    balance += totalCost;
    history.push_back({"sell", symbol, quantity, stocks[symbol].price, totalCost});
    cout << "Sold " << quantity << " of " << symbol << " for $" << totalCost << endl;
}

// Function to calculate and display profit/loss
void calculateProfitLoss(const vector<Transaction>& history) {
    double totalProfitLoss = 0;

    for (const auto& transaction : history) {
        if (transaction.type == "buy") {
            totalProfitLoss -= transaction.total;
        } else if (transaction.type == "sell") {
            totalProfitLoss += transaction.total;
        }
    }

    cout << "\nTotal Profit/Loss: $" << totalProfitLoss << endl;
}

int main() {
    map<string, Stock> stocks;
    vector<Transaction> history;

    double balance;

    // Ask the user to enter their initial balance
    cout << "Enter your initial balance: $";
    cin >> balance;

    // Load stock data from the CSV file
    loadStockData("stocks.csv", stocks);

    int choice;
    do {
        cout << "\nTrading App Menu\n";
        cout << "1. View Stocks\n";
        cout << "2. Buy Stock\n";
        cout << "3. Sell Stock\n";
        cout << "4. View History\n";
        cout << "5. Calculate Profit/Loss\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayStocks(stocks);
                break;
            case 2:
                buyStock(stocks, history, balance);
                break;
            case 3:
                sellStock(stocks, history, balance);
                break;
            case 4:
                displayHistory(history);
                break;
            case 5:
                calculateProfitLoss(history);
                break;
            case 6:
                cout << "Exiting the application.\n";
                break;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
