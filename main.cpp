//-------------------Online Shopping Store(Ecommerce Store)-----------------------------
//--------------------------------------------------------------------------------------


#include <iostream>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#define MAX_SIZE 50
#define V 11
#define V1 6

using namespace std;

// Function to clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

struct user
{
    string name;
    string product;
    int quantity;
    double bill;

    user() {}

    user(string name, string product, int quantity, double bill)
    {
        this->name = name;
        this->product = product;
        this->quantity = quantity;
        this->bill = bill;
    }
};

// Customer Management System
struct Customer
{
    string customerId;
    string name;
    string email;
    string phone;
    string address;
    string password;  // Password for authentication
    int loyaltyPoints;
    double pendingDiscount;  // Store discount from redeemed points
    vector<user> orderHistory;
    bool isRegistered;
    
    Customer() {
        customerId = "";
        name = "";
        email = "";
        phone = "";
        address = "";
        password = "";
        loyaltyPoints = 0;
        pendingDiscount = 0.0;
        isRegistered = false;
    }
    
    Customer(string id, string name, string email, string phone, string address, string password) {
        this->customerId = id;
        this->name = name;
        this->email = email;
        this->phone = phone;
        this->address = address;
        this->password = password;
        this->loyaltyPoints = 0;
        this->pendingDiscount = 0.0;
        this->isRegistered = true;
    }
};

// Hash Table for Customer Management
class CustomerHashTable
{
private:
    static const int TABLE_SIZE = 100;
    vector<Customer> table[TABLE_SIZE];
    
    int hashFunction(string key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }
    
public:
    void insertCustomer(Customer customer) {
        int index = hashFunction(customer.customerId);
        table[index].push_back(customer);
    }
    
    Customer* findCustomer(string customerId) {
        int index = hashFunction(customerId);
        for (auto& customer : table[index]) {
            if (customer.customerId == customerId) {
                return &customer;
            }
        }
        return nullptr;
    }
    
    Customer* findCustomerByEmail(string email) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (auto& customer : table[i]) {
                if (customer.email == email) {
                    return &customer;
                }
            }
        }
        return nullptr;
    }
    
    Customer* findCustomerByPhone(string phone) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (auto& customer : table[i]) {
                if (customer.phone == phone) {
                    return &customer;
                }
            }
        }
        return nullptr;
    }
    
    Customer* findCustomerByEmailOrPhone(string identifier) {
        // Try email first
        Customer* customer = findCustomerByEmail(identifier);
        if (customer) return customer;
        // Try phone
        return findCustomerByPhone(identifier);
    }
    
    void addOrderToHistory(string customerId, user order) {
        Customer* customer = findCustomer(customerId);
        if (customer) {
            customer->orderHistory.push_back(order);
            // Add loyalty points (1 point per 100 rupees spent)
            customer->loyaltyPoints += (int)(order.bill / 100);
        }
    }
    
    void displayCustomerInfo(string customerId) {
        Customer* customer = findCustomer(customerId);
        if (customer) {
            cout << "\n========== Customer Information ==========" << endl;
            cout << "Customer ID: " << customer->customerId << endl;
            cout << "Name: " << customer->name << endl;
            cout << "Email: " << customer->email << endl;
            cout << "Phone: " << customer->phone << endl;
            cout << "Address: " << customer->address << endl;
            cout << "Loyalty Points: " << customer->loyaltyPoints << endl;
            cout << "Total Orders: " << customer->orderHistory.size() << endl;
            cout << "=========================================" << endl;
        } else {
            cout << "Customer not found!" << endl;
        }
    }
    
    void displayOrderHistory(string customerId) {
        Customer* customer = findCustomer(customerId);
        if (customer && !customer->orderHistory.empty()) {
            cout << "\n========== Order History ==========" << endl;
            cout << "Customer: " << customer->name << endl;
            cout << "-----------------------------------" << endl;
            for (size_t i = 0; i < customer->orderHistory.size(); i++) {
                cout << "Order #" << (i + 1) << ":" << endl;
                cout << "  Product: " << customer->orderHistory[i].product << endl;
                cout << "  Quantity: " << customer->orderHistory[i].quantity << endl;
                cout << "  Bill: Rs. " << customer->orderHistory[i].bill << endl;
                cout << "-----------------------------------" << endl;
            }
            cout << "=================================" << endl;
        } else {
            cout << "No order history found for this customer!" << endl;
        }
    }
    
    bool isCustomerRegistered(string customerId) {
        return findCustomer(customerId) != nullptr;
    }
    
    void displayAllCustomers() {
        cout << "\n========== All Registered Customers ==========" << endl;
        bool hasCustomers = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (auto& customer : table[i]) {
                if (customer.isRegistered) {
                    hasCustomers = true;
                    cout << "ID: " << customer.customerId << " | Name: " << customer.name 
                         << " | Email: " << customer.email << " | Points: " << customer.loyaltyPoints << endl;
                }
            }
        }
        if (!hasCustomers) {
            cout << "No customers registered yet!" << endl;
        }
        cout << "=============================================" << endl;
    }
    
    // File Handling Functions
    void saveCustomersToFile(string filename = "customers.txt") {
        string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
        ofstream file(filepath);
        if (!file.is_open()) {
            cout << " Error: Could not create file " << filepath << endl;
            return;
        }
        
        int customerCount = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (auto& customer : table[i]) {
                if (customer.isRegistered) {
                    customerCount++;
                    file << customer.customerId << "|" << customer.name << "|" 
                         << customer.email << "|" << customer.phone << "|" 
                         << customer.address << "|" << customer.password << "|"
                         << customer.loyaltyPoints << "|"
                         << customer.pendingDiscount << "|";
                    
                    // Save order history
                    file << customer.orderHistory.size() << "|";
                    for (auto& order : customer.orderHistory) {
                        file << order.name << "|" << order.product << "|" 
                             << order.quantity << "|" << order.bill << "|";
                    }
                    file << endl;
                }
            }
        }
        file.close();
        cout << " Saved " << customerCount << " customers to " << filepath << endl;
    }
    
    void loadCustomersFromFile(string filename = "customers.txt") {
        string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename; 
        ifstream file(filepath);
        if (!file.is_open()) {
            return; 
        }
        
        string line;
        int loadedCount = 0;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 7) {
                try {
                    Customer customer;
                    customer.customerId = tokens[0];
                    customer.name = tokens[1];
                    customer.email = tokens[2];
                    customer.phone = tokens[3];
                    customer.address = tokens[4];
                    customer.password = (tokens.size() > 5) ? tokens[5] : "";
                    
                    customer.loyaltyPoints = (tokens.size() > 6) ? stoi(tokens[6]) : 0;
                    customer.pendingDiscount = (tokens.size() > 7) ? stod(tokens[7]) : 0.0;
                    
                    customer.isRegistered = true;
                    
                    // Load order history safely
                    if (tokens.size() > 8) {
                        int orderCount = stoi(tokens[8]);
                        int tokenIndex = 9;
                        for (int i = 0; i < orderCount; i++) {
                            if (tokenIndex + 3 < (int)tokens.size()) {
                                user order;
                                order.name = tokens[tokenIndex++];
                                order.product = tokens[tokenIndex++];
                                order.quantity = stoi(tokens[tokenIndex++]);
                                order.bill = stod(tokens[tokenIndex++]);
                                customer.orderHistory.push_back(order);
                            }
                        }
                    }
                    insertCustomer(customer);
                    loadedCount++;
                } catch (...) {
                    continue; 
                }
            }
        }
        file.close();
        cout << " Loaded " << loadedCount << " customers." << endl;
    }
    
    void exportCustomerReport(string filename = "customer_report.txt") {
        string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
        ofstream file(filepath);
        if (!file.is_open()) {
            cout << " Error: Could not create report file " << filepath << endl;
            return;
        }
        
        file << "========== CUSTOMER REPORT ==========" << endl;
        file << "Generated on: " << __DATE__ << " " << __TIME__ << endl;
        file << "=====================================" << endl << endl;
        
        int totalCustomers = 0;
        int totalOrders = 0;
        double totalRevenue = 0;
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (auto& customer : table[i]) {
                if (customer.isRegistered) {
                    totalCustomers++;
                    totalOrders += customer.orderHistory.size();
                    
                    file << "Customer ID: " << customer.customerId << endl;
                    file << "Name: " << customer.name << endl;
                    file << "Email: " << customer.email << endl;
                    file << "Phone: " << customer.phone << endl;
                    file << "Address: " << customer.address << endl;
                    file << "Loyalty Points: " << customer.loyaltyPoints << endl;
                    file << "Total Orders: " << customer.orderHistory.size() << endl;
                    
                    double customerRevenue = 0;
                    for (auto& order : customer.orderHistory) {
                        customerRevenue += order.bill;
                    }
                    totalRevenue += customerRevenue;
                    file << "Total Spent: Rs. " << customerRevenue << endl;
                    file << "----------------------------------------" << endl;
                }
            }
        }
        
        file << endl << "========== SUMMARY ==========" << endl;
        file << "Total Customers: " << totalCustomers << endl;
        file << "Total Orders: " << totalOrders << endl;
        file << "Total Revenue: Rs. " << totalRevenue << endl;
        file << "Average Order Value: Rs. " << (totalOrders > 0 ? totalRevenue / totalOrders : 0) << endl;
        file << "=============================" << endl;
        
        file.close();
        cout << " Customer report exported to " << filepath << endl;
    }
};


struct deliveryUser
{
    user nUser;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    deliveryUser *next = NULL;

    deliveryUser(string name, string product, int quantity, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->nUser = user(name, product, quantity, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
        this->next = NULL;
    }
};


struct Shop
{
    string name;
    string *product;
    int *price;
    int *stock;  // Stock quantity for each product
    string address;
    deliveryUser *nextdeliveryUser;
};


Shop* shop=NULL;
string addressF;
int dist[V];
int distanceP[V1];
int optionDelivery = -999;
int optionDelivery1 = -999;
deliveryUser *currentdeliveryUser = NULL;

// Global Customer Management System
CustomerHashTable customerDB;
string currentLoggedInCustomer = "";
bool isAdminLoggedIn = false;
string adminPassword = "admin123";  // Default admin password

//---------------------------------------------------------------------------------------------------------------------
//*************************************Take Away Users***********************************************

struct takeAway
{
    string name;
    string product;
    int quantity;
    double bill;
    int orderId;
    takeAway *left;
    takeAway *right;

    //takeAwayfromStore *next = NULL;

    takeAway(string name, string product, int quantity, double bill, int orderId)
    {
        this->name=name;
        this->product = product;
        this->quantity = quantity;
        this->bill = bill;
        this->orderId=orderId;
        this->left = NULL;
        this->right = NULL;
    }
};

takeAway *root = NULL;

void display(takeAway *root)
{
    cout <<"\n----------------------------------"<< endl;
    cout << "Name :" << root->name << endl;
    cout << "product :" << root->product << endl;
    cout << "Quantity : " << root->quantity << endl;
    cout << "Bill : " << root->bill << endl;
    cout << "Order ID: " << root->orderId << endl;
    cout <<"-----------------------------------\n"<< endl;
}

takeAway *displayTakeAway(takeAway *root)
{
    if (root)
    {
        displayTakeAway(root->left);
        display(root);
        displayTakeAway(root->right);
    }

    return root;
}

// Helper function to count matching orders
int countCustomerOrders(takeAway *root, string customerName) {
    if (root == NULL) return 0;
    int count = 0;
    if (root->name == customerName) count = 1;
    return count + countCustomerOrders(root->left, customerName) + countCustomerOrders(root->right, customerName);
}

// Display take-away orders filtered by customer or admin
void displayTakeAwayFiltered(takeAway *root, string customerName = "")
{
    if (root == NULL) {
        if (isAdminLoggedIn || currentLoggedInCustomer != "") {
            cout << "No take-away orders found!" << endl;
        } else {
            cout << "Please login to view your orders, or login as admin to view all orders!" << endl;
        }
        return;
    }
    
    if (isAdminLoggedIn) {
        // Admin sees all orders
        displayTakeAway(root);
    } else if (currentLoggedInCustomer != "" && customerName != "") {
        // Customer sees only their orders
        int orderCount = countCustomerOrders(root, customerName);
        if (orderCount == 0) {
            cout << "No take-away orders found for your account!" << endl;
        } else {
            // Traverse and display only matching orders
            if (root->left) displayTakeAwayFiltered(root->left, customerName);
            if (root->name == customerName) {
                display(root);
            }
            if (root->right) displayTakeAwayFiltered(root->right, customerName);
        }
    } else {
        cout << "Please login to view your orders, or login as admin to view all orders!" << endl;
    }
}

// isEmpty or not
int isEmpty(takeAway *root)
{
    return root == NULL;
}


// Height of takeAway tree
int height(takeAway* root) {
    if (root == NULL)
      return -1;
    else {
      int leftH = height(root -> left);
      int rightH = height(root -> right);

    return (1+max(leftH , rightH));
    }
}

// Balance Factor for each takeAway node
int balanceFactor(takeAway *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

// Maximum of two integers as a helper function for height
int max(int a, int b)
{
    return (a > b) ? a : b;
}

// Searching in takeAway tree

takeAway *search(takeAway *root, int id)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->orderId == id)
    {
        return root;
    }
    else if (root->orderId < id)
    {
        return search(root->right, id);
    }
    else if (root->orderId > id)
    {
        return search(root->left, id);
    }

    return root;
}

// Balancing the takeAway's Tree thorugh AVL Rotations

// LL Rotation
takeAway *LLRotation(takeAway *root)
{
    takeAway *x = root->left;
    takeAway *temp = x->right;

    x->right = root;
    root->left = temp;

    root = x;

    return x;
}

// RR Rotation
takeAway *RRRotation(takeAway *root)
{

    takeAway *x = root->right;
    takeAway *temp = x->left;

    x->left = root;
    root->right = temp;

    root = x;

    return x;
}

// LR Rotation
takeAway *LRRotation(takeAway *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}

// RL Rotation
takeAway *RLRotation(takeAway *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}

//  INSERTION in takeAway Tree

takeAway *insertion(string name, int quantity, string product, double bill, int orderId, takeAway *root)
{
    takeAway *newNode = new takeAway(name,  product, quantity, bill, orderId);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->orderId > newNode->orderId)
    {
        root->left = insertion(name, quantity, product, bill, orderId, root->left);
    }
    else if (root->orderId < newNode->orderId)
    {
        root->right = insertion(name, quantity, product, bill, orderId, root->right);
    }

    else
    {
        cout << "No duplicates Values are Allowed " << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        // LL
        if (root->left->orderId > newNode->orderId)
        {
            return LLRotation(root);
        }

        // LR
        if (root->left->orderId < newNode->orderId)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        // RR
        if (root->right->orderId < newNode->orderId)
        {
            return RRRotation(root);
        }

        // RL
        if (root->right->orderId > newNode->orderId)
        {
            return RLRotation(root);
        }
    }

    return root; // in case there is no need of rotation
}

takeAway * minValue(takeAway *root) {
    takeAway *current = root;
    while (current-> left != NULL) {
      current = current -> left;
    }
    return current;
  }

takeAway *deleteNode(takeAway *root, int orderId)
{
    if (root == NULL)
        return root;
    else if (orderId < root->orderId)
        root->left = deleteNode(root->left, orderId);
    else if (orderId > root->orderId)
        root->right = deleteNode(root->right, orderId);
    else {
      if (root-> left == NULL) {
        takeAway* temp = root-> right;
        delete root;
        return temp;
      }
      else if (root-> right == NULL) {
        takeAway* temp = root-> left;
        delete root;
        return temp;
      }

      else {
        takeAway * temp = minValue(root-> right);
        root-> orderId = temp -> orderId;
        root-> right = deleteNode(root-> right, temp-> orderId );
      }
    }

    // getting the balance factor
    int balance = balanceFactor(root);

    // Rotation Cases
    // LEFT LEFT CASE
    if (balance > 1 && orderId < root->left->orderId)
        return LLRotation(root);

    // LEFT RIGHT CASE
    if (balance > 1 && orderId > root->left->orderId)
    {
        root->left = LLRotation(root->left);
        return LRRotation(root);
    }

    // RIHGT RIGHT CASE
    if (balance < -1 && orderId > root->right->orderId)
        return RRRotation(root);

    // RIGHT LEFT CASE
    if (balance < -1 && orderId < root->right->orderId)
    {
        return RLRotation(root);
    }

    return root;
}

// File Handling Functions for Take-Away Orders
void saveTakeAwayOrdersToFile(takeAway *root, ofstream &file) {
    if (root != NULL) {
        saveTakeAwayOrdersToFile(root->left, file);
        file << root->orderId << "|" << root->name << "|" << root->product 
             << "|" << root->quantity << "|" << root->bill << endl;
        saveTakeAwayOrdersToFile(root->right, file);
    }
}

void saveTakeAwayOrders(string filename = "takeaway_orders.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ofstream file(filepath);
    if (!file.is_open()) {
        cout << " Error: Could not create file " << filepath << endl;
        return;
    }
    
    file << "OrderID|CustomerName|Product|Quantity|Bill" << endl;
    saveTakeAwayOrdersToFile(root, file);
    file.close();
    cout << " Take-away orders saved to " << filepath << endl;
}

takeAway* loadTakeAwayOrdersFromFile(string filename = "takeaway_orders.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ifstream file(filepath);
    if (!file.is_open()) return NULL;
    
    string line;
    // Check if file has data before skipping header
    if (file.peek() == EOF) return NULL; 
    getline(file, line); // Skip header

    takeAway* newRoot = NULL;
    int loadedCount = 0;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 5) {
            try {
                int orderId = stoi(tokens[0]);
                string name = tokens[1];
                string product = tokens[2];
                int quantity = stoi(tokens[3]);
                double bill = stod(tokens[4]); 
                
                newRoot = insertion(name, quantity, product, bill, orderId, newRoot);
                loadedCount++;
            } catch (...) {
                continue; // Skip bad data
            }
        }
    }
    
    file.close();
    cout << " Loaded " << loadedCount << " take-away orders." << endl;
    return newRoot;
}

// File Handling Functions for Delivery Orders
void saveDeliveryOrdersToFile(string filename = "delivery_orders.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ofstream file(filepath);
    if (!file.is_open()) {
        cout << " Error: Could not create file " << filepath << endl;
        return;
    }
    
    file << "CustomerName|Product|Quantity|Bill|Address|DeliveryCharges|Distance" << endl;
    
    if (shop->nextdeliveryUser != NULL) {
        deliveryUser *traversal = shop->nextdeliveryUser;
        while (traversal != NULL) {
            file << traversal->nUser.name << "|" << traversal->nUser.product << "|"
                 << traversal->nUser.quantity << "|" << traversal->nUser.bill << "|"
                 << traversal->address << "|" << traversal->deliveryCharges << "|"
                 << traversal->distanceDelivery << endl;
            traversal = traversal->next;
        }
    }
    
    file.close();
    cout << " Delivery orders saved to " << filepath << endl;
}

void loadDeliveryOrdersFromFile(string filename = "delivery_orders.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ifstream file(filepath);
    if (!file.is_open()) return;
    
    string line;
    if (file.peek() == EOF) return;
    getline(file, line); // Skip header
    
    int loadedCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 7) {
            try {
                string name = tokens[0];
                string product = tokens[1];
                int quantity = stoi(tokens[2]);
                double bill = stod(tokens[3]);
                string address = tokens[4];
                double deliveryCharges = stod(tokens[5]);
                int distance = stoi(tokens[6]);
                
                currentdeliveryUser = new deliveryUser(name, product, quantity, bill, address, deliveryCharges, distance);
                
                if (shop->nextdeliveryUser == NULL) {
                    shop->nextdeliveryUser = currentdeliveryUser;
                } else {
                    deliveryUser *temp = shop->nextdeliveryUser;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = currentdeliveryUser;
                    currentdeliveryUser->next = NULL;
                }
                loadedCount++;
            } catch (...) {
                continue;
            }
        }
    }
    file.close();
    cout << " Loaded " << loadedCount << " delivery orders." << endl;
}





//-----------------------------------------------------------------------------------------------------------------------
void placeOrderHomeDeliveryCustomer(string name, string product, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{

    currentdeliveryUser=new deliveryUser(name, product, quantity, bill, address, deliveryCharges, distanceDelivery);

    if (shop->nextdeliveryUser == NULL)
    {
        // if first insert in front
        shop->nextdeliveryUser= currentdeliveryUser;
    }
    else
    {
        // finding the last Node
        deliveryUser *temp = shop->nextdeliveryUser;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentdeliveryUser;
        currentdeliveryUser->next = NULL;
    }

    cout << "\n***************Order Details***************";
    cout << "\n Customer Name : " << name << "\n Order name : " << product << "\n Quantity : " << quantity << "\n Total bill : " << bill;
    cout << "\n Address : " << address << "\n Distance in km : " << distanceDelivery <<"\n Delivery charges : " << deliveryCharges << endl;
}


void displayAllOrdersHomeDeliveryCustomers()
{
    if (shop->nextdeliveryUser == NULL)
    {
        cout << "There is no Order for Home Delivery Customer till yet" << endl;
        return;
    }
    
    // If admin, show all orders; if customer, show only their orders
    if (isAdminLoggedIn) {
        deliveryUser *traversal = shop->nextdeliveryUser;
        while (traversal != NULL)
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "Home Delivery Customer : " << traversal->nUser.name << endl;
            cout << "Product Name : " << traversal->nUser.product << endl;
            cout << "Quantity : " << traversal->nUser.quantity << endl;
            cout << "Delivery Distance : " << traversal->distanceDelivery << " KM"<<endl;
            cout << "Delivery Charges : " << traversal->deliveryCharges << " RS" << endl;
            cout << "Bill : " << traversal->nUser.bill << " RS/_" << endl;
            cout << "Delivery Address : " << traversal->address << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    } else if (currentLoggedInCustomer != "") {
        // Show only logged-in customer's orders
        Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
        bool found = false;
        deliveryUser *traversal = shop->nextdeliveryUser;
        while (traversal != NULL)
        {
            if (traversal->nUser.name == customer->name) {
                found = true;
                cout << "-----------------------------------------------------" << endl;
                cout << "Home Delivery Customer : " << traversal->nUser.name << endl;
                cout << "Product Name : " << traversal->nUser.product << endl;
                cout << "Quantity : " << traversal->nUser.quantity << endl;
                cout << "Delivery Distance : " << traversal->distanceDelivery << " KM"<<endl;
                cout << "Delivery Charges : " << traversal->deliveryCharges << " RS" << endl;
                cout << "Bill : " << traversal->nUser.bill << " RS/_" << endl;
                cout << "Delivery Address : " << traversal->address << endl;
                cout << "-----------------------------------------------------" << endl;
            }
            traversal = traversal->next;
        }
        if (!found) {
            cout << "No delivery orders found for your account!" << endl;
        }
    } else {
        cout << "Please login to view your orders, or login as admin to view all orders!" << endl;
    }
}





//-----------------------------------------------------------------------------------

int minDistance(int dist[], bool visited[])
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (visited[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}



void dijkstra(int graph[V][V], int dist[], int src)
{
    bool visited[V];

    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, visited[i] = false;

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited);

        visited[u] = true;

        for (int v = 0; v < V; v++)

            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                    dist[v] = dist[u] + graph[u][v];
    }
}

//-----------------------------------------------------------------------------------

int minKey(int distanceP[], bool visitedP[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V1; v++)
        if (visitedP[v] == false && distanceP[v] < min)
            min = distanceP[v], min_index = v;

    return min_index;
}

void printMST(int parent[], int graph[V1][V1])
{
    cout << "Edge \tWeight\n";
    for (int i = 1; i < V1; i++)
        cout << parent[i] << " - " << i << " \t"
            << graph[i][parent[i]] << " \n";
}

void prims(int graph[V1][V1], int distanceP[])
{
    bool visitedP[V1];

    for (int i = 0; i < V1; i++)
        distanceP[i] = INT_MAX, visitedP[i] = false;

    distanceP[0] = 0;

    for (int count = 0; count < V1 - 1; count++) {

        int u = minKey(distanceP, visitedP);

        visitedP[u] = true;

        for (int v = 0; v < V1; v++)

            if (graph[u][v] && visitedP[v] == false && graph[u][v] < distanceP[v])
                distanceP[v] = graph[u][v];
    }

}






//-----------------------------------------------------------------------------------


string CityName[] = {"WareHouse", "New Delhi", "Mumbai", "Bengaluru", "Chennai", "Kolkata", "Pune", "Hyderabad", "Ahmedabad", "Jaipur", "Lucknow"};

string HyderabadAreas[]={"Hyderabad", "Hitech City", "Banjara Hills", "Gachibowli", "Charminar", "Secunderabad"};

string NewDelhiAreas[]={"New Delhi", "Dwarka" ,"Saket", "Karol Bagh", "Rohini", "Connaught Place"};

string MumbaiAreas[]={"Mumbai", "Andheri", "Bandra", "Dadar", "Powai", "Colaba"};

string BengaluruAreas[]={"Bengaluru", "Indiranagar", "Koramangala", "Whitefield", "Jayanagar", "Malleshwaram"};

int graph[V][V]={   {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3},   //Warehouse
                    {4, 0, 0, 0, 0, 0, 0, 6, 0, 0, 2},   //New Delhi
                    {0, 0, 0, 0, 9, 4, 0, 0, 0, 0, 11},  //Mumbai
                    {0, 0, 0, 0, 1, 0, 0, 0, 16, 5, 0},  //Bengaluru
                    {0, 0, 9, 1, 0, 8, 13, 0, 10, 6, 0}, //Chennai
                    {0, 0, 4, 0, 8, 0, 0, 3, 17, 0, 7},  //Kolkata
                    {0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 7},  //Pune
                    {0, 6, 0, 0, 0, 3, 0, 0, 5, 0, 0},   //Hyderabad
                    {0, 0, 0, 16, 10, 17, 0, 5, 0, 0, 0},//Ahmedabad
                    {0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0},   //Jaipur
                    {3, 2, 11, 0, 0, 7, 7, 0, 0, 0, 0}  };//Lucknow

//----------------------Hyderabad Graph--------------------------------
int HydGraph[V1][V1]={  { 0, 4, 4, 0, 0, 0 },
                        { 4, 0, 2, 0, 0, 0 },   
                        { 4, 2, 0, 3, 2, 4 },   
                        { 0, 0, 3, 0, 0, 3 },   
                        { 0, 0, 2, 0, 0, 3 },   
                        { 0, 0, 4, 3, 3, 0 } }; 

//---------------------New Delhi Graph------------------------------------
int DelGraph[V1][V1]={  { 0, 4, 0, 2, 0, 0 },
                        { 4, 0, 3, 1, 0, 6 },   
                        { 0, 3, 0, 4, 0, 7 },   
                        { 2, 1, 4, 0, 9, 0 },   
                        { 0, 0, 0, 9, 0, 5 },   
                        { 0, 6, 7, 0, 5, 0 } }; 

//-----------------------Mumbai Graph---------------------------------------
int MumGraph[V1][V1]={  { 0, 9, 15, 6, 0, 0 },
                        { 9, 0, 7, 0, 0, 8 },
                        { 15, 7, 0, 8, 9, 5 },
                        { 6, 0, 8, 0, 11, 0 },
                        { 0, 0, 9, 11, 0, 0 },
                        { 0, 8, 5, 0, 0, 0 } }; 

//----------------------Bengaluru Graph----------------------------------------
int BenGraph[V1][V1]={  { 0, 6, 3, 0, 0, 1 },
                        { 6, 0, 0, 2, 0, 5 },   
                        { 3, 0, 0, 0, 3, 5 },   
                        { 0, 2, 0, 0, 6, 4 },   
                        { 0, 0, 3, 6, 0, 6 },   
                        { 1, 5, 5, 4, 6, 0 } }; 

//----------------------------------------------------------------------------------

int city(){
    int opt;
    cout << "\nThe delivery is available for following Areas : \n" << endl;

    cout<<"\n---------------------------------";
    cout<<"\n |CITY CODE |        City       |";
    cout<<"\n---------------------------------";
    cout<<"\n |     1    |    New Delhi      |"<<endl;
    cout<<"\n |     2    |    Mumbai         |"<<endl;
    cout<<"\n |     3    |    Bengaluru      |"<<endl;
    cout<<"\n |     4    |    Chennai        |"<<endl;
    cout<<"\n |     5    |    Kolkata        |"<<endl;
    cout<<"\n |     6    |    Pune           |"<<endl;
    cout<<"\n |     7    |    Hyderabad      |"<<endl;
    cout<<"\n |     8    |    Ahmedabad      |"<<endl;
    cout<<"\n |     9    |    Jaipur         |"<<endl;
    cout<<"\n |    10    |    Lucknow        |"<<endl;
    cout<<"---------------------------------\n"<<endl;



    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;
}




int newDelhiSub(){
    int opt;
    cout << "\nThe delivery is available for following Areas in New Delhi: " << endl;

    cout<<"\n---------------------------------";
    cout<<"\n |CITY CODE |        AREA       |";
    cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Dwarka         |"<<endl;
    cout<<"\n |     2    |    Saket          |"<<endl;
    cout<<"\n |     3    |    Karol Bagh     |"<<endl;
    cout<<"\n |     4    |    Rohini         |"<<endl;
    cout<<"\n |     5    |    Connaught Place|"<<endl;
    cout<<"---------------------------------\n"<<endl;


    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;

}

int hyderabadSub(){
    int opt;
    cout << "\nThe delivery is available for following Areas in Hyderabad: " << endl;

    cout<<"\n---------------------------------";
    cout<<"\n |CITY CODE |        AREA       |";
    cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Hitech City    |"<<endl;
    cout<<"\n |     2    |    Banjara Hills  |"<<endl;
    cout<<"\n |     3    |    Gachibowli     |"<<endl;
    cout<<"\n |     4    |    Charminar      |"<<endl;
    cout<<"\n |     5    |    Secunderabad   |"<<endl;
    cout<<"---------------------------------\n"<<endl;

    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;

}

int mumbaiSub(){
    int opt;
    cout << "\nThe delivery is available for following Areas in Mumbai: " << endl;

    cout<<"\n---------------------------------";
    cout<<"\n |CITY CODE |        AREA       |";
    cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Andheri        |"<<endl;
    cout<<"\n |     2    |    Bandra         |"<<endl;
    cout<<"\n |     3    |    Dadar          |"<<endl;
    cout<<"\n |     4    |    Powai          |"<<endl;
    cout<<"\n |     5    |    Colaba         |"<<endl;
    cout<<"---------------------------------\n"<<endl;


    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;

}

int bengaluruSub(){
    int opt;
    cout << "\nThe delivery is available for following Areas in Bengaluru: " << endl;

    cout<<"\n---------------------------------";
    cout<<"\n |CITY CODE |        AREA       |";
    cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Indiranagar    |"<<endl;
    cout<<"\n |     2    |    Koramangala    |"<<endl;
    cout<<"\n |     3    |    Whitefield     |"<<endl;
    cout<<"\n |     4    |    Jayanagar      |"<<endl;
    cout<<"\n |     5    |    Malleshwaram   |"<<endl;
    cout<<"---------------------------------\n"<<endl;

    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;

}


int FurthurArea(int option){
    int opt;
    switch(option){

            case 1:
            {
                prims(DelGraph,distanceP);
                opt=newDelhiSub();
                addressF=NewDelhiAreas[opt];
            }
            break;

            case 2:
            {
                prims(MumGraph,distanceP);
                opt=mumbaiSub();
                addressF=MumbaiAreas[opt];
            }
            break;

            case 3:
            {
                prims(BenGraph,distanceP);
                opt=bengaluruSub();
                addressF=BengaluruAreas[opt];
            }
            break;

            case 7:
            {
                prims(HydGraph,distanceP);
                opt=hyderabadSub();
                addressF=HyderabadAreas[opt];
            }
            break;
        }

    return opt;
}

//---------------------------------------------------------------------------------------------

// Customer Management Functions
bool registerCustomer() {
    string customerId, name, email, phone, address, password;
    
    clearScreen();
    cout << "\n========== Customer Registration ==========" << endl;
    cout << "Enter Customer ID: ";
    cin >> customerId;
    
    // Check if customer already exists
    if (customerDB.isCustomerRegistered(customerId)) {
        cout << "\n Customer ID already exists! Please choose a different ID." << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return false;
    }
    
    cout << "Enter Full Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Email: ";
    getline(cin, email);
    
    // Check if email already exists
    if (customerDB.findCustomerByEmail(email)) {
        cout << "\n Email already registered! Please use a different email." << endl;
        cout << "Press Enter to continue...";
        cin.get();
        return false;
    }
    
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    
    // Check if phone already exists
    if (customerDB.findCustomerByPhone(phone)) {
        cout << "\n Phone number already registered! Please use a different phone." << endl;
        cout << "Press Enter to continue...";
        cin.get();
        return false;
    }
    
    cout << "Enter Address: ";
    getline(cin, address);
    
    cout << "Create Password: ";
    getline(cin, password);
    
    Customer newCustomer(customerId, name, email, phone, address, password);
    customerDB.insertCustomer(newCustomer);
    
    cout << "\n Customer registered successfully!" << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Name: " << name << endl;
    cout << "=========================================" << endl;
    cout << "Press Enter to continue...";
    cin.get();
    return true;
}

bool loginCustomer() {
    string identifier, password;
    
    clearScreen();
    cout << "\n========== Customer Login ==========" << endl;
    cout << "Enter Email or Phone Number: ";
    cin.ignore();
    getline(cin, identifier);
    
    cout << "Enter Password: ";
    getline(cin, password);
    
    Customer* customer = customerDB.findCustomerByEmailOrPhone(identifier);
    
    if (customer && customer->password == password) {
        currentLoggedInCustomer = customer->customerId;
        clearScreen();
        cout << "\n Login successful!" << endl;
        cout << "Welcome back, " << customer->name << "!" << endl;
        cout << "Your loyalty points: " << customer->loyaltyPoints << endl;
        cout << "=================================" << endl;
        return true;
    } else {
        cout << "\n Invalid credentials! Please try again." << endl;
        cout << "=================================" << endl;
        cout << "Press Enter to continue...";
        cin.get();
        return false;
    }
}

void logoutCustomer() {
    if (currentLoggedInCustomer != "") {
        clearScreen();
        cout << "\n Logged out successfully!" << endl;
        currentLoggedInCustomer = "";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    } else {
        cout << "\n No customer is currently logged in!" << endl;
    }
}

void viewCustomerProfile() {
    if (currentLoggedInCustomer == "") {
        cout << "\n Please login first to view your profile!" << endl;
        return;
    }
    
    clearScreen();
    customerDB.displayCustomerInfo(currentLoggedInCustomer);
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void viewOrderHistory() {
    if (currentLoggedInCustomer == "") {
        cout << "\n Please login first to view your order history!" << endl;
        return;
    }
    
    clearScreen();
    customerDB.displayOrderHistory(currentLoggedInCustomer);
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void viewLoyaltyPoints() {
    if (currentLoggedInCustomer == "") {
        cout << "\n Please login first to view your loyalty points!" << endl;
        return;
    }
    
    clearScreen();
    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
    if (customer) {
        cout << "\n========== Loyalty Points ==========" << endl;
        cout << "Customer: " << customer->name << endl;
        cout << "Current Points: " << customer->loyaltyPoints << endl;
        if (customer->pendingDiscount > 0) {
            cout << "Pending Discount: Rs. " << customer->pendingDiscount << " (will be applied to next order)" << endl;
        }
        cout << "\nRedemption Options:" << endl;
        cout << "100 points = Rs. 10 discount" << endl;
        cout << "500 points = Rs. 50 discount" << endl;
        cout << "1000 points = Rs. 100 discount" << endl;
        cout << "=================================" << endl;
    }
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void viewAllCustomers() {
    if (!isAdminLoggedIn) {
        cout << "\n Access denied! Admin login required." << endl;
        return;
    }
    customerDB.displayAllCustomers();
}

// Admin login function
void adminLogin() {
    string password;
    cout << "\n========== Admin Login ==========" << endl;
    cout << "Enter admin password: ";
    cin >> password;
    
    if (password == adminPassword) {
        isAdminLoggedIn = true;
        cout << " Admin login successful!" << endl;
        cout << "=================================" << endl;
    } else {
        cout << " Invalid password! Access denied." << endl;
        cout << "=================================" << endl;
    }
}

// Admin logout function
void adminLogout() {
    if (isAdminLoggedIn) {
        isAdminLoggedIn = false;
        cout << "\n Admin logged out successfully!" << endl;
    } else {
        cout << "\n No admin is currently logged in!" << endl;
    }
}

// Update stock function (Admin only)
void updateStock() {
    if (!isAdminLoggedIn) {
        cout << "\n Access denied! Admin login required." << endl;
        return;
    }
    
    cout << "\n========== Update Stock ==========" << endl;
    cout << "Current Stock:" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << i << ". " << shop->product[i] << " - Stock: " << shop->stock[i] << endl;
    }
    
    int productNum, newStock;
    cout << "\nEnter product number (1-10): ";
    cin >> productNum;
    
    if (productNum < 1 || productNum > 10) {
        cout << "Invalid product number!" << endl;
        return;
    }
    
    cout << "Enter new stock quantity: ";
    cin >> newStock;
    
    if (newStock < 0) {
        cout << "Stock cannot be negative!" << endl;
        return;
    }
    
    shop->stock[productNum] = newStock;
    cout << "\n Stock updated successfully!" << endl;
    cout << shop->product[productNum] << " - New Stock: " << shop->stock[productNum] << endl;
    cout << "=================================" << endl;
}

void redeemLoyaltyPoints() {
    if (currentLoggedInCustomer == "") {
        cout << "\n Please login first to redeem loyalty points!" << endl;
        return;
    }
    
    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
    if (!customer) {
        cout << "\n Error: Customer not found!" << endl;
        return;
    }
    if (customer->loyaltyPoints < 100) {
        cout << "\n You need at least 100 points to redeem! Current points: " << customer->loyaltyPoints << endl;
        return;
    }
    
    cout << "\n========== Loyalty Points Redemption ==========" << endl;
    cout << "Your current points: " << customer->loyaltyPoints << endl;
    cout << "Redemption options:" << endl;
    cout << "1. 100 points = Rs. 10 discount" << endl;
    cout << "2. 500 points = Rs. 50 discount" << endl;
    cout << "3. 1000 points = Rs. 100 discount" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice;
    cin >> choice;
    
    int pointsToRedeem, discountAmount;
    switch(choice) {
        case 1:
            pointsToRedeem = 100;
            discountAmount = 10;
            break;
        case 2:
            pointsToRedeem = 500;
            discountAmount = 50;
            break;
        case 3:
            pointsToRedeem = 1000;
            discountAmount = 100;
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    if (customer->loyaltyPoints >= pointsToRedeem) {
        customer->loyaltyPoints -= pointsToRedeem;
        customer->pendingDiscount = discountAmount;  // Store discount for next order
        cout << "\n Redemption successful!" << endl;
        cout << "Points redeemed: " << pointsToRedeem << endl;
        cout << "Discount amount: Rs. " << discountAmount << endl;
        cout << "Remaining points: " << customer->loyaltyPoints << endl;
        cout << "Note: This discount will be applied to your next order!" << endl;
    } else {
        cout << "\n Insufficient points! You have " << customer->loyaltyPoints << " points." << endl;
    }
    cout << "=============================================" << endl;
}

// Stock Management Functions
void saveStockToFile(string filename = "stock.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ofstream file(filepath);
    if (!file.is_open()) {
        cout << " Error: Could not create file " << filepath << endl;
        return;
    }
    
    for (int i = 1; i <= 10; i++) {
        file << i << "|" << shop->stock[i] << endl;
    }
    
    file.close();
    cout << " Stock data saved to " << filepath << endl;
}

void loadStockFromFile(string filename = "stock.txt") {
    string filepath = "C:\\Users\\vitth\\Desktop\\E-Commerce\\" + filename;
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "  No existing stock file found. Using default stock values." << endl;
        return;
    }
    
    string line;
    int loadedCount = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, '|')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 2) {
            int productNum = stoi(tokens[0]);
            int stockValue = stoi(tokens[1]);
            if (productNum >= 1 && productNum <= 10) {
                shop->stock[productNum] = stockValue;
                loadedCount++;
            }
        }
    }
    
    file.close();
    cout << " Loaded stock data for " << loadedCount << " products from " << filepath << endl;
}

// File Management Functions
void saveAllData() {
    cout << "\n========== Saving All Data ==========" << endl;
    customerDB.saveCustomersToFile();
    saveTakeAwayOrders();
    saveDeliveryOrdersToFile();
    saveStockToFile();
    cout << " All data saved successfully!" << endl;
    cout << "=====================================" << endl;
}

void loadAllData() {
    cout << "\n========== Loading All Data ==========" << endl;
    customerDB.loadCustomersFromFile();
    root = loadTakeAwayOrdersFromFile();
    loadDeliveryOrdersFromFile();
    loadStockFromFile();
    cout << " All data loaded successfully!" << endl;
    cout << "=====================================" << endl;
}

void exportAllReports() {
    cout << "\n========== Exporting Reports ==========" << endl;
    customerDB.exportCustomerReport();
    saveTakeAwayOrders("takeaway_report.txt");
    saveDeliveryOrdersToFile("delivery_report.txt");
    cout << " All reports exported successfully!" << endl;
    cout << "======================================" << endl;
}

void backupData() {
    cout << "\n========== Creating Backup ==========" << endl;
    string timestamp = to_string(time(nullptr));
    customerDB.saveCustomersToFile("backup_customers_" + timestamp + ".txt");
    saveTakeAwayOrders("backup_takeaway_" + timestamp + ".txt");
    saveDeliveryOrdersToFile("backup_delivery_" + timestamp + ".txt");
    cout << " Backup created with timestamp: " << timestamp << endl;
    cout << "Backup files saved to: C:\\Users\\vitth\\Desktop\\E-Commerce\\" << endl;
    cout << "====================================" << endl;
}

void restoreFromBackup() {
    cout << "\n========== Restore from Backup ==========" << endl;
    cout << "Available backup files:" << endl;
    cout << "1. customers.txt" << endl;
    cout << "2. takeaway_orders.txt" << endl;
    cout << "3. delivery_orders.txt" << endl;
    cout << "Enter choice (1-3): ";
    
    int choice;
    cin >> choice;
    
    switch(choice) {
        case 1:
            customerDB.loadCustomersFromFile();
            break;
        case 2:
            root = loadTakeAwayOrdersFromFile();
            break;
        case 3:
            loadDeliveryOrdersFromFile();
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    cout << " Restore completed!" << endl;
    cout << "=====================================" << endl;
}

bool isCustomerLoggedIn() {
    return currentLoggedInCustomer != "";
}

string getCurrentCustomerId() {
    return currentLoggedInCustomer;
}

// Forward declarations
void placeOrderWithCustomer(string name, string product, int quantity, double bill, string orderType);

// New Flow Functions
void displayMainDashboard() {
    clearScreen();
    if (currentLoggedInCustomer == "") {
        return; // Should not happen, but safety check
    }
    
    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
    if (!customer) {
        return; // Should not happen, but safety check
    }
    cout << "\n=========================================" << endl;
    cout << "   Welcome back, " << customer->name << "!" << endl;
    cout << "=========================================" << endl;
    cout << "\nAvailable Products:\n" << endl;
    
    cout << "-----------------------------------------------------------------------------------------";
    cout << "\n | ITEM NO. |             ITEM NAME               | ORIGINAL PRICE  |    STOCK    |";
    cout << "\n-----------------------------------------------------------------------------------------";
    for (int i = 1; i <= 10; i++) {
        cout << "\n |    " << i << "     |  " << shop->product[i];
        int nameLen = shop->product[i].length();
        for (int j = nameLen; j < 35; j++) cout << " ";
        cout << "|        " << shop->price[i];
        int priceLen = to_string(shop->price[i]).length();
        for (int j = priceLen; j < 6; j++) cout << " ";
        cout << "     |       " << shop->stock[i] << "       |";
    }
    cout << "\n-----------------------------------------------------------------------------------------" << endl;
    
    cout << "\nEnter Product No. (1-10) to view details, or 0 for Other Options: ";
}

void displayProductDetails(int productNumber) {
    clearScreen();
    if (productNumber < 1 || productNumber > 10) {
        cout << "Invalid product number!" << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
    if (!customer) {
        cout << "Error: Customer not found!" << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    cout << "\n=========================================" << endl;
    cout << "   Product Details" << endl;
    cout << "=========================================" << endl;
    cout << "Product ID: " << productNumber << endl;
    cout << "Product Name: " << shop->product[productNumber] << endl;
    cout << "Price: Rs. " << shop->price[productNumber] << endl;
    cout << "Stock Available: " << shop->stock[productNumber] << endl;
    cout << "=========================================" << endl;
    
    // Ask about loyalty points
    if (customer && customer->pendingDiscount > 0) {
        cout << "\nYou have a pending discount of Rs. " << customer->pendingDiscount << endl;
        cout << "This discount will be applied to your order." << endl;
    } else if (customer && customer->loyaltyPoints >= 100) {
        char usePoints;
        cout << "\nDo you want to use Loyalty Points? (y/n): ";
        cin.ignore();
        cin >> usePoints;
        if (usePoints == 'y' || usePoints == 'Y') {
            redeemLoyaltyPoints();
            customer = customerDB.findCustomer(currentLoggedInCustomer); // Refresh customer data
        }
    }
    
    cout << "\n=========================================" << endl;
    cout << "   Order Options" << endl;
    cout << "=========================================" << endl;
    cout << "1. Get it Delivered" << endl;
    cout << "2. Pick it up from Warehouse" << endl;
    cout << "3. Back to Product List" << endl;
    cout << "Enter your choice: ";
}

void handleProductOrder(int productNumber, int orderType) {
    // orderType: 1 = Delivery, 2 = Pickup
    if (productNumber < 1 || productNumber > 10) {
        cout << "Invalid product number!" << endl;
        return;
    }
    
    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
    if (!customer) {
        cout << "Error: Customer not found!" << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    string name = customer->name;
    int quantity;
    int orderId;
    
    cout << "\nEnter quantity: ";
    cin >> quantity;
    
    if (quantity <= 0) {
        cout << "Invalid quantity! Please enter a positive number." << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    if (quantity > shop->stock[productNumber]) {
        cout << "\n Insufficient stock!" << endl;
        cout << "Available stock: " << shop->stock[productNumber] << endl;
        cout << "Requested quantity: " << quantity << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }
    
    double bill = quantity * shop->price[productNumber];
    double discount = 0.0;
    
    // Apply loyalty discount
    if (customer->pendingDiscount > 0) {
        discount = customer->pendingDiscount;
        bill = max(0.0, bill - discount);
    }
    
    if (orderType == 1) {
        // Delivery order
        dijkstra(graph, dist, 0);
        int optionDelivery = -999;
        int optionDelivery1 = -999;
        string address;
        int distance;
        
        do {
            optionDelivery = city();
        } while (!(optionDelivery >= 0 && optionDelivery <= 10));
        
        if (optionDelivery == 1 || optionDelivery == 2 || optionDelivery == 3 || optionDelivery == 7) {
            do {
                optionDelivery1 = FurthurArea(optionDelivery);
            } while (!(optionDelivery1 >= 0 && optionDelivery1 <= 5));
            
            address = CityName[optionDelivery] + " " + addressF;
            distance = dist[optionDelivery] + distanceP[optionDelivery1];
        } else {
            address = CityName[optionDelivery];
            distance = dist[optionDelivery];
        }
        
        int deliveryChargesPerKM = 20;
        int deliveryCharges = deliveryChargesPerKM * distance;
        bill += deliveryCharges;
        
        // Re-apply discount after adding delivery charges
        if (discount > 0) {
            bill = max(0.0, bill - discount);
        }
        
        placeOrderHomeDeliveryCustomer(name, shop->product[productNumber], quantity, bill, address, deliveryCharges, distance);
        placeOrderWithCustomer(name, shop->product[productNumber], quantity, bill, "Home Delivery");
        
        clearScreen();
        cout << "\n=========================================" << endl;
        cout << "   Order Confirmed!" << endl;
        cout << "=========================================" << endl;
        cout << "Order Type: Home Delivery" << endl;
        cout << "Product: " << shop->product[productNumber] << endl;
        cout << "Quantity: " << quantity << endl;
        if (discount > 0) {
            cout << "Original amount: Rs. " << (quantity * shop->price[productNumber] + deliveryCharges) << endl;
            cout << "Discount applied: Rs. " << discount << endl;
        }
        cout << "Delivery Charges: Rs. " << deliveryCharges << endl;
        cout << "Total Amount: Rs. " << bill << endl;
        cout << "Delivery Address: " << address << endl;
        cout << "=========================================" << endl;
        
    } else if (orderType == 2) {
        // Pickup order
        cout << "Enter Order ID: ";
        cin >> orderId;
        
        root = insertion(name, quantity, shop->product[productNumber], bill, orderId, root);
        placeOrderWithCustomer(name, shop->product[productNumber], quantity, bill, "Take-Away");
        
        clearScreen();
        cout << "\n=========================================" << endl;
        cout << "   Order Confirmed!" << endl;
        cout << "=========================================" << endl;
        cout << "Order Type: Pickup from Warehouse" << endl;
        cout << "Product: " << shop->product[productNumber] << endl;
        cout << "Quantity: " << quantity << endl;
        if (discount > 0) {
            cout << "Original amount: Rs. " << (quantity * shop->price[productNumber]) << endl;
            cout << "Discount applied: Rs. " << discount << endl;
        }
        cout << "Total Amount: Rs. " << bill << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "You can collect your order from the Warehouse" << endl;
        cout << "=========================================" << endl;
    }
    
    // Update stock
    shop->stock[productNumber] -= quantity;
    
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void displayOtherOptions() {
    clearScreen();
    cout << "\n=========================================" << endl;
    cout << "   Other Options" << endl;
    cout << "=========================================" << endl;
    cout << "1. View My Profile" << endl;
    cout << "2. View My Order History" << endl;
    cout << "3. View Loyalty Points" << endl;
    cout << "4. Get Order from Warehouse" << endl;
    cout << "5. Logout" << endl;
    cout << "0. Back to Dashboard" << endl;
    cout << "=========================================" << endl;
    cout << "Enter your choice: ";
}

void handleOtherOptions(int choice) {
    switch(choice) {
        case 1:
            clearScreen();
            viewCustomerProfile();
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 2:
            clearScreen();
            viewOrderHistory();
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 3:
            clearScreen();
            if (currentLoggedInCustomer != "") {
                Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
                cout << "\n=========================================" << endl;
                cout << "   Loyalty Points" << endl;
                cout << "=========================================" << endl;
                cout << "Current Points: " << customer->loyaltyPoints << endl;
                if (customer->pendingDiscount > 0) {
                    cout << "Pending Discount: Rs. " << customer->pendingDiscount << endl;
                }
                cout << "=========================================" << endl;
                cout << "\nWould you like to redeem points? (y/n): ";
                char redeem;
                cin.ignore();
                cin >> redeem;
                if (redeem == 'y' || redeem == 'Y') {
                    redeemLoyaltyPoints();
                }
            }
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        case 4:
            {
                clearScreen();
                cout << "\n=========================================" << endl;
                cout << "   Get Order from Warehouse" << endl;
                cout << "=========================================" << endl;
                cout << "Enter the Order ID: ";
                int orderId;
                cin >> orderId;
                
                takeAway *cust = search(root, orderId);
                if (cust == NULL) {
                    cout << "\nNo such order found in the Warehouse!" << endl;
                } else {
                    // Check if order belongs to logged-in customer
                    Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
                    if (cust->name == customer->name) {
                        root = deleteNode(root, orderId);
                        cout << "\nYour order is ready for collection!" << endl;
                        cout << "Please get it from the counter." << endl;
                        cout << "Order Status: Completed/Picked Up" << endl;
                    } else {
                        cout << "\nThis order does not belong to your account!" << endl;
                    }
                }
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            break;
        case 5:
            logoutCustomer();
            break;
        default:
            break;
    }
}

void authenticationScreen() {
    int choice;
    bool authenticated = false;
    
    while (!authenticated) {
        clearScreen();
        cout << "\n=========================================" << endl;
        cout << "   Welcome to " << shop->name << endl;
        cout << "=========================================" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "=========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                if (registerCustomer()) {
                    // Registration successful, go to login
                    continue;
                }
                break;
            case 2:
                if (loginCustomer()) {
                    authenticated = true;
                }
                break;
            case 0:
                exit(0);
                break;
            default:
                cout << "Invalid choice! Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
        }
    }
}

// Enhanced order placement with customer integration
void placeOrderWithCustomer(string name, string product, int quantity, double bill, string orderType) {
    // Add order to customer history if logged in
    if (isCustomerLoggedIn()) {
        user order(name, product, quantity, bill);
        customerDB.addOrderToHistory(currentLoggedInCustomer, order);
        
        Customer* customer = customerDB.findCustomer(currentLoggedInCustomer);
        if (customer) {
            cout << "\n Order placed successfully!" << endl;
            cout << "Customer: " << customer->name << endl;
            cout << "Order Type: " << orderType << endl;
            cout << "Loyalty Points Earned: " << (int)(bill / 100) << endl;
            cout << "Total Loyalty Points: " << customer->loyaltyPoints << endl;
            
            // Clear pending discount after applying
            if (customer->pendingDiscount > 0) {
                customer->pendingDiscount = 0.0;
            }
        }
    } else {
        cout << "\n Order placed successfully!" << endl;
        cout << "Note: Register and login to earn loyalty points!" << endl;
    }
    
    // Auto-save data after order placement (with error handling)
    try {
        customerDB.saveCustomersToFile();
        saveTakeAwayOrders();
        saveDeliveryOrdersToFile();
        saveStockToFile();
    } catch (...) {
        cout << "Warning: Could not save data automatically." << endl;
    }
}


int main()
{
    shop =new Shop;
    shop->name="WareHouse";
    shop->address="Connaught Place, New Delhi";
    shop->nextdeliveryUser = NULL;

    shop->product=new string[11]{"","Sony FX30           ", "Sony FR7            ", "Canon EOS 90D      ",
                                 "Nikon D6            ", "Panasonic Lumix G10","Canon EOS 250D      ",
                                  "Sony Alpha 7        ", "Nikon D750          ", "Panasonic Lumix GH5",
                                "Canon EOS 5D Mark "};


    shop->price=new int[11]{0, 45500, 175000, 289000, 155700, 75000, 210000, 199999, 100000, 45000, 500000};
    shop->stock=new int[11]{0, 50, 30, 25, 40, 35, 20, 45, 30, 50, 15};  // Initial stock for each product
    
    // Load existing data at startup
    loadAllData();
    
    // New Flow: Authentication -> Dashboard -> Product/Options
    while (true) {
        // Step 1: Authentication
        authenticationScreen();
        
        // Step 2: Main Dashboard Loop
        while (currentLoggedInCustomer != "") {
            int choice;
            displayMainDashboard();
            cin >> choice;
            
            if (choice == 0) {
                // Other Options
                int optionChoice;
                do {
                    displayOtherOptions();
                    cin >> optionChoice;
                    
                    if (optionChoice == 0) {
                        break; // Back to dashboard
                    }
     
                    handleOtherOptions(optionChoice);
                    
                    // If logged out, break from dashboard loop
                    if (currentLoggedInCustomer == "") {
                        break;
                    }
                } while (optionChoice != 0);
                
            } else if (choice >= 1 && choice <= 10) {
                // Product selected
                int productChoice;
                do {
                    displayProductDetails(choice);
                    cin >> productChoice;
                    
                    if (productChoice == 3) {
                        break; // Back to product list
                    } else if (productChoice == 1) {
                        // Delivery
                        handleProductOrder(choice, 1);
                        break;
                    } else if (productChoice == 2) {
                        // Pickup
                        handleProductOrder(choice, 2);
                        break;
                    } else {
                        cout << "Invalid choice! Press Enter to continue...";
                        cin.ignore();
                        cin.get();
                    }
                } while (productChoice != 3);
                
            } else {
                cout << "Invalid choice! Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
    
    // Save all data before exiting
    saveAllData();
    cout << "Thank you for Using our Services!" << endl;
    return 0;
}