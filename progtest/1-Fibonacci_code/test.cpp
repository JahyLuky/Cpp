#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
#endif /* __PROGTEST__ */

bool check_file(const char *file) {
    return false;
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
    ifstream file(inFile, ios::binary);
    if (!file || !file.is_open() || !file.good()) {
        cout << "!file" << endl;
        return false;
    }
    /*
    if (ifs.peek() == EOF) {
        return false;
    }*/

    int dec = 0, pow = 128, bin = 0;
    for (char c; file.get(c);) {
        if (file.fail()) {
            cout << "fail" << endl;
            return false;
        }
        //cout << "data: " << c << endl;
        // converting to bits
        cout << "bin: ";
        for (int i = 7; i >= 0; --i) {
            bin = (c >> i) & 1;
            dec += bin * pow;
            cout << bin;
            //cout << bin << " * " << pow << endl;
            pow /= 2;
        }
        cout << "\nDec: " << dec << endl;
        
        // TODO: to_fib

        dec = 0;
        pow = 128;
    }


    file.close();
    return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    return false;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1, const char *file2) {
    ifstream a(file1, ios::binary);
    ifstream b(file2, ios::binary);

    if (!a || !b)
        return false;

    // get size of files
    a.seekg(0, ios::end);
    int file_size1 = a.tellg();
    b.seekg(0, ios::end);
    int file_size2 = b.tellg();

    if (file_size1 != file_size2)
        return false;

    // compare each char in files
    char c_a, c_b;
    for (; a.get(c_a);) {
        b.get(c_b);
        if (a.fail() || b.fail() || c_a != c_b)
            return false;
    }

    a.close();
    b.close();

    return true;
}

int main() {
    if (identicalFiles("a.txt", "b.txt")) {
        cout << "identical files" << endl;
    } else {
        cout << "not identical files" << endl;
    }
    cout << "output: " << endl;
    utf8ToFibonacci("example/dst_0.fib", "output.fib");
    cout << "-------------\ninput: " << endl;
    assert (utf8ToFibonacci("example/src_0.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_0.fib"));


    assert (utf8ToFibonacci("example/src_1.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_1.fib"));
    assert (utf8ToFibonacci("example/src_2.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_2.fib"));
    assert (utf8ToFibonacci("example/src_3.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_3.fib"));
    assert (utf8ToFibonacci("example/src_4.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_4.fib"));
    assert (!utf8ToFibonacci("example/src_5.utf8", "output.fib"));
    assert (fibonacciToUtf8("example/src_6.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_6.utf8"));
    assert (fibonacciToUtf8("example/src_7.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_7.utf8"));
    assert (fibonacciToUtf8("example/src_8.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_8.utf8"));
    assert (fibonacciToUtf8("example/src_9.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_9.utf8"));
    assert (fibonacciToUtf8("example/src_10.fib", "output.utf8")
            && identicalFiles("output.utf8", "example/dst_10.utf8"));
    assert (!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
