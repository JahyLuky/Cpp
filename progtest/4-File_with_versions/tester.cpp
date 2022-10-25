#define __PROGTEST__

#include <vector>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <iostream>

using namespace std;

#include "test.cpp"

#include <filesystem>


int version_number = 0;
std::vector<int> positions;
std::ostringstream current_version;

#define base_path "/tmp/test_file"

void add_version(std::fstream & f, int position) {
    positions.push_back(position);
    std::ostringstream new_version;
    ++version_number;
    new_version << base_path << std::setfill('0') << std::setw(5) << (version_number);
    std::filesystem::copy(current_version.str(), new_version.str());
    current_version.str(new_version.str());
    f.flush();
    f.close();

    f.open(current_version.str(), std::ios::binary | std::ios::out | std::ios::in);
    f.seekg(position, std::ios::beg);
    assert(f);
}

int undo_version(std::fstream & f) {
    std::ostringstream new_version;
    --version_number;
    new_version << base_path << std::setfill('0') << std::setw(5) << (version_number);
    std::filesystem::remove(current_version.str());
    current_version.str(new_version.str());
    int position = positions.back();
    positions.pop_back();
    f.flush();
    f.close();
    f.open(current_version.str(), std::ios::binary | std::ios::out | std::ios::in);
    f.seekg(position,std::ios::beg);
    return position;
}

bool writeTest(CFile & x, std::fstream & fout,
               const char * data,
               uint32_t wrLen) {
    fout.write(data, wrLen);
    assert(fout);
    fout.flush();
    return x.write(reinterpret_cast<const uint8_t *>(data), wrLen) == wrLen;
}

#define BUFFER_SIZE 1000

int readTest(CFile & x,
             std::fstream & fin,
             uint32_t rdLen) {
    uint8_t tmp[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    fin.read(buffer, rdLen);
    long real_count = fin.gcount();
    uint32_t idx = 0;
    unsigned int bytes = x.read(tmp, rdLen);
    if (bytes != real_count) {
        std::cerr << "Error with read length yours:" << bytes << "correct:" << real_count << std::endl;
        assert(false);
    }
    for (int i = 0; i < real_count; ++i)
        if (tmp[idx++] != buffer[i]) {
            std::cerr << "Miss match:\nCorrect:" << buffer << "\nYours:" << tmp << std::endl;
            assert(false);
        }
    return real_count;
}

int random_seek(CFile & cfile, std::fstream & file) {
    if (cfile.fileSize() > 0) {
        cfile.seek(cfile.fileSize());
        int position = rand() % (cfile.fileSize()+1);
        cfile.seek(position);
        file.seekg(position, std::ios::beg);

        if (rand() > RAND_MAX / 2) {
            cfile.addVersion();
            add_version(file, position);
        } else if (rand() > RAND_MAX / 2) {
            if (version_number) {
                assert(cfile.undoVersion());
                return undo_version(file);
            } else {
                assert(!cfile.undoVersion());
            }
        }
        return position;
    }
    assert(false);
}


void super_dooper_tester() {

    CFile cfile;
    bool only_clean = false;

    clean_up:
    int current_delete = 0;
    std::ostringstream new_version;
    new_version << base_path << std::setfill('0') << std::setw(5) << (current_delete);
    while (std::filesystem::exists(new_version.str())) {
        std::filesystem::remove(new_version.str());
        ++current_delete;
        new_version.str("");
        new_version << base_path << std::setfill('0') << std::setw(5) << (current_delete);
    }
    if (only_clean) {
        exit(0);
    }

    current_version << base_path << std::setfill('0') << std::setw(5) << version_number;
    std::ofstream out;
    out.open(current_version.str());
    out.close();

    std::fstream fout;

    fout.open(current_version.str(), std::ios::binary | std::ios::out | std::ios::in);

    // netestuje se na edge case vytvoreni verze s prazdnym souborem
    int count = 2000;
    for (int i = 0; i < count; ++i) {
        if (i % (count / 100) == 0) {
            std::cout << "Test " << i / (count / 100) << "% done cfile size:" << cfile.fileSize()
                      << " Number of versions:"
                      << version_number << std::endl;
            if (i % (count / 200) == 0) {
                while (version_number) {
                    assert(cfile.undoVersion());
                    undo_version(fout);
                }
                assert(!cfile.undoVersion());
            }
        }
        for (int k = 0; k < 10; ++k) {
            // READ seek and truncate testing
            if (cfile.fileSize() > 0) {
                int position = random_seek(cfile, fout);

                fout << std::flush;
                fout.close();

                fout.open(current_version.str(), std::ios::out|std::ios::binary | std::ios::in);
                fout.seekg(position, std::ios::beg);

                std::cout << "Reading from " << current_version.str() << std::endl;
                int bytes_read = readTest(cfile, fout, rand() % (BUFFER_SIZE - 1));
                fout.close();

                position += bytes_read;

                cfile.truncate();
                std::filesystem::resize_file(current_version.str(), position);

                fout.open(current_version.str(), std::ios::out|std::ios::binary | std::ios::in);
                fout.seekg(position, std::ios::beg);
            }
        }
        // Copy test and = test
        std::vector<CFile> files;
        for (int l = 0; l < 50; ++l) {
            files.push_back(cfile);
        }
        cfile = files[rand() % files.size()];
        files.clear();

        // Testing writes
        std::ostringstream oss;
        oss.str("");
        for (int p = 0, l = rand() % 100; p < l; ++p) {
            oss.clear();
            oss.str("");
            for (int j = 0, c = rand() % 20; j < c + 5; ++j)
                oss << (char) ('a' + rand() % 26);
            oss << " \n";
            if (rand() > RAND_MAX / 1.5 && cfile.fileSize() > 0) {
                int position = random_seek(cfile, fout);
                cfile.seek(position);
                fout.seekg(position,std::ios::beg);
                assert(fout);
            }
            std::cout << "Version:" << current_version.str()<< " at: " << fout.tellg() << " writing: " << oss.str()  << std::flush;
            assert(writeTest(cfile, fout, oss.str().c_str(), oss.str().length()));
            oss.str("");
            assert(writeTest(cfile, fout, oss.str().c_str(), oss.str().length()));
        }
    }
    only_clean = true;
    goto clean_up;
}

int main() {
    super_dooper_tester();
    return 0;
}
