#ifndef DDSREADER_H
#define DDSREADER_H

#include <ImageDefs.h>

class DDSReader
{
public:
    DDSReader() = default;

    DDSTexture ReadDDSTexture(const char* filePath);
};

#endif // DDSREADER_H
