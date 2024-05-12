#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Ship {
public:
    explicit Ship(int id) : id(id) {}

    int id;
    Ship *parent = nullptr;
    Ship *child = nullptr;
};

class Fleet {
private:
    std::vector<Ship *> ships;
public:
    explicit Fleet(int N) {
        for (int i = 1; i <= N; i++) {
            Ship *ship = makeSet(i);
            ships.push_back(ship);
        }
    }

    static Ship *makeSet(int id) { // Create a new set with one element
        Ship *ship = new Ship(id);
        ship->parent = nullptr;
        ship->child = nullptr;
        return ship;
    }

    static Ship *findSet(Ship *ship) { // Find the leader of the set
        if (ship->parent == nullptr) {
            return ship;
        }
        return ship->parent;
    }

    static int getSetSize(Ship *ship) { // Get set size
        int size = 0;
        ship = findSet(ship); // Find the leader of the set

        while (ship->child != nullptr) {
            size++;
            ship = ship->child;
        }

        return size;
    }

    static Ship *getLastShip(Ship *ship) {
        while (ship->child != nullptr) {
            ship = ship->child;
        }
        return ship;
    }

    static bool setParent(Ship *leader, Ship *ship) {
        if (leader->parent != nullptr) { // Leader has to be a leader
            std::cout << "Leader is not leader!" << std::endl;
            return false;
        }

        while (ship != nullptr) {
            ship->parent = leader;
            ship = ship->child;
        }

        return true;
    }

    void unionSet(Ship *ship1, Ship *ship2) {
        // Find the leaders of the sets
        Ship *set1 = findSet(ship1);
        Ship *set2 = findSet(ship2);

        if (set1 == set2) { // If the leaders are the same, the sets are already merged
            return;
        }

        // Get both sizes
//        int size1 = getSetSize(set1);
//        int size2 = getSetSize(set2);

        // Merge the smaller set into the bigger set
//        if (size1 > size2) {
        Ship *lastShip = getLastShip(set1); // Get last ship of larger set
        lastShip->child = set2; // Set last ship of larger set to point to the leader of smaller set
        if (!setParent(set1, set2)) {
            std::cerr << "Napaka pri postavljanju starša!" << std::endl;
        }
        ships.erase(std::remove(ships.begin(), ships.end(), set2), ships.end()); // Remove smaller set from array
//        }else{
//            Ship* lastShip = getLastShip(set2); // Get last ship of larger set
//            lastShip->child = set1; // Set last ship of larger set to point to the leader of smaller set
//            if(!setParent(set2, set1)) {
//                std::cout << "Napaka pri postavljanju starša!" << std::endl;
//            }
//            ships.erase(std::remove(ships.begin(), ships.end(), set1), ships.end());
//        }
    }

    Ship *findShipById(int id) { // Go through array and check all lists
        for (Ship *ship: ships) {
            Ship *current = ship;
            while (current != nullptr) {
                if (current->id == id) {
                    return current;
                }
                current = current->child;
            }
        }
        return nullptr;
    }

    static int calculateSumOfDistances(Ship *ship) {
        int sum = 0;

        Ship *leader = findSet(ship);

        if (leader == ship) {
            return 0;
        }

        Ship *current = leader;
//        std::cout << "Ships: ";
//        std::cout << current->id << " ";

        while (current != ship) {
            sum += std::abs(current->id - current->child->id) % 1000;
            current = current->child;
//            std::cout << current->id << " ";
        }

//        std::cout << " Sum: ";

        return sum;
    }
};


int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Uporaba: " << argv[0] << " ime_datoteke_z_vhodi.txt" << std::endl;
        return 1;
    }

//    argv[1] = "testni_primer1.txt";

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Napaka pri odpiranju datoteke!" << std::endl;
        return 1;
    }

    int N; // Število vojaških ladij
    inputFile >> N;

    //init ships with N (1-N)
    Fleet fleet(N);

    char commandType; // Tip ukaza ('G' ali 'C')
    while (inputFile >> commandType) { // Branje ukazov
        if (commandType == 'G') {
            int X;
            inputFile >> X;
//            std::cout << "G " << X << " - ";
            Ship *ship = fleet.findShipById(X);
            if (ship != nullptr) {
                int sum = Fleet::calculateSumOfDistances(ship);
                std::cout << sum << std::endl;
            }


        } else if (commandType == 'C') {
            int M, S;
            inputFile >> M >> S;
            Ship *ship1 = fleet.findShipById(S);
            Ship *ship2 = fleet.findShipById(M);
//            std::cout << "C " << M << " " << S << std::endl;
            if (ship1 != nullptr && ship2 != nullptr) {
                fleet.unionSet(ship1, ship2);
            } else {
                std::cout << "Ladja ne obstaja!" << std::endl;
            }

        }
    }

    inputFile.close(); // Zapiranje datoteke
    return 0;
}
