struct Node {
    long long val;
    Node() : val(LLONG_MIN) {} // Identity for max
    Node(long long v) : val(v) {}

    static Node merge(const Node& left, const Node& right) {
        return Node(max(left.val, right.val)); // Range Max combination
    }
};

//just update the node struct in the  subgrid sum template
