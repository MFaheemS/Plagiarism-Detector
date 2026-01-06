#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

class PlagiarismDetector {
private:
    string text;
    int lenA;
    vector<int> suffixArray;
    vector<int> lcp;

    void buildSuffixArray() {
        int n = text.length();
        suffixArray.resize(n);
        vector<int> rank(n), temp(n);

        for (int i = 0; i < n; i++) {
            suffixArray[i] = i;
            rank[i] = text[i];
        }

        for (int k = 1; k < n; k *= 2) {
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
        int n = text.length();
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
    pair<string, int> findLongestCommonSubstring(const string& docA, const string& docB) {
        text = docA + "#" + docB;
        lenA = docA.length();
        int n = text.length();

        if (n <= 1) return { "", 0 };

        buildSuffixArray();
        buildLCPArray();

        int maxLen = 0;
        int pos = 0;

        for (int i = 1; i < n; i++) {
            bool fromDifferentDocs =
                (suffixArray[i - 1] < lenA && suffixArray[i] > lenA) ||
                (suffixArray[i - 1] > lenA && suffixArray[i] < lenA);

            if (fromDifferentDocs && lcp[i] > maxLen) {
                maxLen = lcp[i];
                pos = suffixArray[i];
            }
        }

        string result = (maxLen > 0) ? text.substr(pos, maxLen) : "";
        return { result, maxLen };
    }
};

double plagiarismPercent(int matchLen, int docLen) {
    if (docLen == 0) return 0.0;
    return (static_cast<double>(matchLen) / docLen) * 100.0;
}

string plagiarismLevel(double percent) {
    if (percent < 10) return "Very Low";
    if (percent < 30) return "Low";
    if (percent < 60) return "Moderate";
    return "High";
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

void analyzeDocuments(const string& docA, const string& docB) {
    PlagiarismDetector detector;

    auto start = chrono::high_resolution_clock::now();
    auto result = detector.findLongestCommonSubstring(docA, docB);
    auto end = chrono::high_resolution_clock::now();

    double percentA = plagiarismPercent(result.second, docA.length());
    double percentB = plagiarismPercent(result.second, docB.length());

    cout << "\n=========== Plagiarism Report ===========" << endl;
    cout << "Matched Length: " << result.second << " characters" << endl;

    if (result.second > 0) {
        cout << "Longest Common Substring (preview): \""
            << result.first.substr(0, min(200, result.second)) << "\"\n";
    }

    cout << "\nDocument A Plagiarism: " << percentA << "% ("
        << plagiarismLevel(percentA) << ")" << endl;
    cout << "Document B Plagiarism: " << percentB << "% ("
        << plagiarismLevel(percentB) << ")" << endl;

    cout << "\nExecution Time: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;

    cout << "Time Complexity: O((m+n) log(m+n))" << endl;
    cout << "========================================\n";
}

void runTestCases() {
    analyzeDocuments(
        "The quick brown fox jumps over the lazy dog",
        "A lazy dog sleeps while the quick brown fox jumps"
    );

    analyzeDocuments("AAAAA", "BBBBB");

    analyzeDocuments(
        "plagiarism detection system",
        "plagiarism detection system"
    );

    analyzeDocuments(
        "algorithm design and analysis of algorithms",
        "analysis requires good algorithm knowledge"
    );
}

void runCustomFiles() {
    string fileA, fileB;
    cout << "Enter path to Document A: ";
    getline(cin, fileA);
    cout << "Enter path to Document B: ";
    getline(cin, fileB);

    string docA = readFile(fileA);
    string docB = readFile(fileB);

    if (docA.empty() || docB.empty()) {
        cout << "Error reading files." << endl;
        return;
    }

    cout << "\nDocument A Length: " << docA.length() << endl;
    cout << "Document B Length: " << docB.length() << endl;

    analyzeDocuments(docA, docB);
}

int main() {
    cout << "===== Plagiarism Detection System =====\n";
    cout << "1. Run predefined test cases\n";
    cout << "2. Compare custom documents\n";
    cout << "Choose option (1 or 2): ";

    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        runTestCases();
    }
    else if (choice == 2) {
        runCustomFiles();
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    cout << "\nProgram completed successfully.\n";
    return 0;
}
