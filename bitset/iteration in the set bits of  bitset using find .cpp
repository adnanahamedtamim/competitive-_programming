bitset<500> BS;

BS[2] = 1;
BS[10] = 1;
BS[100] = 1;

for (int i = BS._Find_first(); i < BS.size(); i = BS._Find_next(i)) {
    cout << i << '\n';
}
