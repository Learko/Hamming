#include <optional>

#include <boost/dynamic_bitset.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "decode.hpp"

using namespace std;
using namespace boost::numeric;
using boost::dynamic_bitset;


static const auto parity_mat = [] {
    ublas::matrix<bool> parity_m(3, 7);
    vector<bool> parity_init = {
        1, 0, 1, 0, 1, 0, 1,
        0, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 1, 1, 1, 1
    };

    copy(parity_init.begin(),
         parity_init.end(),
         parity_m.data().begin()
    );

    return parity_m;
}();

static const auto decode_mat = [] {
    ublas::matrix<bool> decode_m(4, 7);
    vector<bool> decode_init = {
        0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 1
    };

    copy(decode_init.begin(),
         decode_init.end(),
         decode_m.data().begin()
    );

    return decode_m;
}();


optional<uint8_t> parity_check(const ublas::matrix<bool> &code_mat) {
    auto syndrome = ublas::prod(parity_mat, code_mat);

    dynamic_bitset<uint8_t> error_bs;
    for_each(syndrome.begin1(), syndrome.end1(), [&error_bs](auto v) {
        error_bs.push_back(v % 2);
    });

    auto error = error_bs.to_ulong();

    if (error == 0) { return {}; }
    else { return error - 1; }
}


vector<uint8_t> decode(const vector<uint8_t> &bytes) {
    dynamic_bitset<uint8_t> data;
    data.init_from_block_range(bytes.begin(), bytes.end());

    bool back = data[data.size() - 1]; data.pop_back();
    while (!back) {
        back = data[data.size() - 1]; data.pop_back();
    }

    dynamic_bitset<uint8_t> result;

    for (auto p = 0u; p < data.size(); p += 7) {
        ublas::matrix<bool> code_mat(7, 1);
        vector<bool> code_init;

        for (auto i = 0u; i < 7; ++i) {
            code_init.push_back(data[p+i]);
        }
        
        copy(code_init.begin(),
             code_init.end(),
             code_mat.data().begin()
        );

        if (auto error = parity_check(code_mat); error) {
            code_mat(error.value(), 0) = !code_init[error.value()];
        }

        auto prod = ublas::prod(decode_mat, code_mat);

        for_each(prod.begin1(), prod.end1(), [&result] (auto v) {
            result.push_back(v % 2);
        });
    }

    vector<uint8_t> blocks(result.num_blocks());
    to_block_range(result, blocks.begin());

    return blocks;
}