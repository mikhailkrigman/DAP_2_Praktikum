def levenshtein_distance_and_operations(s1, s2):
    m, n = len(s1), len(s2)
    
    # Initialisiere die DP-Tabelle und die Operationstabelle
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    operations = [[[] for _ in range(n + 1)] for _ in range(m + 1)]

    # Fülle die Basisfälle
    for i in range(1, m + 1):
        dp[i][0] = i
        operations[i][0] = operations[i - 1][0] + [f'Löschen von {s1[i - 1]} an Position {i - 1}']
    for j in range(1, n + 1):
        dp[0][j] = j
        operations[0][j] = operations[0][j - 1] + [f'Einfügen von {s2[j - 1]} an Position {j - 1}']

    # Fülle die DP-Tabelle und verfolge die Operationen
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]
                operations[i][j] = operations[i - 1][j - 1]
            else:
                delete_cost = dp[i - 1][j] + 1
                insert_cost = dp[i][j - 1] + 1
                replace_cost = dp[i - 1][j - 1] + 1

                # Wähle die Operation mit den geringsten Kosten
                min_cost = min(delete_cost, insert_cost, replace_cost)

                if min_cost == delete_cost:
                    dp[i][j] = delete_cost
                    operations[i][j] = operations[i - 1][j] + [f'Löschen von {s1[i - 1]} an Position {i - 1}']
                elif min_cost == insert_cost:
                    dp[i][j] = insert_cost
                    operations[i][j] = operations[i][j - 1] + [f'Einfügen von {s2[j - 1]} an Position {j - 1}']
                elif min_cost == replace_cost:
                    dp[i][j] = replace_cost
                    operations[i][j] = operations[i - 1][j - 1] + [f'Ersetzen von {s1[i - 1]} mit {s2[j - 1]} an Position {i - 1}']

    return dp[m][n], operations[m][n]

# Beispielanwendung
s1 = "kitten"
s2 = "sitting"
distance, operations = levenshtein_distance_and_operations(s1, s2)
print(f"Levenshtein-Distanz: {distance}")
print("Operationen:")
for op in operations:
    print(op)