#include <iostream>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// Definition for an ordered set supporting duplicate keys via pairs {value, unique_id}
typedef tree<
    pair<int, int>,
    null_type,
    less<pair<int, int>>,
    rb_tree_tag,
    tree_order_statistics_node_update
> ordered_set;

int main() {
    ordered_set s;
    int id = 0;

    s.insert({10, ++id});
    s.insert({20, ++id});
    s.insert({20, ++id}); // handles duplicate values safely

    // 1. Get the 1st smallest element (0-indexed)
    auto it = s.find_by_order(1);
    cout << "1st element: " << it->first << "\n";

    // 2. Count elements strictly less than 20
    cout << "Count < 20: " << s.order_of_key({20, 0}) << "\n";

    return 0;
}
