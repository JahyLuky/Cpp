#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

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
    cout << "size1 = " << file_size1 << ", size2 = " << file_size2 << endl;
    if (file_size1 != file_size2)
        return false;

    // compare each char in files
    char c_a, c_b;
    for (; a.get(c_a);) {
        b.get(c_b);
        if (a.fail() || b.fail() || c_a != c_b) {
            cout << "c_a != c_b -> " << c_a << "!=" << c_b << endl;
            return false;
        }
    }

    a.close();
    b.close();

    cout << "identicalFiles" << endl;
    return true;
}


bool dumpFile(const char *fileName) {
    ifstream ifs(fileName, ios::in | ios::binary);

    if (!ifs) {
        cout << "Cannot open " << fileName << endl;
        return false;
    }

    int offset = 0;
    char chars[11] = {0};
    for (char c; ifs.get(c);) {
        if (offset % 10 == 0) {
            if (offset > 0)
                cout << " " << chars << '\n';
            cout << setw(8) << setfill('0') << offset;
        }

        cout << ' ' << hex << setw(2) << setfill('0') << (unsigned int) (unsigned char) c;
        chars[offset % 10] = isalpha(c) ? c : '.';
        offset++;
    }
    cout << endl;
    return true;
}


int main(int argc, char *argv[]) {
    if (identicalFiles("a.txt", "b.txt")) {
        cout << "done" << endl;
    } else {
        cout << "error" << endl;
    }
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
        dumpFile(argv[i]);
    }
    return EXIT_SUCCESS;
}
  