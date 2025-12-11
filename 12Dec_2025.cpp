#include <iostream>
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
using namespace std;

int main() {
    string text = "   Hello Boost Algorithms!   ";

    boost::trim(text);        // remove leading & trailing spaces
    boost::to_upper(text);    // convert to uppercase

    vector<string> words;
    boost::split(words, text, boost::is_any_of(" ")); // split into words

    cout << "Processed text: " << text << endl;
    cout << "Words:" << endl;
    for (auto& w : words) cout << w << endl;

    return 0;
}