#include <iostream>
#include <vector>
#include <string>
#include "RBT.h"
using namespace std;



int findHeight(RBN* root) {
    if (!root) {
        return -1;
    }
    int leftHeight = findHeight(root->left);
    int rightHeight = findHeight(root->right);
    return max(leftHeight, rightHeight) + 1;
}

void preorder(RBN* root, int row, int col, int height,
    vector<vector<string>>& ans) {
    if (!root) {
        return;
    }
    ans[row][col] = to_string(root->key) + (root->red ? "*" : "");
    int offset = pow(2, height - row - 1);
    if (root->left) {
        preorder(root->left, row + 1, col - offset, height, ans);
    }
    if (root->right) {
        preorder(root->right, row + 1, col + offset, height, ans);
    }
}

vector<vector<string>> treeToMatrix(RBN* root) {
    int height = findHeight(root);
    int rows = height + 1;
    int cols = pow(2, height + 1) - 1;
    vector<vector<string>> ans(rows, vector<string>(cols, ""));
    preorder(root, 0, (cols - 1) / 2, height, ans);
    return ans;
}

void print2DArray(vector<vector<string>>& arr) {

    for (auto& row : arr) {
        for (auto& cell : row) {
            if (cell.empty()) {
                cout << " ";
            }
            else {
                cout << cell;
            }
        }
        cout << endl;
    }
}



void print(RBT tree) {
    vector<vector<string>> result = treeToMatrix(tree.getRoot());
    print2DArray(result);
}

void show() {
    cout << "1-insert \n2-delete \n3-print\n0-exit\n";
};

int main() {
    RBT tree = RBT();
    //15, 10, 20, 5, 12, 18, 25, 3, 8, 11, 13
    //tree.insert(5);
    //tree.insert(3);
    //tree.insert(7);
    //tree.insert(6);
    //tree.insert(20);
    //tree.insert(10);
    //tree.insert(30);
    //tree.delete_(10);
    //tree.delete_(3);
    //tree.delete_(7);
    ////tree.delete_(10);
    
    int ch;
    do {
        show();
        cin >> ch;

        int num;
        switch (ch) {
        case 1:
            cout << "Enter the number\n";
            cin >> num;
            tree.insert(num);
            break;
        case 2:
            cout << "Enter the number\n";
            cin >> num;
            tree.delete_(num);
            break;
        case 3:
            cout << endl << endl;
            print(tree);
            break;
        case 0:
            cout << "Exiting\n";
            break;
        default:
            cout << "Invalid choice\n";
        }

        cout << endl;

    } while (ch != 0);

    return 0;
}