#include <iostream>
#include <vector>
#include <string>

std::pair<int, std::vector<std::string>> levenshtein_distance_and_operations(const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();
    
    // Initialisiere die DP-Tabelle und die Operationstabelle
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<std::vector<std::string>>> operations(m + 1, std::vector<std::vector<std::string>>(n + 1));

    // Fülle die Basisfälle
    for (int i = 1; i <= m; ++i) {
        dp[i][0] = i;
        operations[i][0] = operations[i - 1][0];
        operations[i][0].push_back("Löschen von " + std::string(1, s1[i - 1]) + " an Position " + std::to_string(i - 1));
    }
    for (int j = 1; j <= n; ++j) {
        dp[0][j] = j;
        operations[0][j] = operations[0][j - 1];
        operations[0][j].push_back("Einfügen von " + std::string(1, s2[j - 1]) + " an Position " + std::to_string(j - 1));
    }

    // Fülle die DP-Tabelle und verfolge die Operationen
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
                operations[i][j] = operations[i - 1][j - 1];
            } else {
                int delete_cost = dp[i - 1][j] + 1;
                int insert_cost = dp[i][j - 1] + 1;
                int replace_cost = dp[i - 1][j - 1] + 1;

                int min_cost = std::min({delete_cost, insert_cost, replace_cost});

                if (min_cost == delete_cost) {
                    dp[i][j] = delete_cost;
                    operations[i][j] = operations[i - 1][j];
                    operations[i][j].push_back("Löschen von " + std::string(1, s1[i - 1]) + " an Position " + std::to_string(i - 1));
                } else if (min_cost == insert_cost) {
                    dp[i][j] = insert_cost;
                    operations[i][j] = operations[i][j - 1];
                    operations[i][j].push_back("Einfügen von " + std::string(1, s2[j - 1]) + " an Position " + std::to_string(j - 1));
                } else if (min_cost == replace_cost) {
                    dp[i][j] = replace_cost;
                    operations[i][j] = operations[i - 1][j - 1];
                    operations[i][j].push_back("Ersetzen von " + std::string(1, s1[i - 1]) + " mit " + std::string(1, s2[j - 1]) + " an Position " + std::to_string(i - 1));
                }
            }
        }
    }

    return {dp[m][n], operations[m][n]};
}

int main() {
    std::string s1 = "kitten";
    std::string s2 = "sitting";
    
    auto result = levenshtein_distance_and_operations(s1, s2);
    int distance = result.first;
    std::vector<std::string> operations = result.second;
    
    std::cout << "Levenshtein-Distanz: " << distance << std::endl;
    std::cout << "Operationen:" << std::endl;
    for (const auto& op : operations) {
        std::cout << op << std::endl;
    }
    
    return 0;
}