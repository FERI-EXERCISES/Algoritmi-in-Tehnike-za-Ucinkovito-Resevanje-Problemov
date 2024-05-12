#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
#include <algorithm>

int main(int argc, char *argv[]) {
    /*
    if (argc < 2) {
        std::cerr << "Uporaba: " << argv[0] << " ime_datoteke_z_vhodi.txt" << std::endl;
        return 1;
    }
    */

    argv[1] = "testni_primer1.txt";

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Napaka pri odpiranju datoteke!" << std::endl;
        return 1;
    }

    int N, M, K;
    inputFile >> N >> M >> K;


    //init polje
    std::map<int, std::vector<std::tuple<int, int>>> islandsRowMap;
    for (int i = 0; i < K; ++i) {
        int X, Y, T;
        inputFile >> X >> Y >> T;

        islandsRowMap[X].push_back(std::make_tuple(Y, T));
    }

    inputFile.close(); // Zapiranje datoteke

    /*
    // Output for verification
    for (const auto& row : islandsRowMap) {
        std::cout << "Row " << row.first << ":" << std::endl;
        for (const auto& island : row.second) {
            std::cout << "\tLocation: " << std::get<0>(island) << ", Time: " << std::get<1>(island) << std::endl;
        }
    }

    for (int i = 0; i < N+5; ++i) {
        std::cout<<islandsRowMap[i].size()<<std::endl;
    }
    */

    // Inicializacija
    std::vector<int> RESULT(K + 1, std::numeric_limits<int>::max());
    std::vector<std::vector<int>> A(K + 1, std::vector<int>(2, std::numeric_limits<int>::max()));
    RESULT[0] = 0;
    A[0][0] = 0;

    // Glavna zanka za vsako vrstico
    for (int row = 1; row <= N; ++row) {
        auto it = islandsRowMap.find(row);
        if (it != islandsRowMap.end()) {
            std::vector<std::tuple<int, int>> islansInRow = it->second;

            //precompute line
            std::vector<std::tuple<int, int>> left(islansInRow.size() + 1,
                                                   std::make_tuple(std::numeric_limits<int>::max(),
                                                                   std::numeric_limits<int>::max()));
            std::vector<std::tuple<int, int>> left_(islansInRow.size() + 1,
                                                    std::make_tuple(std::numeric_limits<int>::max(),
                                                                    std::numeric_limits<int>::max()));
            std::vector<std::tuple<int, int>> right(islansInRow.size() + 1,
                                                    std::make_tuple(std::numeric_limits<int>::max(), 0));
            std::vector<std::tuple<int, int>> right_(islansInRow.size() + 1,
                                                     std::make_tuple(std::numeric_limits<int>::max(), 0));

            std::vector<int> comulative(islansInRow.size() + 1, 0);
            std::sort(islansInRow.begin(), islansInRow.end(),
                      [](const std::tuple<int, int> &a, const std::tuple<int, int> &b) {
                          return std::get<1>(a) < std::get<1>(b);
                      });
            for (int i = 0; i < islansInRow.size(); ++i) {
                comulative[i + 1] = comulative[i] + std::get<1>(islansInRow[i]);
            }

            std::sort(islansInRow.begin(), islansInRow.end(),
                      [](const std::tuple<int, int> &a, const std::tuple<int, int> &b) {
                          return std::get<0>(a) < std::get<0>(b);
                      });

            left[0] = std::make_tuple(1, 0);
            left[1] = islansInRow[0];

            right[0] = std::make_tuple(M, 0);
            right[1] = islansInRow.back();

            left_[0] = std::make_tuple(1, 0);
            left_[1] = islansInRow[0];

            right_[0] = std::make_tuple(M, 0);
            right_[1] = islansInRow.back();

            for (int i = 1; i < islansInRow.size(); i++) {
                auto copy_left = left;
                auto copy_right = right;
                for (int j = 0; j <= i; j++) {
                    auto curr_left = left[j + 1];
                    // trenutni otok + cena > en otok majn plus tega
                    if (std::get<1>(left[j + 1]) + std::get<0>(left[j + 1]) >
                        std::get<1>(copy_left[j]) + std::get<0>(islansInRow[i]) + std::get<1>(islansInRow[i])) {
                        left[j + 1] = {std::get<1>(copy_left[j]) + std::get<1>(islansInRow[i]),
                                       std::get<0>(islansInRow[i])};
                    }
                    if (std::get<1>(right[j + 1]) - std::get<0>(right[j + 1]) >
                        std::get<1>(copy_right[j]) - std::get<0>(islansInRow[islansInRow.size() - i - 1]) +
                        std::get<1>(islansInRow[islansInRow.size() - i - 1])) {
                        right[j + 1] = {
                                std::get<1>(copy_right[j]) + std::get<1>(islansInRow[islansInRow.size() - i - 1]),
                                std::get<0>(islansInRow[islansInRow.size() - i - 1])};
                    }
                    if (std::get<1>(left_[j + 1]) + (std::get<0>(left_[j + 1]) - 1) * 2 >
                        std::get<1>(copy_left[j]) + (std::get<0>(islansInRow[i]) - 1) * 2 +
                        std::get<1>(islansInRow[i])) {
                        left_[j + 1] = {std::get<1>(copy_left[j]) + std::get<1>(islansInRow[i]),
                                        std::get<0>(islansInRow[i])};
                    }
                    if (std::get<1>(right_[j + 1]) + (M - std::get<0>(right_[j + 1])) * 2 >
                        std::get<1>(copy_right[j]) + (M - std::get<0>(islansInRow[islansInRow.size() - i - 1])) * 2 +
                        std::get<1>(islansInRow[islansInRow.size() - i - 1])) {
                        right_[j + 1] = {
                                std::get<1>(copy_right[j]) + std::get<1>(islansInRow[islansInRow.size() - i - 1]),
                                std::get<0>(islansInRow[islansInRow.size() - i - 1])};
                    }
                }
            }

            //solve
            auto copy_A = A;
            for (int i = 0; i < left.size(); i++) {
                for (int j = 0; j < A.size() - i; j++) {
                    RESULT[i + j] = std::min(
                            RESULT[i + j],
                            std::min(
                                    copy_A[j][0] + std::get<1>(left[i]) + std::get<0>(left[i]) - 1,
                                    copy_A[j][1] + std::get<1>(right[i]) + M - std::get<0>(right[i])
                            ));

                    A[i + j][0] = std::min(
                            A[j + i][0],
                            std::min(
                                    copy_A[j][0] + std::get<1>(left_[i]) + (std::get<0>(left_[i]) - 1) * 2,
                                    copy_A[j][1] + comulative[i] + M - 1
                            ));

                    A[i + j][1] = std::min(
                            A[j + i][1],
                            std::min(
                                    copy_A[j][1] + std::get<1>(right_[i]) + (M - std::get<0>(right_[i])) * 2,
                                    copy_A[j][0] + comulative[i] + M - 1
                            ));
                }
            }
        } else {
            // ni otoka v vrstici
        }

        // inc A
        for (auto rowA: A) {
            rowA[0]++;
            rowA[1]++;
        }
    }

    // izpiši čas da raziščejo k otokov
    for (int i = 1; i < K; ++i) {
        std::cout << RESULT[i] << " ";
    }
    std::cout << RESULT[K];

    return 0;
}