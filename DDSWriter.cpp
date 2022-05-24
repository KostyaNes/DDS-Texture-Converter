#include "DDSWriter.h"

#include <fstream>
#include <QDebug>

void DDSWriter::WriteDDSTexture(DDSTexture& texture, const char* filePath)
{
    DDS_HEADER header = GenerateHeader(texture.m_format, texture.m_width, texture.m_height);

    std::ofstream outputImage;
    outputImage.open(filePath, std::ios::out | std::ios::binary);

    outputImage.write(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));

    for (auto byte: texture.m_data)
    {
        outputImage << byte;
    }

    outputImage.close();
}

DDS_HEADER DDSWriter::GenerateHeader(CompressionType type, int width, int height)
{
    DDS_HEADER header;
    header.dwMagic = ('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24);
    header.dwSize = 124;
    header.dwFlags = (DDSD_CAPS)|(DDSD_HEIGHT)|(DDSD_WIDTH)|(DDSD_PIXELFORMAT)|(DDSD_LINEARSIZE);
    header.dwHeight = static_cast<DWORD>(height);
    header.dwWidth = static_cast<DWORD>(width);

    switch (type)
    {
        case (dxt1):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 8);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
            break;
        }
        case (dxt3):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 16);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('3' << 24);
            break;
        }
        case (dxt5):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 16);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);
            break;
        }
    }

    header.ddspf.dwSize = 32;
    header.ddspf.dwFlags = DDPF_FOURCC;
    header.dwCaps = DDSD_CAPS;

    return header;
}
