#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// 1. Hashing Techniques
// Division Method:
int divisionMethod(int key, int tableSize) {
    return key % tableSize;
}

// Multiplication Method:
int multiplicationMethod(int key, int tableSize, double A) {
    double fractionalPart = key * A - int(key * A);
    return int(tableSize * fractionalPart);
}

// Mid Square Method:
int midSquareMethod(int key, int tableSize, int r) {
    int square = key * key;
    int middleDigits = (square / int(pow(10, (r - 1)))) % int(pow(10, r));
    return middleDigits % tableSize;
}

// Folding Method:
int foldingMethod(int key, int tableSize) {
    int sum = 0;
    while (key > 0) {
        sum += key % 100;
        key /= 100;
    }
    return sum % tableSize;
}

// 2. Collision Resolution Techniques
// Chaining Method:
void insertChaining(vector<int> hashTable[], int key, int tableSize) {
    int index = key % tableSize;
    hashTable[index].push_back(key);
}

void printChaining(vector<int> hashTable[], int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        cout << i << ": ";
        for (int key : hashTable[i])
            cout << key << " -> ";
        cout << "NULL\n";
    }
}

// Open Addressing Method:
void insertOpenAddressing(int hashTable[], int tableSize, int key) {
    int index = key % tableSize;
    int i = 0;
    while (hashTable[(index + i) % tableSize] != -1) {
        i++;
    }
    hashTable[(index + i) % tableSize] = key;
}

// Double Hashing:
int hash2(int key, int tableSize) {
    return 7 - (key % 7);
}

void insertDoubleHashing(int hashTable[], int tableSize, int key) {
    int index = key % tableSize;
    int i = 0;
    while (hashTable[(index + i * hash2(key, tableSize)) % tableSize] != -1) {
        i++;
    }
    hashTable[(index + i * hash2(key, tableSize)) % tableSize] = key;
}

int main() {
    const int tableSize = 10;
    int hashTable[tableSize];
    vector<int> chainHashTable[tableSize];
    fill(begin(hashTable), end(hashTable), -1);

    int keys[] = {345, 678, 910, 123, 567, 890};
    int numKeys = sizeof(keys) / sizeof(keys[0]);

    cout << "Hashing Techniques:\n";

    cout << "Division Method:\n";
    for (int key : keys) {
        cout << "Key: " << key << " -> Index: " << divisionMethod(key, tableSize) << "\n";
    }

    cout << "\nMultiplication Method:\n";
    double A = 0.75;
    for (int key : keys) {
        cout << "Key: " << key << " -> Index: " << multiplicationMethod(key, tableSize, A) << "\n";
    }

    cout << "\nMid Square Method:\n";
    for (int key : keys) {
        cout << "Key: " << key << " -> Index: " << midSquareMethod(key, tableSize, 2) << "\n";
    }

    cout << "\nFolding Method:\n";
    for (int key : keys) {
        cout << "Key: " << key << " -> Index: " << foldingMethod(key, tableSize) << "\n";
    }

    cout << "\nCollision Resolution Techniques:\n";

    cout << "Separate Chaining:\n";
    for (int key : keys) {
        insertChaining(chainHashTable, key, tableSize);
    }
    printChaining(chainHashTable, tableSize);

    cout << "\nOpen Addressing:\n";
    for (int key : keys) {
        insertOpenAddressing(hashTable, tableSize, key);
    }
    for (int i = 0; i < tableSize; i++) {
        cout << i << ": " << (hashTable[i] == -1 ? "EMPTY" : to_string(hashTable[i])) << "\n";
    }

    cout << "\nDouble Hashing:\n";
    fill(begin(hashTable), end(hashTable), -1);
    for (int key : keys) {
        insertDoubleHashing(hashTable, tableSize, key);
    }
    for (int i = 0; i < tableSize; i++) {
        cout << i << ": " << (hashTable[i] == -1 ? "EMPTY" : to_string(hashTable[i])) << "\n";
    }

    return 0;
}

