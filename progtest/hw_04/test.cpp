#ifndef __PROGTEST__

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */

class CFile_version {
public:
    uint8_t *m_data;
    uint32_t m_size;
    uint32_t m_pos;
    int *m_versions;
};

class CFile {
public:
    CFile(void)
            : m_data(0), m_size(0), m_pos(0), m_versions(0) {}

    CFile(const CFile & tmp){
        m_data = (uint8_t*) malloc(tmp.m_size * sizeof(uint8_t));
        for (uint8_t i = 0; i < tmp.m_size; ++i) {
            m_data[i] = tmp.m_data[i];
        }
        m_size=(tmp.m_size);
        m_pos=tmp.m_pos;
        m_versions = tmp.m_versions;
    }

    ~CFile(void) {
        free(m_data);
    }
    // copy cons, dtor, op=

    CFile &operator=(CFile_version &tmp) {
//        if (&tmp == this) {
//            return *this;
//        }
        free(m_data);
        m_pos = tmp.m_pos;
        m_size = tmp.m_size;
        m_versions = tmp.m_versions;
        m_data = (uint8_t *) malloc((uint8_t) tmp.m_size * sizeof(uint8_t));
        for (uint8_t i = 0; i < m_size; i++) {
            m_data[i] = tmp.m_data[i];
        }
        return *this;
    }

    bool seek(uint32_t offset) {
        if (m_size < offset) {
            return false;
        }
        m_pos = offset;
        return true;
    }

    uint32_t read(uint8_t *dst, uint32_t bytes) {
        uint8_t getUint8 = (uint8_t) bytes;
        uint8_t toRead = m_pos + getUint8;

        if (toRead >= m_size) {
            toRead = m_size;
        }

        uint8_t haveRead = 0;
        for (uint8_t i = m_pos, j = 0; i < toRead; ++i, ++haveRead, ++j) {
            //cout << (int)m_data[i] << " ";
            dst[j] = m_data[i];
        }
        //cout << endl;
        m_pos += haveRead;
        //cout << m_pos << " " << (int)haveRead << endl;
        return haveRead;
    }

    uint32_t write(const uint8_t *src, uint32_t bytes) {
        uint8_t getUint8 = (uint8_t) bytes;

        if (m_size == 0) {
            m_data = (uint8_t *) malloc(getUint8 * sizeof(uint8_t));
        }
        if ((m_pos + getUint8) > m_size) {
            uint8_t resize = getUint8 + m_pos + 1;
            m_data = (uint8_t *) realloc(m_data, resize * sizeof(uint8_t));
            m_size += (m_pos + getUint8 - m_size);
        }

        for (uint8_t i = m_pos, j = 0; i < (m_pos + getUint8); ++i, ++j) {
            m_data[i] = src[j];
            //cout << (int)m_data[i] << " ";
        }
        //cout << endl;
        m_pos += getUint8;
        //cout << m_pos << endl;
        return bytes;
    }

    void truncate(void) {
        m_size = m_pos;
    }

    uint32_t fileSize(void) const {
        return m_size;
    }

    //metoda archivuje aktuální obsah souboru a aktuální pozici v souboru (vytvoří verzi).
    //Tato verze bude uložena v instanci CFile.
    void addVersion(void) {
        m_versions++;
    }

    //metoda vrátí obsah souboru a aktuální pozici v souboru do stavu,
    //ve kterém byly při odpovídajícím předchozím volání addVersion.
    //Vracet se k předchozím verzím lze vícenásobně, dokud existují předchozí archivované verze.
    //Volání undoVersion vrátí true pro úspěch, false pro neúspěch (neexistuje předchozí verze).
    bool undoVersion(void) {
        if ((m_versions--) == nullptr) {
            return false;
        }

        return true;
    }

private:
    uint8_t *m_data;
    uint32_t m_size;
    uint32_t m_pos;
    int *m_versions;
    CFile_version *arr_versions;
    /* TODO
     * alokovat nove pole o vetsi velikosti
     * vse prekopcit, pak smazat stare
     * pole <jinyCFile>
     *
     */
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
    /*
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
     */

    return 0;
}

#endif /* __PROGTEST__ */