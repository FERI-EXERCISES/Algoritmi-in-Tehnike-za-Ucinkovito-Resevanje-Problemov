#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

struct Cushion {
    int x, y, length;

    Cushion(int x, int y, int length) : x(x), y(y), length(length) {}
};

class CushionManager {
private:
    std::vector<int> segmentTree;
    std::map<int, Cushion> cushionMap;
    int counter = 1;
    int S;
    int V;

    // Funkcija za posodabljanje drevesa; dodaja ali odstranjuje najmanjši Y
    void update(int node, int start, int end, int x, int length, int y, bool adding) {
        if (start > x + length - 1 || end < x) return; // Trenutni segment se ne prekriva z blazino

        if (start == end) {
            if (adding) {
                // Pri dodajanju blazine nastavimo Y, če je trenutni V ali manjši od Y
                if (segmentTree[node] == V) segmentTree[node] = y;
                else segmentTree[node] = std::min(segmentTree[node], y);
            } else {
                // Ponastavitev na višino zidu in ponovno preverjanje za najmanjši Y med preostalimi blazinami
                segmentTree[node] = V;
                for (auto const &[key, val]: cushionMap) {
                    if (val.x <= start && start < val.x + val.length) {
                        segmentTree[node] = std::min(segmentTree[node], val.y);
                    }
                }
            }
        } else {
            int mid = (start + end) / 2;
            update(node * 2, start, mid, x, length, y, adding);
            update(node * 2 + 1, mid + 1, end, x, length, y, adding);
            segmentTree[node] = std::max(segmentTree[node * 2], segmentTree[node * 2 + 1]);
        }
    }

public:
    CushionManager(int width, int height) : S(width), V(height) {
        segmentTree.assign(4 * width, height); // Inicializacija z višino zidu
    }

    void addCushion(int y, int x, int length) {
        update(1, 0, S - 1, x, length, y, true);
        cushionMap.insert({counter++, Cushion(x, y, length)});
    }

    void removeCushion(int order) {
        if (order < 1 || order > counter) {
            std::cout << "Neveljaven vrstni red za odstranitev." << std::endl;
            return;
        }

        Cushion c = cushionMap.extract(order).mapped();

        // Odstrani blazino
        update(1, 0, S - 1, c.x, c.length, c.y, false);

        // Po odstranitvi blazine jo tudi odstranimo iz seznama
        cushionMap.erase(order);
    }

    int getLength() {
        return segmentTree[1];
    }
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Uporaba: " << argv[0] << " ime_datoteke_z_vhodi.txt" << std::endl;
        return 1;
    }

    //argv[1] = "testni_primer1.txt";

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Napaka pri odpiranju datoteke!" << std::endl;
        return 1;
    }

    int N, V, S;
    inputFile >> N >> V >> S;

    //init polje
    //std::cout << "N: " << N << " V: " << V << " S: " << S << std::endl;
    CushionManager cm(S, V);

    char commandType;
    for (int i = 0; i < N; ++i) {
        inputFile >> commandType;
        if (commandType == '+') {
            //dodaj blazino
            int Y, X, D;
            inputFile >> Y >> X >> D;
            //std::cout << "+ " << Y << " " << X << " " << D << "\t";
            cm.addCushion(Y, X, D);

        } else if (commandType == '-') {
            //odstrani blazino indeksa K-1
            int K;
            inputFile >> K;
            //std::cout << "- " << K << "\t";
            cm.removeCushion(K);

        }

        //print state
        //std::cout << "Output: " << cm.getLength() << std::endl;
        std::cout << cm.getLength() << std::endl;

    }

    inputFile.close(); // Zapiranje datoteke
    return 0;
}