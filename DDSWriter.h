#ifndef DDSWRITER_H
#define DDSWRITER_H
#define NOMINMAX

#include <DDSDefs.h>
#include <ImageDefs.h>

#include <sstream>

class DDSWriter
{
public:
    DDSWriter() = default;

    void WriteDDSTexture(DDSTexture& texture, const char* filePath);
    DDS_HEADER GenerateHeader(CompressionType type, int width, int height);
};

#endif // DDSWRITER_H
