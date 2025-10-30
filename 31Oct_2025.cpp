#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> computeLPS(const string& pat) {
    int m = pat.length();
    vector<int> lps(m, 0);
    int len = 0;
    for (int i = 1; i < m; i++) {
        while (len > 0 && pat[i] != pat[len]) {
            len = lps[len - 1];
        }
        if (pat[i] == pat[len]) {
            len++;
        }
        lps[i] = len;
    }
    return lps;
}

vector<int> KMPSearch(const string& txt, const string& pat) {
    vector<int> indices;
    int n = txt.length(), m = pat.length();
    if (m == 0) return indices;

    vector<int> lps = computeLPS(pat);
    int i = 0, j = 0;

    while (i < n) {
        if (pat[j] == txt[i]) { i++; j++; }
        if (j == m) {
            indices.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return indices;
}

int main() {
    string text = "ABABCABCABABABABD";
    string pattern = "ABABD";
    
    vector<int> results = KMPSearch(text, pattern);
    
    if (results.empty()) {
        cout << "Pattern not found." << endl;
    } else {
        cout << "Pattern found at indices:" << endl;
        for (int index : results) {
            cout << index << endl;
        }
    }
    return 0;
}
