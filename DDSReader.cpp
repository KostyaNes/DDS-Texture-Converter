#include "DDSReader.h"

#include <fstream>
#include <limits>

#include <QDebug>

DDSTexture DDSReader::ReadDDSTexture(const char *filePath)
{
    DDSTexture texture;

    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    char* buffer = new char[sizeof(DDS_HEADER)];

    file.read(buffer, sizeof(DDS_HEADER));

    DDS_HEADER* header = reinterpret_cast<DDS_HEADER*>(buffer);
    const DWORD dxt1 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
    const DWORD dxt3 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('3' << 24);
    const DWORD dxt5 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);

    texture.m_width = header->dwWidth;
    texture.m_height = header->dwHeight;

    int dataSizeMultiplier = 0;

    switch (header->ddspf.dwFourCC)
    {
        case (dxt1):
        {
            texture.m_format = CompressionType::dxt1;
            dataSizeMultiplier = 8;
            break;
        }
        case(dxt3):
        {
            texture.m_format = CompressionType::dxt3;
            dataSizeMultiplier = 16;
            break;
        }
        case(dxt5):
        {
            texture.m_format = CompressionType::dxt5;
            dataSizeMultiplier = 16;
            break;
        }
    }

    unsigned long long dataSize = (texture.m_width + (4 - texture.m_width % 4) % 4) * (texture.m_height + (4 - texture.m_height % 4) % 4) / 16 * dataSizeMultiplier;

    if (dataSize > 0)
    {
        delete[] buffer;
        buffer = new char[dataSize];
        file.read(buffer, dataSize);

        for (unsigned int i = 0; i < dataSize; i++)
        {
            texture.m_data.push_back(buffer[i]);
        }
    }

    file.close();

    delete[] buffer;

    return texture;
}
