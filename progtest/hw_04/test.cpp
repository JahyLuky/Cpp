#ifndef __PROGTEST__

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */


struct Versions {
    uint8_t *m_data;
    uint32_t m_size;
    uint32_t m_pos;

    //Versions() = default;

    Versions(void) {
        m_pos = 0;
        m_size = 0;
        m_data = nullptr;
    }

    Versions(const Versions &a) {
        //delete [] this;
        m_size = a.m_size;
        m_pos = a.m_pos;
        m_data = new uint8_t[a.m_size];
        for (uint32_t i = 0; i < a.m_size; ++i) {
            m_data[i] = a.m_data[i];
        }
    }

    Versions &operator=(const Versions &undoVer) {
        if (&undoVer == this)
            return *this;
        //delete [] (this);
        m_pos = undoVer.m_pos;
        m_size = undoVer.m_size;
        delete[] (m_data);
        m_data = new uint8_t[undoVer.m_size];
        for (uint32_t i = 0; i < undoVer.m_size; i++) {
            m_data[i] = undoVer.m_data[i];
        }
        return *this;
    }

    ~Versions(void) {
        //delete[] (m_data);
    }
};

class CFile {
public:
    void printy(const Versions &tmp, const char *label) {
        cout << label << endl;
        for (uint32_t i = 0; i < tmp.m_size; ++i) {
            cout << (int) tmp.m_data[i] << " ";
        }
        cout << endl;
    }

    CFile(void) {
        version_cnt = 0;
        data.m_pos = 0;
        data.m_size = 0;
        data.m_data = nullptr;
    }

    CFile(const CFile &oldFile) {
        *this = oldFile; // op = CFile
    }

    CFile &operator=(const CFile *a) {
        if (a == this) {
            return *this;
        }
        //delete[] (&data);
        data = a->data;
        version_cnt = a->version_cnt;
        return *this;
    }

    ~CFile(void) {
        delete[] (data.m_data);
    }

    bool seek(uint32_t offset) {
        if (data.m_size < offset) {
            return false;
        }
        data.m_pos = offset;
        return true;
    }

    uint32_t read(uint8_t *dst, uint32_t bytes) {
        uint8_t getUint8 = (uint8_t) bytes;
        uint8_t toRead = data.m_pos + getUint8;

        if (toRead >= data.m_size) {
            toRead = data.m_size;
        }

        uint8_t haveRead = 0;
        for (uint8_t i = data.m_pos, j = 0; i < toRead; ++i, ++haveRead, ++j) {
            //cout << (int)m_data[i] << " ";
            dst[j] = data.m_data[i];
        }
        //cout << endl;
        data.m_pos += haveRead;
        //cout << m_pos << " " << (int)haveRead << endl;
        return haveRead;
    }

    uint32_t write(const uint8_t *src, uint32_t bytes) {
        if (data.m_size == 0) {
            data.m_data = new uint8_t[bytes];
        }
        if ((data.m_pos + bytes) > data.m_size) {
            uint8_t *tmp = new uint8_t[bytes + data.m_size];
            for (uint32_t i = 0; i < data.m_size; ++i) {
                tmp[i] = data.m_data[i];
            }
            delete[] data.m_data;
            data.m_data = new uint8_t[bytes + data.m_size];
            for (uint32_t i = 0; i < data.m_size; ++i) {
                data.m_data[i] = tmp[i];
            }
            delete[] tmp;
            data.m_size += (data.m_pos + bytes - data.m_size);
        }
        for (uint32_t i = data.m_pos, j = 0; i < (data.m_pos + bytes); ++i, ++j) {
            data.m_data[i] = src[j];
            //cout << (int)m_data[i] << " ";
        }
        //cout << endl;
        data.m_pos += bytes;
        //cout << m_pos << endl;
        return bytes;
    }

    void truncate(void) {
        data.m_size = data.m_pos;
    }

    uint32_t fileSize(void) const {
        return data.m_size;
    }

    void addVersion(void) {
        version_cnt++;
        verze[version_cnt] = new CFile(*this);
        verze[version_cnt]->version_cnt = version_cnt;
        //printy(verze[version_cnt]->data, "add: ");
//        verze[version_cnt].m_pos = data.m_pos;
//        verze[version_cnt].m_size = data.m_size;
//        verze[version_cnt].m_data = new uint8_t(data.m_size);
//        for (uint32_t i = 0; i < data.m_size; i++) {
//            verze[version_cnt].m_data = data.m_data;
//        }
    }

    bool undoVersion(void) {
        if ((version_cnt) == 0)
            return false;
        //cout << version_cnt;
        //delete[] (data.m_data);
        data = verze[version_cnt]->data;
        //delete [] (&verze[version_cnt]);
        //printy(data, "undo: ");
        version_cnt--;
        return true;
    }
    //-----------------------------------------------------------------------------
private:
    Versions data;
    size_t version_cnt;
    CFile **verze;
};

#ifndef __PROGTEST__

bool writeTest(CFile &x, const initializer_list<uint8_t> &data, uint32_t wrLen) {
    return x.write(data.begin(), data.size()) == wrLen;
}

bool readTest(CFile &x, const initializer_list<uint8_t> &data, uint32_t rdLen) {
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