//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Anthony Moran
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>  // for std::remove

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Bid aBid) : Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================
class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    void destroyTree(Node* node);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // FixMe (1): initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // FixMe (2): recurse from root deleting every node
    destroyTree(root);
}

void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // FixMe (3a): call inOrder and pass root
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // FixMe (4a): call postOrder and pass root
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // FixMe (5a): call preOrder and pass root
    preOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // FixMe (6a): implement inserting
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // FixMe (7a): implement removing
    root = removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FixMe (8): search tree
    Node* current = root;

    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        if (bidId < current->bid.bidId) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    // not found
    return Bid();
}

/**
 * Add a bid to some node (recursive)
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    if (bid.bidId < node->bid.bidId) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->bid.bidId << ": " << node->bid.title << " | "
            << node->bid.amount << " | " << node->bid.fund << endl;
        inOrder(node->right);
    }
}

void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->bid.bidId << ": " << node->bid.title << " | "
            << node->bid.amount << " | " << node->bid.fund << endl;
    }
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        cout << node->bid.bidId << ": " << node->bid.title << " | "
            << node->bid.amount << " | " << node->bid.fund << endl;
        preOrder(node->left);
        preOrder(node->right);
    }
}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    if (node == nullptr) return node;

    if (bidId < node->bid.bidId) {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId > node->bid.bidId) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        // found node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        else if (node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            // two children: find min on right
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
}

void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);

    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << endl;

    try {
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    string csvPath, bidKey;
    switch (argc) {
    case 2: csvPath = argv[1]; bidKey = "98223"; break;
    case 3: csvPath = argv[1]; bidKey = argv[2]; break;
    default: csvPath = "eBid_Monthly_Sales.csv"; bidKey = "98223";
    }

    clock_t ticks;
    BinarySearchTree* bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            ticks = clock();
            loadBids(csvPath, bst);
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();
            bid = bst->Search(bidKey);
            ticks = clock() - ticks;
            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    delete bst;
    return 0;
}