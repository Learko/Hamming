#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>

using namespace std;


int main() {
    ios::sync_with_stdio(false);

    random_device rd;
    mt19937 gen(rd());

    bernoulli_distribution ber_dist(0.25f);
    uniform_int_distribution<uint8_t> uni_dist(0, 7);
    
    cin >> noskipws;
    istream_iterator<uint8_t> begin(cin), end;

    bool prev = false;

    transform(
        begin, end,
        ostream_iterator<uint8_t>(cout),
        [&] (uint8_t v) -> uint8_t {
            if (!prev && ber_dist(gen)) {
                v ^= 1 << uni_dist(gen);
                prev = true;
            }
            else {
                prev = false;
            }

            return v;
    });


    return EXIT_SUCCESS;
}