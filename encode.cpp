#include <boost/dynamic_bitset.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "encode.hpp"

using namespace std;
using namespace boost::numeric;
using boost::dynamic_bitset;


const auto generator_mat = [] {
    ublas::matrix<bool> generator_m(7, 4);
    vector<bool> generator_init = {
        1, 1, 0, 1,
        1, 0, 1, 1,
        1, 0, 0, 0,
        0, 1, 1, 1,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    copy(generator_init.begin(),
         generator_init.end(),
         generator_m.data().begin()
    );

    return generator_m;
}();


vector<uint8_t> encode(const vector<uint8_t> &bytes) {
    vector<bool> plain;
    plain.reserve(bytes.size() * 8);

    for (auto && c : bytes) {
        for (auto i = 0u; i < 8u; ++i) {
            plain.push_back(c & (1 << i));
        }
    }

    dynamic_bitset<uint8_t> result;
    for (auto it = plain.begin(); it != plain.end(); advance(it, 4)) {
        ublas::matrix<bool> value_mat(4, 1);
        copy(it, it+4, value_mat.data().begin());

        auto prod = ublas::prod(generator_mat, value_mat);

        for_each(prod.begin1(), prod.end1(), [&result] (auto v) {
            result.push_back(v % 2);
        });
    }

    result.push_back(1);
    while (result.size() % 8 != 0) {
        result.push_back(0);
    }

    vector<uint8_t> blocks(result.num_blocks());
    to_block_range(result, blocks.begin());

    return blocks;
}