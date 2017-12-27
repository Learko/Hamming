#include <iostream>
#include <iterator>
#include <functional>
#include <string>
#include <vector>

#include "hamming.hpp"

using namespace std;

const auto usage = "./hamming [encode|decode]"s;


int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    
    if (argc != 2) {
        cout << usage << endl;
        return EXIT_FAILURE;
    }

    function<vector<uint8_t>(const vector<uint8_t> &)> op;

    if      (argv[1] == "encode"s) { op = encode; }
    else if (argv[1] == "decode"s) { op = decode; }
    else {
        cout << usage << endl;
        return EXIT_FAILURE;
    }

    cin >> noskipws;
    istream_iterator<uint8_t> begin(cin), end;
    vector<uint8_t> bytes(begin, end);

    auto result = op(bytes);

    copy(result.begin(),
         result.end(),
         ostream_iterator<uint8_t>(cout)
    );


    return EXIT_SUCCESS;
}