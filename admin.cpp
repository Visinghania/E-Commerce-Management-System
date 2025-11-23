#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <queue> // ADDED: For Priority Queue (Min-Heap)

using namespace std;

// ================= CONSTANTS & PATHS =================
const string BASE_PATH = "C:\\Users\\vitth\\Desktop\\E-Commerce\\";
const string CREDENTIALS_FILE = "admin_credentials.txt";
const string STOCK_FILE = "stock.txt";
const string CUSTOMERS_FILE = "customers.txt";
const string DELIVERY_FILE = "delivery_orders.txt";
const string TAKEAWAY_FILE = "takeaway_orders.txt";

// Product names must match main.cpp to ensure consistency
string PRODUCT_NAMES[11] = {
    "", 
    "Sony FX30", 
    "Sony FR7", 
    "Canon EOS 90D",
    "Nikon D6", 
    "Panasonic Lumix G10",
    "Canon EOS 250D",
    "Sony Alpha 7", 
    "Nikon D750", 
    "Panasonic Lumix GH5",
    "Canon EOS 5D Mark"
};

// ================= DATA STRUCTURES FOR ALERTS =================

// Node to store in the Priority Queue
struct StockNode {
    int id;
    string name;
    int quantity;
};

// Comparator for Min-Heap (Smallest quantity stays at top)
struct CompareStock {
    bool operator()(StockNode const& p1, StockNode const& p2) {
        return p1.quantity > p2.quantity; // Returns true if p1 > p2, causing swap (Min-Heap logic)
    }
};

// ================= UTILITY FUNCTIONS =================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Helper to split string by delimiter
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// ================= AUTHENTICATION =================

void generateDefaultCredentials() {
    string fullPath = BASE_PATH + CREDENTIALS_FILE;
    ifstream checkFile(fullPath);
    
    // Only create if it doesn't exist
    if (!checkFile.good()) {
        ofstream outFile(fullPath);
        if (outFile.is_open()) {
            outFile << "admin" << endl;     // Default ID
            outFile << "admin123" << endl;  // Default Password
            cout << ">> Generated default admin credentials file." << endl;
            outFile.close();
        } else {
            cout << "Error: Could not create credentials file at " << fullPath << endl;
        }
    }
    checkFile.close();
}

bool login() {
    string fullPath = BASE_PATH + CREDENTIALS_FILE;
    ifstream file(fullPath);
    
    string savedId, savedPass;
    if (file.is_open()) {
        getline(file, savedId);
        getline(file, savedPass);
        file.close();
    } else {
        cout << "Error: Credentials file missing. Restart the application." << endl;
        return false;
    }

    string inputId, inputPass;
    
    cout << "\n========== ADMIN LOGIN ==========" << endl;
    cout << "Enter Admin ID: ";
    cin >> inputId;
    cout << "Enter Password: ";
    cin >> inputPass;

    if (inputId == savedId && inputPass == savedPass) {
        return true;
    }
    return false;
}

// ================= FEATURES =================

// DSA FEATURE: MIN-HEAP (Priority Queue)
void viewLowStockAlerts() {
    clearScreen();
    cout << "\n========== LOW STOCK ALERTS (Min-Heap) ==========" << endl;

    // Priority Queue to store stock items, ordered by lowest quantity first
    priority_queue<StockNode, vector<StockNode>, CompareStock> minHeap;

    ifstream inFile(BASE_PATH + STOCK_FILE);
    string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            if (data.size() >= 2) {
                try {
                    int id = stoi(data[0]);
                    int qty = stoi(data[1]);
                    if (id >= 1 && id <= 10) {
                        // Push to heap (O(log N) complexity)
                        StockNode node = {id, PRODUCT_NAMES[id], qty};
                        minHeap.push(node);
                    }
                } catch(...) {}
            }
        }
        inFile.close();
    }

    // Threshold for alert
    const int THRESHOLD = 10;
    bool foundCritical = false;

    cout << left << setw(10) << "ID" << setw(25) << "Product Name" << "Remaining Qty" << endl;
    cout << "--------------------------------------------------------" << endl;

    // Extract elements from heap (O(log N) per extraction)
    // We only care about items where quantity < THRESHOLD
    while (!minHeap.empty()) {
        StockNode current = minHeap.top();
        minHeap.pop();

        if (current.quantity < THRESHOLD) {
            foundCritical = true;
            // 
            cout << left << setw(10) << current.id 
                 << setw(25) << current.name 
                 << " [CRITICAL: " << current.quantity << "]" << endl;
        } else {
            // Since it's a Min-Heap, if the current item is safe (>= 10), 
            // all remaining items in the heap are also safe. We can stop.
            break; 
        }
    }

    if (!foundCritical) {
        cout << ">> No critical stock alerts. All items healthy." << endl;
    }

    cout << "--------------------------------------------------------" << endl;
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

void manageStock() {
    clearScreen();
    cout << "\n========== STOCK MANAGEMENT ==========" << endl;
    
    int currentStock[11] = {0}; // Default 0
    
    // 1. Load current stock
    ifstream inFile(BASE_PATH + STOCK_FILE);
    string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            if (data.size() >= 2) {
                try {
                    int id = stoi(data[0]);
                    int qty = stoi(data[1]);
                    if (id >= 1 && id <= 10) currentStock[id] = qty;
                } catch(...) {}
            }
        }
        inFile.close();
    }

    // 2. Display Stock
    cout << left << setw(10) << "ID" << setw(25) << "Product Name" << "Stock Qty" << endl;
    cout << "------------------------------------------------" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << left << setw(10) << i << setw(25) << PRODUCT_NAMES[i] << currentStock[i] << endl;
    }
    cout << "------------------------------------------------" << endl;

    // 3. Update Stock
    int prodId, newQty;
    cout << "\nEnter Product ID to update (0 to cancel): ";
    cin >> prodId;
    
    if (prodId >= 1 && prodId <= 10) {
        cout << "Enter New Stock Quantity for " << PRODUCT_NAMES[prodId] << ": ";
        cin >> newQty;
        currentStock[prodId] = newQty;

        // 4. Save back to file
        ofstream outFile(BASE_PATH + STOCK_FILE);
        if (outFile.is_open()) {
            for (int i = 1; i <= 10; i++) {
                outFile << i << "|" << currentStock[i] << endl;
            }
            outFile.close();
            cout << "\n>> Stock updated successfully!" << endl;
        } else {
            cout << "Error: Could not save stock file." << endl;
        }
    }
    
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

void viewPendingOrders() {
    clearScreen();
    cout << "\n========== PENDING ORDERS ==========" << endl;
    
    // 1. Delivery Orders
    cout << "\n--- Pending Delivery Orders ---" << endl;
    ifstream delFile(BASE_PATH + DELIVERY_FILE);
    string line;
    bool hasOrders = false;
    
    if (delFile.is_open()) {
        getline(delFile, line); // Skip header
        while (getline(delFile, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            if (data.size() >= 4) {
                cout << "[Delivery] Customer: " << data[0] << " | Item: " << data[1] << " | Qty: " << data[2] << " | Bill: " << data[3] << endl;
                hasOrders = true;
            }
        }
        delFile.close();
    }
    if (!hasOrders) cout << "No pending delivery orders." << endl;

    // 2. Takeaway Orders
    cout << "\n--- Pending Warehouse Pickup Orders ---" << endl;
    ifstream takeFile(BASE_PATH + TAKEAWAY_FILE);
    hasOrders = false;
    
    if (takeFile.is_open()) {
        getline(takeFile, line); // Skip header
        while (getline(takeFile, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            if (data.size() >= 5) {
                cout << "[ID: " << data[0] << "] Customer: " << data[1] << " | Item: " << data[2] << " | Qty: " << data[3] << endl;
                hasOrders = true;
            }
        }
        takeFile.close();
    }
    if (!hasOrders) cout << "No pending pickup orders." << endl;

    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

void viewPastOrders() {
    // We treat "Past Orders" as the history stored inside the Customers file
    clearScreen();
    cout << "\n========== PAST ORDER HISTORY (By Customer) ==========" << endl;
    
    ifstream file(BASE_PATH + CUSTOMERS_FILE);
    string line;
    
    if (!file.is_open()) {
        cout << "No customer records found." << endl;
    } else {
        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            
            if (data.size() >= 8) {
                string name = data[1];
                int historyCount = stoi(data[8]);
                
                if (historyCount > 0) {
                    cout << "\nCustomer: " << name << " (Total Orders: " << historyCount << ")" << endl;
                    int tokenIndex = 9;
                    for(int i=0; i<historyCount; i++) {
                        // Ensure we don't go out of bounds
                        if(tokenIndex + 3 < data.size()) {
                            cout << "   - " << data[tokenIndex+1] // Product
                                 << " (Qty: " << data[tokenIndex+2] << ")" // Qty
                                 << " Bill: Rs." << data[tokenIndex+3] << endl; // Bill
                            tokenIndex += 4; // Move to next order group
                        }
                    }
                }
            }
        }
        file.close();
    }
    
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

void customerManagement() {
    clearScreen();
    cout << "\n========== REGISTERED CUSTOMERS ==========" << endl;
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(25) << "Email" << setw(15) << "Phone" << "Points" << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    
    ifstream file(BASE_PATH + CUSTOMERS_FILE);
    string line;
    
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> data = split(line, '|');
            if (data.size() >= 7) {
                cout << left << setw(10) << data[0] 
                     << setw(20) << data[1] 
                     << setw(25) << data[2] 
                     << setw(15) << data[3] 
                     << data[6] << endl;
            }
        }
        file.close();
    } else {
        cout << "No customers found." << endl;
    }
    
    cout << "\nPress Enter to return...";
    cin.ignore();
    cin.get();
}

// ================= MAIN =================

int main() {
    generateDefaultCredentials();

    if (!login()) {
        cout << "Login Failed. Exiting..." << endl;
        return 0;
    }

    int choice;
    do {
        clearScreen();
        cout << "\n========== ADMIN DASHBOARD ==========" << endl;
        cout << "1. Stock Management (Add/Update)" << endl;
        cout << "2. View Past Orders (Customer History)" << endl;
        cout << "3. View Pending Orders" << endl;
        cout << "4. Customer Management" << endl;
        cout << "5. Low Stock Alerts (Min-Heap)" << endl; // Added New Option
        cout << "0. Exit" << endl;
        cout << "=====================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: manageStock(); break;
            case 2: viewPastOrders(); break;
            case 3: viewPendingOrders(); break;
            case 4: customerManagement(); break;
            case 5: viewLowStockAlerts(); break; // Call New Function
            case 0: cout << "Exiting Admin Panel..." << endl; break;
            default: cout << "Invalid choice." << endl; break;
        }
    } while (choice != 0);

    return 0;
}