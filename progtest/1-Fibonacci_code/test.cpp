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

void print_data(const vector<bool> &data) {
    cout << "bin data: ";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 8 == 0 and i != 0) {
            cout << " ";
        }
        cout << data[i];
    }
    cout << endl;
}

// byte = 0000 0000
// byte << 1
// -> 0000 000 '0'
// byte | data[0+0]
// -> 0000 000 'data[0]'
bool save_file(const vector<bool> &data, const char *outFile) {
    ofstream ofs(outFile, ios::out | ios::binary);
    if (!ofs || !ofs.good() || !ofs.is_open()) {
        cout << "savefile" << endl;
        return false;
    }

    char byte = 0;
    // separate into 8 bits sequences
    for (size_t i = 0; i < data.size(); i += 8) {
        // convert 8 bits sequence to char
        for (size_t j = 0; j < 8 && (i + j) < data.size(); ++j) {
            // bitshift left
            byte <<= 1;
            // bit OR
            byte |= data[i + j];
        }
        // write byte to file
        ofs.write(&byte, 1);
        byte = 0;
    }

    ofs.close();
    if (!ofs.good()) {
        cout << "ofs.good\n";
        return false;
    }
    return true;
}

void fill_data(vector<bool> &data, vector<size_t> &pos) {
    size_t j = pos.size() - 1;
    for (size_t i = 0; i <= pos[0]; ++i) {
        if (pos[j] == i) {
            j--;
            data.push_back(1);
        } else {
            data.push_back(0);
        }
    }
    data.push_back(1);
}

int nearest_fib(int dec, size_t &n) {
    n = 0;
    int f1 = 0, f2 = 1, f3 = 1;
    if (dec == f1) {
        n = 0;
        return f1;
    }
    if (dec == f2) {
        n = 1;
        return f2;
    }
    while (dec >= f3) {
        f1 = f2;
        f2 = f3;
        n++;
        f3 = f1 + f2;
    }
    if (dec == f3)
        return f3;
    return f2;
}

void fib_code(vector<bool> &data, int dec) {
    vector<size_t> pos;
    size_t n = 0;
    int near_fib = 0;
    if (dec == 0) {
        pos.push_back(1);
    } else {
        dec++;
    }

    while (dec > 0) {
        near_fib = nearest_fib(dec, n);
        pos.push_back(n - 1);
        dec = dec - near_fib;
    }

    fill_data(data, pos);
    pos.clear();
}

void UTF8_to_bin(vector<bool> &data, unsigned char &c, int flag, int &pause, int a) {
    int bin = 0;
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
                cout << "default switch case " << flag << endl;
                break;
        }
        pause++;
    }
}

int get_dec(const vector<bool> &data) {
    //                   2^(size-1)
    int dec = 0, pow = 1 << int(data.size() - 1);
    for (const auto &item: data) {
        dec += item * pow;
        pow /= 2;
    }
    return dec;
}

void fill_zeros(vector<bool> &data) {
    if ((data.size() % 8) == 0)
        return;
    while ((data.size() % 8) != 0)
        data.push_back(0);
}

bool UTF8_encode(char a, unsigned char c, ifstream &ifs, int &pause, vector<bool> &data) {
    // wrong mask
    if ((c & 0b11111000) == 0b11111000 ||
        (c & 0b11111100) == 0b11111100 ||
        (c & 0b11111110) == 0b11111110 ||
        (c & 0b11111111) == 0b11111111) {
        cout << "wrong mask" << endl;
        return false;
    }
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    else if ((c & 0b11110000) == 0b11110000) {
        UTF8_to_bin(data, c, 4, pause, 7);
        ifs.get(a);
        c = (unsigned char) a;
        if ((c & 0b10000000) == 0b10000000) {
            UTF8_to_bin(data, c, 4, pause, 7);
            ifs.get(a);
            c = (unsigned char) a;
            if ((c & 0b10000000) == 0b10000000) {
                UTF8_to_bin(data, c, 4, pause, 7);
                ifs.get(a);
                c = (unsigned char) a;
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
        ifs.get(a);
        c = (unsigned char) a;
        if ((c & 0b10000000) == 0b10000000) {
            UTF8_to_bin(data, c, 3, pause, 7);
            ifs.get(a);
            c = (unsigned char) a;
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
        ifs.get(a);
        c = (unsigned char) a;
        if ((c & 0b10000000) == 0b10000000) {
            UTF8_to_bin(data, c, 2, pause, 7);
        } else {
            cout << "error" << endl;
            return false;
        }
    }
        // 0xxxxxxx
    else if ((c & 0b10000000) == 0b00000000) {
        UTF8_to_bin(data, c, 1, pause, 7);
        // char is out of range {0 - 0x10ffff}
    } else {
        cout << "out of range" << endl;
        return false;
    }
    return true;
}

// (c>>7) & 1 = first bit in 'c'
// c = abcd efgh
// c >> 7
// -> 'a'
// 'a' & 1
// -> value of first bit
bool read_UTF8(ifstream &ifs, const char *outFile) {
    vector<bool> data;
    vector<bool> to_save;
    int pause = 0, dec = 0;
    unsigned char c;
    // TODO: unsinged char
    // get each char/byte from file
    for (char a; ifs.get(a);) {
        c = (unsigned char) a;
        data = {};
        pause = 0;

        if (!UTF8_encode(a, c, ifs, pause, data)) {
            cout << "UTF8_encode" << endl;
            return false;
        }

        dec = get_dec(data);
        fib_code(to_save, dec);
    }

    fill_zeros(to_save);

    if (!save_file(to_save, outFile)) {
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

    if (!read_UTF8(ifs, outFile)) {
        cout << "read_UTF8\n";
        return false;
    }

    ifs.close();
    return true;
}

// 00000000 00000101 00001010
void get_dec(size_t data, vector<bool> &to_save) {
    size_t bin = 0;
    vector<bool> tmp = {};
    if (data == 0) {
        to_save.push_back(0);
        fill_zeros(to_save);
    }
    while (data > 0) {
        bin = data % 2;
        tmp.insert(tmp.begin(), bin);
        data /= 2;
    }
    fill_zeros(tmp);
    for (const auto &item: tmp) {
        to_save.push_back(item);
    }
}

bool fib_encode(size_t data, vector<bool> &to_save) {
    // 0xxxxxxx
    if (data < 128) {
        get_dec(data, to_save);


        // 110xxxxx 10xxxxxx
    } else if (data < 2048) {
        get_dec(data, to_save);
        //print_data(to_save);

        // 1110xxxx 10xxxxxx 10xxxxxx
    } else if (data < 65536) {


        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    } else if (data < 1114112) {


        // char is out of range {0 - 0x10ffff}
    } else {
        return false;
    }

    //get_dec(data, to_save, i);
    return true;
}

bool dec_to_bin(vector<size_t> &data, const char *outFile) {
    vector<bool> to_save;
    for (size_t &item: data) {
        if (!fib_encode(item, to_save)) {
            cout << "fib_encode" << endl;
            return false;
        }
    }

    if (!save_file(to_save, outFile)) {
        cout << "chyba ulozeni" << endl;
        return false;
    }
    return true;
}

bool fib_decode(vector<bool> &data, const char *outFile) {
    vector<size_t> to_save;
    size_t f1 = 0, f2 = 1, f3 = 1;
    size_t dec = 0, last = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == 1 && last == 1) {
            last = 0;
            f1 = 0;
            f2 = 1;
            f3 = 1;
            to_save.push_back(dec - 1);
            dec = 0;
        } else if (data[i] == 1 && last == 0) {
            dec += f3;
            f1 = f2;
            f2 = f3;
            f3 = f1 + f2;
            last = 1;
        } else {
            last = 0;
            f1 = f2;
            f2 = f3;
            f3 = f1 + f2;
        }
    }

    cout << "to decode ";
    for (const auto &item: to_save) {
        cout << item << ", ";
    }
    cout << endl;

    if (!dec_to_bin(to_save, outFile)) {
        cout << "dec_to_bin" << endl;
        return false;
    }

    return true;
}

bool read_fib(ifstream &ifs, const char *outFile) {
    vector<bool> data;
    int bin = 0;
    unsigned char c;
    for (char a; ifs.get(a);) {
        c = (unsigned char) a;
        // converting byte into bits
        for (int i = 0; i < 8; ++i) {
            // bitshift right
            bin = c >> i;
            // bit AND
            bin &= 1;
            data.push_back(bin);
        }
    }

    if (!fib_decode(data, outFile)) {
        cout << "!decode" << endl;
        return false;
    }

    return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    ifstream ifs(inFile, ios::binary);
    if (!ifs || !ifs.is_open() || !ifs.good()) {
        cout << "!file" << endl;
        return false;
    }

    if (!read_fib(ifs, outFile)) {
        cout << "read_fib\n";
        return false;
    }

    ifs.close();
    return true;
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
    long file_size1 = a.tellg();
    b.seekg(0, ios::end);
    long file_size2 = b.tellg();
    if (file_size1 != file_size2) {
        cout << "SIZE\nout = " << file_size1 << " == " << file_size2 << " = dst" << endl;
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
    return true;
}

int main() {
    assert (utf8ToFibonacci("example/src_0.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_0.fib"));

    assert (utf8ToFibonacci("example/src_1.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_1.fib"));
    assert (utf8ToFibonacci("example/src_2.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_2.fib"));
    assert (utf8ToFibonacci("example/src_3.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_3.fib"));
    assert (utf8ToFibonacci("example/src_4.utf8", "output.fib")
            && identicalFiles("output.fib", "example/dst_4.fib"));
    assert (!utf8ToFibonacci("example/src_5.utf8", "output.fib"));

    cout << "\n\n\n";
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
