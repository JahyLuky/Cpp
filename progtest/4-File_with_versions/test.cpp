#ifndef __PROGTEST__

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */


class CFile {
private:
    struct Version {
        uint8_t *m_data;
        uint32_t m_size;
        uint32_t m_pos;
    };

    void printy(const Version &tmp, const char *label) {
        cout << label << endl;
        for (uint32_t i = 0; i < tmp.m_size; ++i) {
            cout << (int) tmp.m_data[i] << " ";
        }
        cout << endl;
    }

    //-----------------------------------------------------------------------------
    Version **verze;
    size_t version_cnt;

    uint8_t *m_data;
    uint32_t m_size;
    uint32_t m_pos;

public:
    CFile(void) {
        version_cnt = 0;
        m_pos = 0;
        m_size = 0;
        m_data = nullptr;
        verze = nullptr;
    }

    CFile(const CFile &oldFile) {
        version_cnt = oldFile.version_cnt;
        m_pos = oldFile.m_pos;
        m_size = oldFile.m_size;

        if (m_size > 0) {
            m_data = (uint8_t *) malloc(oldFile.m_size);
        } else {
            m_data = nullptr;
        }

        for (uint32_t i = 0; i < m_size; i++) {
            m_data[i] = oldFile.m_data[i];
        }

        if (version_cnt != 0) {
            verze = (Version **) malloc(sizeof(Version *) * version_cnt);
        } else {
            verze = nullptr;
        }

        for (uint32_t i = 0; i < version_cnt; i++) {
            verze[i] = (Version *) malloc(sizeof(Version));
            verze[i]->m_pos = oldFile.verze[i]->m_pos;
            verze[i]->m_size = oldFile.verze[i]->m_size;

            if (verze[i]->m_size > 0) {
                verze[i]->m_data = (uint8_t *) malloc(oldFile.verze[i]->m_size);

                for (uint32_t j = 0; j < verze[i]->m_size; j++) {
                    verze[i]->m_data[j] = oldFile.verze[i]->m_data[j];
                }
            } else {
                verze[i]->m_data = nullptr;
            }
        }
    }

    CFile &operator=(const CFile &a) {
        if (&a == this) {
            return *this;
        }

        if (m_data != nullptr) {
            free(m_data);
            m_data = nullptr;
        }

        for (uint32_t i = 0; i < version_cnt; i++) {
            free(verze[i]->m_data);
            free(verze[i]);
        }
        free(verze);

        version_cnt = a.version_cnt;
        m_size = a.m_size;
        m_pos = a.m_pos;

        if (m_size != 0) {
            m_data = (uint8_t *) malloc(a.m_size);
        } else {
            m_data = nullptr;
        }

        if (version_cnt != 0) {
            verze = (Version **) malloc(sizeof(Version *) * version_cnt);
        } else {
            verze = nullptr;
        }

        for (uint32_t i = 0; i < version_cnt; i++) {
            verze[i] = (Version *) malloc(sizeof(Version));
            verze[i]->m_pos = a.verze[i]->m_pos;
            verze[i]->m_size = a.verze[i]->m_size;
            verze[i]->m_data = (uint8_t *) malloc(a.verze[i]->m_size);

            for (uint32_t j = 0; j < verze[i]->m_size; j++) {
                verze[i]->m_data[j] = a.verze[i]->m_data[j];
            }
        }

        return *this;
    }

    ~CFile(void) {
        if (m_data != nullptr) {
            free(m_data);
        }

        for (uint32_t i = 0; i < version_cnt; i++) {
            free(verze[i]->m_data);
            free(verze[i]);
        }

        free(verze);
    }

    bool seek(uint32_t offset) {
        if (m_size < offset) {
            return false;
        }
        m_pos = offset;
        return true;
    }

    uint32_t read(uint8_t *dst, uint32_t bytes) {
        if (m_pos == 0 && m_data == nullptr)
        {
            return 0;
        }
        uint32_t toRead = m_pos + bytes;

        if (toRead >= m_size) {
            toRead = m_size;
        }

        uint32_t haveRead = 0;
        for (uint32_t i = m_pos, j = 0; i < toRead; ++i, ++haveRead, ++j) {
            dst[j] = m_data[i];
        }
        m_pos += haveRead;
        return haveRead;
    }

    uint32_t write(const uint8_t *src, uint32_t bytes) {
        if (m_size == 0) {
            m_data = (uint8_t *) malloc(bytes);
        }

        if ((m_pos + bytes) > m_size) {
            uint8_t *tmp = (uint8_t *) malloc(bytes + m_size);
            for (uint32_t i = 0; i < m_size; ++i) {
                tmp[i] = m_data[i];
            }

            free(m_data);
            m_data = (uint8_t *) malloc(bytes + m_size);

            for (uint32_t i = 0; i < m_size; ++i) {
                m_data[i] = tmp[i];
            }
            free(tmp);
            m_size += (m_pos + bytes - m_size);
        }
        for (uint32_t i = m_pos, j = 0; i < (m_pos + bytes); ++i, ++j) {
            m_data[i] = src[j];
        }
        m_pos += bytes;
        return bytes;
    }

    void truncate(void) {
        if (m_pos == 0) {
            free(m_data);
            m_data = nullptr;
        } else {
            Version tmp;
            tmp.m_data = (uint8_t *) malloc(m_pos);
            for (uint32_t i = 0; i < m_pos; ++i) {
                tmp.m_data[i] = m_data[i];
            }
            free(m_data);
            m_data = (uint8_t *) malloc(m_pos);
            for (uint32_t i = 0; i < m_pos; ++i) {
                m_data[i] = tmp.m_data[i];
            }
            free(tmp.m_data);
        }
        m_size = m_pos;
    }

    uint32_t fileSize(void) const {
        return m_size;
    }

    void addVersion(void) {
        verze = (Version **) realloc(verze, (version_cnt + 1) * sizeof(Version *));
        verze[version_cnt] = (Version *) malloc(sizeof(Version));

        verze[version_cnt]->m_pos = m_pos;
        verze[version_cnt]->m_size = m_size;
        verze[version_cnt]->m_data = (uint8_t *) malloc(m_size);

        for (uint32_t i = 0; i < m_size; i++) {
            verze[version_cnt]->m_data[i] = m_data[i];
        }
        version_cnt++;
    }

    bool undoVersion(void) {
        if ((version_cnt) == 0)
            return false;

        free(m_data);
        m_data = nullptr;
        m_pos = verze[version_cnt - 1]->m_pos;
        m_size = verze[version_cnt - 1]->m_size;

        if (m_size > 0)
            m_data = (uint8_t *) malloc(m_size);

        for (uint32_t i = 0; i < m_size; i++) {
            m_data[i] = verze[version_cnt - 1]->m_data[i];
        }

        free(verze[version_cnt - 1]->m_data);
        free(verze[version_cnt - 1]);

        version_cnt--;
        return true;
    }
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
/*
int main(void) {
    CFile f0;

    assert(writeTest(f0, {10, 20, 30}, 3));
    assert(f0.fileSize() == 3);
    assert(writeTest(f0, {60, 70, 80}, 3));
    assert(f0.fileSize() == 6);
    assert(f0.seek(2));
    assert(writeTest(f0, {5, 4}, 2));
    assert(f0.fileSize() == 6);
    assert(f0.seek(1));
    assert(readTest(f0, {20, 5, 4, 70, 80}, 7));
    assert(f0.seek(3));
    f0.addVersion();
    assert(f0.seek(6));
    assert(writeTest(f0, {100, 101, 102, 103}, 4));
    f0.addVersion();
    assert(f0.seek(5));
    CFile f1(f0);
    f0.truncate();
    assert(f0.seek(0));
    assert(readTest(f0, {10, 20, 5, 4, 70}, 20));
    assert(f0.undoVersion());
    assert(f0.seek(0));
    assert(readTest(f0, {10, 20, 5, 4, 70, 80, 100, 101, 102, 103}, 20));
    assert(f0.undoVersion());
    assert(f0.seek(0));
    assert(readTest(f0, {10, 20, 5, 4, 70, 80}, 20));
    assert(!f0.seek(100));
    assert(writeTest(f1, {200, 210, 220}, 3));
    assert(f1.seek(0));
    assert(readTest(f1, {10, 20, 5, 4, 70, 200, 210, 220, 102, 103}, 20));
    assert(f1.undoVersion());
    assert(f1.undoVersion());
    assert(readTest(f1, {4, 70, 80}, 20));
    assert(!f1.undoVersion());

    return 0;
}
*/

#endif /* __PROGTEST__ */