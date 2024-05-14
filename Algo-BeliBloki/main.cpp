#include <iostream>
#include <fstream>
#include <unordered_set>
#include <filesystem>
#include <chrono>

struct Point {
    int x, y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

struct PointHash {
    std::size_t operator()(const Point &p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y << 16);
    }
};

class Blocks {
private:
    int width, height;
    std::unordered_set<Point, PointHash> blackPoints;

public:
    Blocks(int width, int height) : width(width), height(height) {}

    void addPoint(int x, int y) {
        blackPoints.insert({x, y});
    }

    bool isBlack(int x, int y) const {
        if (x < 1 || x > width || y < 1 || y > height) return true;
        return blackPoints.find({x, y}) != blackPoints.end();
    }

    bool consider(int x, int y, int dx, int dy, bool evenIfUnit) const {
        if (isBlack(x, y)) return false;
        int xx = x + dx, yy = y + dy;
        bool isUnit = isBlack(xx, yy);
        if (isUnit) {
            if (!evenIfUnit || !isBlack(x + dy, y + dx) || !isBlack(x - dy, y - dx))
                return false;
        }
        return true;
    }

    int countValidMoves() const {
        int counter = 0;
        for (int y = 1; y <= height; y++) if (consider(1, y, 1, 0, true)) counter++;
        for (int x = 1; x <= width; x++) if (consider(x, 1, 0, 1, false)) counter++;
        for (const auto &point: blackPoints) {
            if (consider(point.x + 1, point.y, 1, 0, true)) counter++;
            if (consider(point.x, point.y + 1, 0, 1, false)) counter++;
        }
        return counter;
    }
};

int Algo(std::string path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return -1;
    }

    int W, H, K;
    inputFile >> W >> H >> K;

    Blocks blocks(W, H);

    for (int i = 0; i < K; ++i) {
        int x, y;
        inputFile >> x >> y;
        blocks.addPoint(x, y);
    }
    inputFile.close();

    auto start = std::chrono::high_resolution_clock::now();
    int blox = blocks.countValidMoves();
    auto end = std::chrono::high_resolution_clock::now();

    // path
    // std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
    // W * H
    // K
    // blox

    std::cout << path << "\t" << K << "\t" << W * H << "\t" << blox << "\t" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    // graf čas/K
    // graf blox/K
    // graf čas/W*H
    // graf čas/blox


    return blox;
}

int main(int argc, char *argv[]) {
    std::string path = "beli_bloki";
    std::cout << "path" << "\t" << "K" << "\t" << "W * H" << "\t" << "blox" << "\t" << "time(us)" << std::endl;
    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        auto file_path = entry.path();
        if (file_path.extension() == ".in"){
            Algo(file_path.string());
        }
    }
    return 0;
}
