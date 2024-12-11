#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class SkipListNode
{
public:
    int score;
    string player;
    vector<SkipListNode*> forward;

    SkipListNode(int level, int score, string player)
            : score(score), player(player), forward(level + 1, nullptr) {}
};

class SkipList
{
private:
    int maxLevel;
    float probability;
    int currentLevel;
    SkipListNode* header;

    int Generate_Random_Level()
    {
        int newLevel = 0;
        while ((rand() / double(RAND_MAX)) < probability && newLevel < maxLevel) {
            newLevel++;
        }
        return newLevel;
    }

public:
    SkipList(int maxLevel, float probability)
            : maxLevel(maxLevel), probability(probability), currentLevel(0)
    {
        header = new SkipListNode(maxLevel, -1, "");
    }

    ~SkipList()
    {
        SkipListNode* current = header;
        while (current) {
            SkipListNode* temp = current;
            current = current->forward[0];
            delete temp;
        }
    }

    void insert(int score, string player)
    {
        vector<SkipListNode*> update(maxLevel + 1, nullptr);
        SkipListNode* current = header;

        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   current->forward[i]->score > score) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == nullptr || current->score != score || current->player != player)
        {
            int newLevel = Generate_Random_Level();
            if (newLevel > currentLevel) {
                for (int i = currentLevel + 1; i <= newLevel; i++) {
                    update[i] = header;
                }
                currentLevel = newLevel;
            }

            SkipListNode* newNode = new SkipListNode(newLevel, score, player);
            for (int i = 0; i <= newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    void remove(int score, string player)
    {
        vector<SkipListNode*> update(maxLevel + 1, nullptr);
        SkipListNode* current = header;

        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   current->forward[i]->score > score) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->score == score && current->player == player)
        {
            for (int i = 0; i <= currentLevel; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }
            delete current;

            while (currentLevel > 0 && header->forward[currentLevel] == nullptr) {
                currentLevel--;
            }
        }
    }

    bool search(int score, string player) const
    {
        SkipListNode* current = header;
        for (int i = currentLevel; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                   current->forward[i]->score > score) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        return (current != nullptr && current->score == score && current->player == player);
    }

    void topN(int N) const
    {
        cout << "Top " << N << " players:" << endl;
        SkipListNode* current = header->forward[0];
        int count = 0;

        while (current != nullptr && count < N) {
            cout << current->player << " (Score: " << current->score << ")" << endl;
            current = current->forward[0];
            count++;
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    SkipList skipList(4, 0.5);
    int choice, score;
    string player;

    while (true) {
        cout << "\n=== Game Leaderboard Menu ===" << endl;
        cout << "1. Add/Update Player Score" << endl;
        cout << "2. Remove Player" << endl;
        cout << "3. Retrieve Top N Players" << endl;
        cout << "4. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter player name: ";
            cin >> player;
            cout << "Enter player score: ";
            cin >> score;
            skipList.insert(score, player);
            cout << "Player score updated." << endl;
        } else if (choice == 2) {
            cout << "Enter player name to remove: ";
            cin >> player;
            cout << "Enter player score: ";
            cin >> score;
            skipList.remove(score, player);
            cout << "Player removed." << endl;
        } else if (choice == 3) {
            int N;
            cout << "Enter the number of top players to retrieve: ";
            cin >> N;
            skipList.topN(N);
        } else if (choice == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
