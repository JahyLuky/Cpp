#ifndef __PROGTEST__

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */

size_t g_id = 0;

class CFile {
public:
    void printy(const CFile &tmp, const char *label) {
        cout << label << endl;
        for (uint32_t i = 0; i < tmp.prev->m_size; ++i) {
            cout << (int) tmp.prev->m_data[i] << " ";
        }
        cout << endl;
    }

    CFile(void) {
        prev = new Versions;
        prev->m_pos = 0;
        prev->m_size = 0;
        prev->m_data = nullptr;
    }

    CFile(const CFile &tmp) {
        prev = new Versions;
        prev->m_pos = tmp.prev->m_pos;
        prev->m_size = tmp.prev->m_size;
        prev->m_data = new uint8_t[tmp.prev->m_size];
        for (uint32_t i = 0; i < tmp.prev->m_size; ++i) {
            prev->m_data[i] = tmp.prev->m_data[i];
            //cout << (int)prev->m_data[i] << " ";
        }
        //cout << endl;
    }

    ~CFile(void) {
        delete[] (prev->m_data);
    }

    CFile &operator=(CFile &tmp) {
        if (&tmp == this) {
            return *this;
        }
        delete[] (tmp.prev->m_data);
        prev->m_data = new uint8_t[tmp.prev->m_size];
        for (uint32_t i = 0; i < tmp.prev->m_size; i++) {
            prev->m_data[i] = tmp.prev->m_data[i];
        }
        prev->m_pos = tmp.prev->m_pos;
        prev->m_size = tmp.prev->m_size;
        return *this;
    }

    bool seek(uint32_t offset) {
        if (prev->m_size < offset) {
            return false;
        }
        prev->m_pos = offset;
        return true;
    }

    uint32_t read(uint8_t *dst, uint32_t bytes) {
        uint8_t getUint8 = (uint8_t) bytes;
        uint8_t toRead = prev->m_pos + getUint8;

        if (toRead >= prev->m_size) {
            toRead = prev->m_size;
        }

        uint8_t haveRead = 0;
        for (uint8_t i = prev->m_pos, j = 0; i < toRead; ++i, ++haveRead, ++j) {
            //cout << (int)m_data[i] << " ";
            dst[j] = prev->m_data[i];
        }
        //cout << endl;
        prev->m_pos += haveRead;
        //cout << m_pos << " " << (int)haveRead << endl;
        return haveRead;
    }

    uint32_t write(const uint8_t *src, uint32_t bytes) {
        if (prev->m_size == 0) {
            prev->m_data = new uint8_t[bytes];
        }
        if ((prev->m_pos + bytes) > prev->m_size) {
            uint8_t *tmp = new uint8_t[bytes + prev->m_size];
            for (uint32_t i = 0; i < prev->m_size; ++i) {
                tmp[i] = prev->m_data[i];
            }
            delete[] prev->m_data;
            prev->m_data = new uint8_t[bytes + prev->m_size];
            for (uint32_t i = 0; i < prev->m_size; ++i) {
                prev->m_data[i] = tmp[i];
            }
            delete[] tmp;
            prev->m_size += (prev->m_pos + bytes - prev->m_size);
        }
        for (uint32_t i = prev->m_pos, j = 0; i < (prev->m_pos + bytes); ++i, ++j) {
            prev->m_data[i] = src[j];
            //cout << (int)m_data[i] << " ";
        }
        //cout << endl;
        prev->m_pos += bytes;
        //cout << m_pos << endl;
        return bytes;
    }

    void truncate(void) {
        prev->m_size = prev->m_pos;
    }

    uint32_t fileSize(void) const {
        return prev->m_size;
    }

    void addVersion(void) {
        verze[g_id].prev = new Versions;
        g_id++;
        verze[g_id].prev = prev;
        printy(verze[g_id], "Add");
    }

    bool undoVersion(void) {
        if (prev == nullptr) {
            return false;
        }
        this->prev = verze[g_id].prev;
        //cout << g_id << endl;
        printy(verze[g_id], "Undo");
        --g_id;
        return true;
    }

private:
    struct Versions {
        uint8_t *m_data;
        uint32_t m_size;
        uint32_t m_pos;

        Versions(void) {
            m_data = nullptr;
            m_size = 0;
            m_pos = 0;
        }

        Versions(const Versions &tmp) {
            m_data = new uint8_t[tmp.m_size];
            for (uint32_t i = 0; i < tmp.m_size; ++i) {
                m_data[i] = tmp.m_data[i];
            }
            m_size = tmp.m_size;
            m_pos = tmp.m_pos;
        }

        Versions &operator=(Versions &tmp) {
            if (&tmp == this) {
                return *this;
            }
            delete[] (tmp.m_data);
            m_data = new uint8_t[tmp.m_size];
            for (uint32_t i = 0; i < tmp.m_size; i++) {
                m_data[i] = tmp.m_data[i];
            }
            m_pos = tmp.m_pos;
            m_size = tmp.m_size;
            return *this;
        }
    };

    Versions *prev;
    CFile *verze;
};

#ifndef __PROGTEST__

bool writeTest(CFile &x,
               const initializer_list<uint8_t> &data,
               uint32_t wrLen) {
    return x.write(data.begin(), data.size()) == wrLen;
}

bool readTest(CFile &x,
              const initializer_list<uint8_t> &data,
              uint32_t rdLen) {
    uint8_t tmp[100];
    uint32_t idx = 0;

    if (x.read(tmp, rdLen) != data.size())
        return false;
    for (auto v: data)
        if (tmp[idx++] != v)
            return false;
    return true;
}

int main(void) {
    CFile f0;

    assert (writeTest(f0, {10, 20, 30}, 3));
    assert (f0.fileSize() == 3);
    assert (writeTest(f0, {60, 70, 80}, 3));
    assert (f0.fileSize() == 6);
    assert (f0.seek(2));
    assert (writeTest(f0, {5, 4}, 2));
    assert (f0.fileSize() == 6);
    assert (f0.seek(1));
    assert (readTest(f0, {20, 5, 4, 70, 80}, 7));
    assert (f0.seek(3));
    f0.addVersion();
    assert (f0.seek(6));
    assert (writeTest(f0, {100, 101, 102, 103}, 4));
    f0.addVersion();
    assert (f0.seek(5));
    CFile f1(f0);
    f0.truncate();
    assert (f0.seek(0));
    assert (readTest(f0, {10, 20, 5, 4, 70}, 20));
    assert (f0.undoVersion());
    assert (f0.seek(0));
    assert (readTest(f0, {10, 20, 5, 4, 70, 80, 100, 101, 102, 103}, 20));
    assert (f0.undoVersion());
    assert (f0.seek(0));
    assert (readTest(f0, {10, 20, 5, 4, 70, 80}, 20));
    assert (!f0.seek(100));
    assert (writeTest(f1, {200, 210, 220}, 3));
    assert (f1.seek(0));
    assert (readTest(f1, {10, 20, 5, 4, 70, 200, 210, 220, 102, 103}, 20));
    assert (f1.undoVersion());
    assert (f1.undoVersion());
    assert (readTest(f1, {4, 70, 80}, 20));
    assert (!f1.undoVersion());

    return 0;
}

#endif /* __PROGTEST__ */