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

void save_8bits(ofstream &ofs, const vector<bool> &data) {
    char byte = 0;

    for (size_t i = 0; i < 8 && i < data.size(); ++i) {
        // bitshift left
        byte <<= 1;
        // bit OR
        byte |= data[i];
    }
    // write byte to file
    ofs.write(&byte, 1);
}

// byte = 0000 0000
// byte << 1
// -> 0000 000 '0'
// byte | data[0+0]
// -> 0000 000 'data[0]'
bool save_file(const vector<bool> &data, ofstream &ofs) {
    /*
    ofstream ofs(outFile, ios::out | ios::binary);
    if (!ofs || !ofs.good() || !ofs.is_open()) {
        cout << "savefile" << endl;
        return false;
    }
    */

    char byte = 0;
    // separate into 8 bits sequences
    for (size_t i = 0; i < 8 && i < data.size(); ++i) {
        // bitshift left
        byte <<= 1;
        // bit OR
        byte |= data[i];
    }
    // write byte to file
    ofs.write(&byte, 1);

    if (!ofs.good()) {
        //cout << "ofs.good\n";
        return false;
    }
    ofs.close();
    return true;
}

void fill_data(vector<bool> &data, vector<int> &pos) {
    int j = int(pos.size() - 1);
    //cout << "pos " << pos[0] << " j " << j << endl;
    if (pos[0] == 1 && j == 0) {
        data.push_back(1);
        data.push_back(1);
        return;
    }
    for (int i = 0; i <= pos[0]; ++i) {
        if (pos[j] == i) {
            j--;
            data.push_back(1);
        } else {
            data.push_back(0);
        }
    }
    data.push_back(1);
    //print_data(data);
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

void split_data_8bits(vector<bool> &data) {
    for (size_t i = 0; i < 4; ++i) {
        swap(data[i], data[7 - i]);
    }
    //print_data(data);
}

void fib_code(vector<bool> &data, int dec, ofstream &ofs) {
    vector<int> pos;
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
    cout << "fill (";
    fill_data(data, pos);
    int cnt = int(data.size());
    cout << cnt << ") ";
    //print_data(data);
    if (cnt >= 8) {
        while (cnt >= 8) {
            split_data_8bits(data);
            save_8bits(ofs, data);
            for (int j = 0; j < 8; ++j) {
                data.erase(data.begin());
            }
            cnt -= 8;
        }
    }
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

void fill_zeros_front(vector<bool> &data) {
    if ((data.size() % 8) == 0)
        return;
    while ((data.size() % 8) != 0)
        data.insert(data.begin(), 0);
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
    else if ((c & 0b11111000) == 0b11110000) {
        UTF8_to_bin(data, c, 4, pause, 7);
        ifs.get(a);
        if (ifs.fail()) {
            cout << "fail" << endl;
            return false;
        }
        c = (unsigned char) a;
        if ((c & 0b11000000) == 0b10000000) {
            UTF8_to_bin(data, c, 4, pause, 7);
            ifs.get(a);
            if (ifs.fail()) {
                cout << "fail" << endl;
                return false;
            }
            c = (unsigned char) a;
            if ((c & 0b11000000) == 0b10000000) {
                UTF8_to_bin(data, c, 4, pause, 7);
                ifs.get(a);
                if (ifs.fail()) {
                    cout << "fail" << endl;
                    return false;
                }
                c = (unsigned char) a;
                if ((c & 0b11000000) == 0b10000000) {
                    UTF8_to_bin(data, c, 4, pause, 7);
                } else {
                    cout << "error" << endl;
                    return false;
                }
            } else {
                //cout << "error" << endl;
                return false;
            }
        } else {
            //cout << "error" << endl;
            return false;
        }
        // 1110xxxx 10xxxxxx 10xxxxxx
    } else if ((c & 0b11110000) == 0b11100000) {
        UTF8_to_bin(data, c, 3, pause, 7);
        ifs.get(a);
        if (ifs.fail()) {
            //cout << "fail" << endl;
            return false;
        }
        c = (unsigned char) a;
        if ((c & 0b11000000) == 0b10000000) {
            UTF8_to_bin(data, c, 3, pause, 7);
            ifs.get(a);
            if (ifs.fail()) {
                //cout << "fail" << endl;
                return false;
            }
            c = (unsigned char) a;
            if ((c & 0b11000000) == 0b10000000) {
                UTF8_to_bin(data, c, 3, pause, 7);
            } else {
                //cout << "error" << endl;
                return false;
            }
        } else {
            //cout << "error" << endl;
            return false;
        }
    }
        // 110xxxxx 10xxxxxx
    else if ((c & 0b11100000) == 0b11000000) {
        UTF8_to_bin(data, c, 2, pause, 7);
        ifs.get(a);
        if (ifs.fail()) {
            //cout << "fail" << endl;
            return false;
        }
        c = (unsigned char) a;
        if ((c & 0b11000000) == 0b10000000) {
            UTF8_to_bin(data, c, 2, pause, 7);
        } else {
            //cout << "error" << endl;
            return false;
        }
    }
        // 0xxxxxxx
    else if ((c & 0b10000000) == 0b00000000) {
        UTF8_to_bin(data, c, 1, pause, 7);
        //print_data(data);

        // char is out of range {0 - 0x10ffff}
    } else {
        //cout << "out of range" << endl;
        return false;
    }
    return true;
}

void swap(bool &x, bool &y) {
    bool tmp = x;
    x = y;
    y = tmp;
}

void split_data(vector<bool> &data) {
    for (size_t i = 0; i < data.size(); i += 8) {
        for (size_t j = i; j < i + 4; ++j) {
            swap(data[j], data[i + 7 - (j - i)]);
        }
    }
    //print_data(data);
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

    //TODO: mayybe too early
    ofstream ofs(outFile, ios::out | ios::binary);
    if (!ofs || !ofs.good() || !ofs.is_open()) {
        cout << "savefile" << endl;
        return false;
    }

    // get each char/byte from file
    for (char a; ifs.get(a);) {
        if (ifs.fail()) {
            //cout << "fail" << endl;
            ofs.close();
            return false;
        }
        c = (unsigned char) a;
        data = {};
        pause = 0;

        if (!UTF8_encode(a, c, ifs, pause, data)) {
            //cout << "UTF8_encode" << endl;
            ofs.close();
            return false;
        }

        dec = get_dec(data);
        //cout << "dec " << dec << endl;
        data.clear();
        fib_code(to_save, dec, ofs);
        //print_data(to_save);

    }

    fill_zeros(to_save);
    //print_data(to_save);
    split_data(to_save);
    cout << "size: " << to_save.size() << endl;
    if (to_save.size() > 0) {
        if (!save_file(to_save, ofs)) {
            //cout << "chyba ulozeni" << endl;
            return false;
        }
    }
    return true;
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
    ifstream ifs(inFile, ios::binary);
    //if (!ifs || !ifs.is_open() || !ifs.good()) {
    if (!ifs.good()) {
        //cout << "!file" << endl;
        return false;
    }
    /*
    if (ifs.peek() == EOF) {
        return false;
    }
    */
    if (!read_UTF8(ifs, outFile)) {
        cout << "read_UTF8\n";
        return false;
    }

    ifs.close();
    return true;
}

void dec_to_bin(size_t data, vector<bool> &tmp) {
    size_t bin = 0;
    while (data > 0) {
        bin = data % 2;
        tmp.insert(tmp.begin(), bin);
        data /= 2;
    }
}

bool fib_encode(size_t data, vector<bool> &to_save, ofstream &ofs) {
    vector<bool> tmp = {};
    // 0xxxxxxx
    if (data < 128) {
        if (data == 0) {
            tmp.push_back(0);
            fill_zeros(tmp);
        } else {
            dec_to_bin(data, tmp);
            if ((tmp.size() % 8) != 0) {
                while ((tmp.size() % 8) != 0) {
                    tmp.insert(tmp.begin(), 0);
                    //tmp.push_back(0);
                }
            }
        }
        //print_data(tmp);
        //split_data(tmp);
        for (const auto &item: tmp) {
            to_save.push_back(item);
        }
        //print_data(to_save);

        save_8bits(ofs, to_save);
        for (int j = 0; j < 8; ++j) {
            to_save.erase(to_save.begin());
        }
        //print_data(to_save);
        //cout << "final\n";
        //print_data(to_save);

        // 110xxxxx 10xxxxxx
    } else if (data < 2048) {
        dec_to_bin(data, tmp);
        if ((tmp.size() % 11) != 0) {
            while ((tmp.size() % 11) != 0) {
                //tmp.push_back(0);
                tmp.insert(tmp.begin(), 0);
            }
        }
        tmp.insert(tmp.begin(), 1);
        tmp.insert(tmp.begin() + 1, 1);
        tmp.insert(tmp.begin() + 2, 0);
        tmp.insert(tmp.begin() + 8, 1);
        tmp.insert(tmp.begin() + 9, 0);
        //split_data(tmp);

        for (const auto &item: tmp) {
            to_save.push_back(item);
        }

        for (int i = 0; i < 2; ++i) {
            save_8bits(ofs, to_save);
            for (int j = 0; j < 8; ++j) {
                to_save.erase(to_save.begin());
            }
        }

        // 1110xxxx 10xxxxxx 10xxxxxx
    } else if (data < 65536) {
        dec_to_bin(data, tmp);
        if ((tmp.size() % 16) != 0) {
            while ((tmp.size() % 16) != 0)
                tmp.insert(tmp.begin(), 0);
        }
        tmp.insert(tmp.begin(), 1);
        tmp.insert(tmp.begin() + 1, 1);
        tmp.insert(tmp.begin() + 2, 1);
        tmp.insert(tmp.begin() + 3, 0);
        tmp.insert(tmp.begin() + 8, 1);
        tmp.insert(tmp.begin() + 9, 0);
        tmp.insert(tmp.begin() + 16, 1);
        tmp.insert(tmp.begin() + 17, 0);
        //split_data(tmp);

        for (const auto &item: tmp) {
            to_save.push_back(item);
        }
        for (int i = 0; i < 3; ++i) {
            save_8bits(ofs, to_save);
            for (int j = 0; j < 8; ++j) {
                to_save.erase(to_save.begin());
            }
        }

        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    } else if (data < 1114112) {
        dec_to_bin(data, tmp);
        if ((tmp.size() % 21) != 0) {
            while ((tmp.size() % 21) != 0)
                tmp.insert(tmp.begin(), 0);
        }
        tmp.insert(tmp.begin(), 1);
        tmp.insert(tmp.begin() + 1, 1);
        tmp.insert(tmp.begin() + 2, 1);
        tmp.insert(tmp.begin() + 3, 1);
        tmp.insert(tmp.begin() + 4, 0);
        tmp.insert(tmp.begin() + 8, 1);
        tmp.insert(tmp.begin() + 9, 0);
        tmp.insert(tmp.begin() + 16, 1);
        tmp.insert(tmp.begin() + 17, 0);
        tmp.insert(tmp.begin() + 24, 1);
        tmp.insert(tmp.begin() + 25, 0);
        //split_data(tmp);

        for (const auto &item: tmp) {
            to_save.push_back(item);
        }

        for (int i = 0; i < 4; ++i) {
            save_8bits(ofs, to_save);
            for (int j = 0; j < 8; ++j) {
                to_save.erase(to_save.begin());
            }
        }

        // char is out of range {0 - 0x10ffff}
    } else {
        //cout << "out of range" << endl;
        return false;
    }


    tmp.clear();
    //print_data(tmp);
    //get_dec(data, to_save, i);
    return true;
}

bool get_out(int item, ofstream &ofs) {
    vector<bool> to_save;

    if (!fib_encode(item, to_save, ofs)) {
        // << "fib_encode" << endl;
        ofs.close();
        return false;
    }

    return true;
}

bool read_fib(ifstream &ifs, const char *outFile) {
    vector<int> to_save;
    ofstream ofs(outFile, ios::out | ios::binary);
    if (!ofs || !ofs.good() || !ofs.is_open()) {
        cout << "savefile" << endl;
        return false;
    }
    int bin = 0, cnt = 0, last1 = 0, last_bin = 0;
    size_t f1 = 0, f2 = 1, f3 = 1;
    size_t dec = 0, last = 0, flag = 0, last_one = 0;
    unsigned char c;
    for (char a; ifs.get(a);) {
        if (ifs.fail()) {
            cout << "fail\n";
            return false;
        }
        c = (unsigned char) a;
        // converting byte into bits
        for (int i = 0; i < 8; ++i) {
            // bitshift right
            bin = c >> i;
            // bit AND
            bin &= 1;
            // more than 32bits
            if (cnt > 32) {
                cout << "cnt>32\n";
                return false;
            }
            if (last1 == 1 && bin == 1) {
                cnt = 0;
            } else {
                cnt++;
            }
            if (bin == 1) {
                last1 = 1;
            } else {
                last1 = 0;
            }
            //cout << bin;



            if (bin == 1 && last == 1 && last_bin == 1) {
                //cout << ",1,";
                last = 0;
                f1 = 0;
                f2 = 1;
                f3 = 1;
                cout << "dec " << dec-1 << endl;
                if (!get_out(dec - 1, ofs)) {
                    cout << "dec_to_bin" << endl;
                    return false;
                }
                dec = 0;
                last_one = 0;
                flag = 1;
            } else if (bin == 1 && last == 0) {
                //cout << "1";
                dec += f3;
                f1 = f2;
                f2 = f3;
                f3 = f1 + f2;
                last = 1;
                last_one = 1;
                flag = 0;
            } else {
                //cout << "0";
                last = 0;
                f1 = f2;
                f2 = f3;
                f3 = f1 + f2;
            }
            last_bin = bin;
        }

    }
    if (flag == 0 && last_one == 1) {
        cout << "\nmissing ending 1" << endl;
        return false;
    }
    return true;
}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {
    ifstream ifs(inFile, ios::binary);
    //if (!ifs || !ifs.is_open() || !ifs.good()) {
    if (!ifs.good()) {
        cout << "!file" << endl;
        return false;
    }
    /*
    if (ifs.peek() == EOF) {
        return false;
    }
    */
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
    a.seekg(0, a.beg);
    b.seekg(0, b.beg);
    // compare each char in files
    char c_a, c_b;
    //int bin = 0;
    for (; a.get(c_a);) {
        b.get(c_b);
        //cout << c_a << " == " << c_b << endl;
        if (a.fail() || b.fail() || c_a != c_b) {
            cout << "a!=b\n";
            return false;
        }
        /*
        for (int i = 7; i >= 0; --i) {
            bin = (c_a >> i) & 1;
            //cout << bin;
        }
         */
    }

    a.close();
    b.close();
    return true;
}

int main() {
    utf8ToFibonacci("example/src_0.utf8", "output.fib");
    cout << "1------------\n";
    utf8ToFibonacci("example/src_1.utf8", "output.fib");
    cout << "2------------\n";
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

    fibonacciToUtf8("example/src_6.fib", "output.utf8");
    cout << "---------\n";
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
    cout << "\n\n\n";
    if (fibonacciToUtf8("in.utf8", "output.utf8") == true) {
        cout << "chyba" << endl;
    }


    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */