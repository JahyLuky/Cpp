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

void print_data(const vector<int> &data) {
    cout << "bin data: ";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 8 == 0 and i != 0) {
            cout << " ";
        }
        cout << data[i];
    }
    cout << endl;
}

int nearest_fib(int dec) {
    int f1 = 0, f2 = 1, f3 = 1;
    if (dec == f1)
        return f1;
    if (dec == f2)
        return f2;
    while (dec >= f3) {
        f1 = f2;
        f2 = f3;
        f3 = f1 + f2;
    }
    if (dec == f3)
        return f3;
    return f2;
}

void fib_code(int dec) {
    vector<int> mem;
    cout << "fib(" << dec << ") = " << nearest_fib(dec) << endl;
}

bool save_file(const vector<int> &data1, const char *outFile) {
    ofstream ofs(outFile, ios::out | ios::binary);
    if (!ofs || !ofs.good() || !ofs.is_open()) {
        cout << "savefile" << endl;
        return false;
    }

    char byte = 0;
    // byte = 0000 0000
    // byte << 1
    // -> 0000 000 '0'
    // byte | data[0+0]
    // -> 0000 000 'data[0]'

    vector<int> data = {0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0};
    //cout << "ofs << ";

    // separate into 8 bits sequences
    for (size_t i = 0; i < data.size(); i += 8) {
        // convert 8 bits sequence to char
        for (size_t j = 0; j < 8 && (i + j) < data.size(); ++j) {
            // bitshift left
            byte <<= 1;
            // bit OR
            byte |= data[i + j];
        }
        //cout << byte << endl;
        //cout << i << " " << data.size() << endl;

        // write byte to file
        ofs.write(&byte, 1);

        byte = 0;
    }

    ofs.close();
    if (!ofs.good())
        return false;
    return true;
}

void UTF8_to_bin(vector<int> &data, char &c, int flag, int &pause, int a) {
    int bin = 0;
    //cout << "data: " << c << endl;
    //cout << "bin: ";
    // converting byte into bits
    for (int i = a; i >= 0; --i) {
        // bitshift right
        bin = c >> i;
        // bit AND
        bin &= 1;

        switch (flag) {
            case 1:
                data.push_back(bin);
                break;
            case 2:
                if (pause == 0 || pause == 1 || pause == 2
                    || pause == 8 || pause == 9) {
                    break;
                }
                data.push_back(bin);
                break;
            case 3:
                if (pause == 0 || pause == 1 || pause == 2 || pause == 3
                    || pause == 8 || pause == 9 || pause == 16 || pause == 17) {
                    break;
                }
                data.push_back(bin);
                break;
            case 4:
                if (pause == 0 || pause == 1 || pause == 2 || pause == 3
                    || pause == 4 || pause == 8 || pause == 9 || pause == 16
                    || pause == 17 || pause == 24 || pause == 25) {
                    break;
                }
                data.push_back(bin);
                break;
            default:
                cout << "necof " << flag << endl;
                break;
        }
        pause++;
    }
}

int get_dec(const vector<int> &data) {
    //                   2^(size-1)
    int dec = 0, pow = 1 << int(data.size() - 1);
    for (const auto &item: data) {
        dec += item * pow;
        //cout << dec << " = " << item << " * "<< pow <<  endl;
        pow /= 2;
    }
    return dec;
}

// (c>>7) & 1 = first bit in 'c'
// c = abcd efgh
// c >> 7
// -> 'a'
// 'a' & 1
// -> value of first bit
bool read_UTF8(ifstream &ifs, const char *outFile) {
    vector<int> data;
    int pause = 0, dec = 0;
    // TODO: static cast unsinged char
    // get each char/byte from file
    for (char c; ifs.get(c);) {
        data = {};
        pause = 0;

        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((c & 0b11110000) == 0b11110000) {
            UTF8_to_bin(data, c, 4, pause, 7);
            ifs.get(c);
            if ((c & 0b10000000) == 0b10000000) {
                UTF8_to_bin(data, c, 4, pause, 7);
                ifs.get(c);
                if ((c & 0b10000000) == 0b10000000) {
                    UTF8_to_bin(data, c, 4, pause, 7);
                    ifs.get(c);
                    if ((c & 0b10000000) == 0b10000000) {
                        UTF8_to_bin(data, c, 4, pause, 7);
                    } else {
                        cout << "error" << endl;
                        return false;
                    }
                } else {
                    cout << "error" << endl;
                    return false;
                }
            } else {
                cout << "error" << endl;
                return false;
            }
            // 1110xxxx 10xxxxxx 10xxxxxx
        } else if ((c & 0b11100000) == 0b11100000) {
            UTF8_to_bin(data, c, 3, pause, 7);
            ifs.get(c);
            if ((c & 0b10000000) == 0b10000000) {
                UTF8_to_bin(data, c, 3, pause, 7);
                ifs.get(c);
                if ((c & 0b10000000) == 0b10000000) {
                    UTF8_to_bin(data, c, 3, pause, 7);
                } else {
                    cout << "error" << endl;
                    return false;
                }
            } else {
                cout << "error" << endl;
                return false;
            }
        }
            // 110xxxxx 10xxxxxx
        else if ((c & 0b11000000) == 0b11000000) {
            UTF8_to_bin(data, c, 2, pause, 7);
            ifs.get(c);
            if ((c & 0b10000000) == 0b10000000) {
                UTF8_to_bin(data, c, 2, pause, 7);

            } else {
                cout << "error" << endl;
                return false;
            }
        }
            // UTF-8 out of range
        else {
            if ((c >> 7) == 1) {
                cout << "error c>>7" << endl;
                return false;
            }
            /*
            if (ifs.peek() == EOF) {
                cout << "EOF\n";
                //break;
            }*/
            UTF8_to_bin(data, c, 1, pause, 7);
        }
        print_data(data);
        dec = get_dec(data);
        cout << "dec = " << dec << endl;

        fib_code(dec);
    }
    if (!save_file(data, outFile)) {
        cout << "chyba ulozeni" << endl;
        return false;
    }
    return true;
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
    ifstream ifs(inFile, ios::binary);
    if (!ifs || !ifs.is_open() || !ifs.good()) {
        cout << "!file" << endl;
        return false;
    }
    /*
    if (ifs.peek() == EOF) {
        return false;
    }*/

    if (!read_UTF8(ifs, outFile)) {
        return false;
    }

    ifs.close();
    if (!ifs.good())
        return false;
    return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    return false;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1, const char *file2) {
    ifstream a(file1, ios::binary);
    ifstream b(file2, ios::binary);

    if (!a || !b) {
        cout << "!a\n";
        return false;
    }

    // get size of files
    a.seekg(0, ios::end);
    int file_size1 = a.tellg();
    b.seekg(0, ios::end);
    int file_size2 = b.tellg();
    cout << "size1 = " << file_size1 << " == " << file_size2 << " = size2" << endl;
    if (file_size1 != file_size2) {
        cout << "size\n";
        return false;
    }

    // compare each char in files
    char c_a, c_b;
    int bin = 0;
    for (; a.get(c_a);) {
        b.get(c_b);
        cout << c_a << " == " << c_b << endl;
        if (a.fail() || b.fail() || c_a != c_b) {
            cout << "a!=b\n";
            return false;
        }

        for (int i = 7; i >= 0; --i) {
            bin = (c_a >> i) & 1;
            cout << bin;
        }
    }

    a.close();
    b.close();
    //cout << "identical files" << endl;
    return true;
}

int main() {
    /*if (identicalFiles("output.fib", "example/dst_0.fib")) {
        cout << "identical files" << endl;
    } else {
        cout << "not identical files" << endl;
    }*/

    cout << "-------------\ninput: " << endl;
    utf8ToFibonacci("example/src_2.utf8", "output.fib");
    //cout << "b.txt" << endl;
    //utf8ToFibonacci("b.txt", "output.fib");
    /*
     *
    if (identicalFiles("output.fib", "example/dst_0.fib")) {
        cout << "identical files" << endl;
    } else {
        cout << "not identical files" << endl;
    }

    cout << "output: " << endl;
    utf8ToFibonacci("example/dst_0.fib", "output.fib");

    */

    //assert (utf8ToFibonacci("example/src_0.utf8", "output.fib")
    //        && identicalFiles("output.fib", "example/dst_0.fib"));

/*
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
*/
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
