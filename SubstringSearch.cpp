#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<int> PrefixShifts(const string& substring) {
    int m = substring.length();
    vector<int> shifts(m, 0);

    int border = 0;
    for (int i = 1; i < m; ++i) {
        while (border > 0 && substring[i] != substring[border]) {
            border = shifts[border - 1];
        }

        if (substring[i] == substring[border]) {
            border++;
        }

        shifts[i] = border;
    }

    return shifts;
}

vector<int> SuffixShifts(const string& substring) {
    int m = substring.length();
    vector<int> suffixShifts(m + 1, 0);

    vector<int> border(m + 1, 0);
    int lastPrefixPosition = m;

    for (int i = m; i > 0; --i) {
        if (i > lastPrefixPosition && border[i + m - lastPrefixPosition - 1] < i - lastPrefixPosition) {
            suffixShifts[m - i] = border[i + m - lastPrefixPosition - 1];
        } else {
            if (i < lastPrefixPosition) {
                lastPrefixPosition = i;
            }
            int j = max(0, lastPrefixPosition - i);
            while (i - j > 0 && substring[i - j - 1] == substring[m - 1 - j]) {
                ++j;
            }
            suffixShifts[m - i] = j;
            border[i - 1] = j;
        }
    }

    return suffixShifts;
}

// Функция для поиска подстроки с использованием сдвигов по префиксу шаблона и суффиксу текста
int searchSubstringOptimized(const string& text, const string& substring) {
    int n = text.length();
    int m = substring.length();

    vector<int> prefixShifts = PrefixShifts(substring);
    vector<int> suffixShifts = SuffixShifts(substring);

    int i = 0;
    int j = 0;
    while (i < n) {
        if (text[i] == substring[j]) {
            i++;
            j++;

            if (j == m) {
                return i - j;
            }
        } else {
            if (j > 0) {
                i += max(prefixShifts[j - 1], suffixShifts[j]);
                j = 0;
            } else {
                i++;
            }
        }
    }

    return -1;
}

// Функция поиска подстроки алгоритмом Бойера-Мура
vector<int> searchBoyerMoore(const string& text, const string& substring) {
    vector<int> shifts;

    vector<int> prefixShifts = PrefixShifts(substring);
    vector<int> suffixShifts = SuffixShifts(substring);

    int m = substring.size();
    int n = text.size();

    int i = m - 1;
    int j = m - 1;

    while (i < n) {
        if (substring[j] == text[i]) {
            if (j == 0) {
                shifts.push_back(i);
                i += 2 * m;
            } else {
                --i;
                --j;
            }
        } else {
            int maxShift = max(j - prefixShifts[j], suffixShifts[j]);
            i += maxShift;
            j = m - 1;
        }
    }

    return shifts;
}

int main() {
    string text = "Flowers";
    string substring = "owe";

    int result1 = searchSubstringOptimized(text, substring);

    if (result1 != -1) {
        cout << "Подстрока найдена в позиции " << result1 << endl;
    } else {
        cout << "Подстрока не найдена" << endl;
    }

    vector<int> result2 = searchBoyerMoore(text, substring);

    if (!result2.empty()) {
        cout << "Подстрока найдена на позициях:" << endl;
        for (int shift : result2) {
            cout << "позиция: " << shift << endl;
        }
    } else {
        cout << "Подстрока не найдена" << endl;
    }

    return 0;
}


