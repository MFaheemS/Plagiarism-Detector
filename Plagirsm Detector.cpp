#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

struct Match {
    int posA;
    int posB;
    int length;
};

class PlagiarismDetector {
private:
    string text;
    int lenA;
    vector<int> suffixArray;
    vector<int> lcp;

    void buildSuffixArray() {
        int n = text.size();
        suffixArray.resize(n);
        vector<int> rank(n), temp(n);

        for (int i = 0; i < n; i++) {
            suffixArray[i] = i;
            rank[i] = text[i];
        }

        for (int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int a, int b) {
                if (rank[a] != rank[b]) return rank[a] < rank[b];
                int ra = (a + k < n) ? rank[a + k] : -1;
                int rb = (b + k < n) ? rank[b + k] : -1;
                return ra < rb;
            };

            sort(suffixArray.begin(), suffixArray.end(), cmp);

            temp[suffixArray[0]] = 0;
            for (int i = 1; i < n; i++) {
                temp[suffixArray[i]] = temp[suffixArray[i - 1]];
                if (cmp(suffixArray[i - 1], suffixArray[i]))
                    temp[suffixArray[i]]++;
            }
            rank = temp;
        }
    }

    void buildLCPArray() {
        int n = text.size();
        lcp.assign(n, 0);
        vector<int> rank(n);

        for (int i = 0; i < n; i++)
            rank[suffixArray[i]] = i;

        int h = 0;
        for (int i = 0; i < n; i++) {
            if (rank[i] > 0) {
                int j = suffixArray[rank[i] - 1];
                while (i + h < n && j + h < n && text[i + h] == text[j + h])
                    h++;
                lcp[rank[i]] = h;
                if (h > 0) h--;
            }
        }
    }

public:
    vector<Match> findAllMatches(const string& docA, const string& docB) {
        text = docA + "#" + docB;
        lenA = docA.length();
        int n = text.length();

        buildSuffixArray();
        buildLCPArray();

        vector<Match> matches;

        for (int i = 1; i < n; i++) {
            int s1 = suffixArray[i - 1];
            int s2 = suffixArray[i];

            bool fromDifferentDocs =
                (s1 < lenA && s2 > lenA) ||
                (s2 < lenA && s1 > lenA);

            if (fromDifferentDocs && lcp[i] > 0) {
                int posA = (s1 < lenA) ? s1 : s2;
                int posB = (s1 > lenA) ? s1 - lenA - 1 : s2 - lenA - 1;
                matches.push_back({posA, posB, lcp[i]});
            }
        }
        return matches;
    }
};

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

int computeTotalMatchedLength(vector<Match>& matches, int lenA) {
    sort(matches.begin(), matches.end(),
         [](const Match& a, const Match& b) {
             return a.length > b.length;
         });

    vector<bool> used(lenA, false);
    int total = 0;

    for (auto& m : matches) {
        bool overlap = false;
        for (int i = 0; i < m.length; i++) {
            if (used[m.posA + i]) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            for (int i = 0; i < m.length; i++)
                used[m.posA + i] = true;
            total += m.length;
        }
    }
    return total;
}

double plagiarismPercent(int matched, int total) {
    if (total == 0) return 0.0;
    return (double)matched / total * 100.0;
}

string plagiarismLevel(double p) {
    if (p < 10) return "Very Low";
    if (p < 30) return "Low";
    if (p < 60) return "Moderate";
    return "High";
}

void analyze(const string& docA, const string& docB) {
    PlagiarismDetector detector;

    auto start = chrono::high_resolution_clock::now();
    auto matches = detector.findAllMatches(docA, docB);
    int totalMatched = computeTotalMatchedLength(matches, docA.length());
    auto end = chrono::high_resolution_clock::now();

    double percentA = plagiarismPercent(totalMatched, docA.length());
    double percentB = plagiarismPercent(totalMatched, docB.length());

    cout << "\n=========== Plagiarism Report ===========" << endl;
    cout << "Total Copied Content: " << totalMatched << " characters\n";
    cout << "Document A Plagiarism: " << percentA << "% (" << plagiarismLevel(percentA) << ")\n";
    cout << "Document B Plagiarism: " << percentB << "% (" << plagiarismLevel(percentB) << ")\n";
    cout << "Execution Time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";
    cout << "========================================\n";
}

int main() {
    cout << "===== Plagiarism Detection System =====\n";
    cout << "1. Run test case\n";
    cout << "2. Compare files\n";
    cout << "Choose option: ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        analyze(
            "algorithm design and analysis of algorithms",
            "analysis requires good algorithm knowledge"
        );
    } else if (choice == 2) {
        string f1, f2;
        cout << "Enter file A path: ";
        getline(cin, f1);
        cout << "Enter file B path: ";
        getline(cin, f2);

        string docA = readFile(f1);
        string docB = readFile(f2);

        if (!docA.empty() && !docB.empty())
            analyze(docA, docB);
    } else {
        cout << "Invalid choice\n";
    }

    return 0;
}
