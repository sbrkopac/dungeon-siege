
#include <algorithm>
#include <new>
#include <cstdint>

#include <osg/Endian>
#include <osg/GL>
#include <osg/Image>
#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

class ReaderWriterRAW : public osgDB::ReaderWriter
{
    // using osgDB::ReaderWriter;

public:

    static const uint32_t RAW_MAGIC;
    static const uint32_t RAW_FORMAT_8888;

public:

    ReaderWriterRAW();

    virtual const char * className() const override;

    // read
    virtual ReadResult readObject(const std::string & file, const Options * options = nullptr) const override;

    virtual ReadResult readObject(std::istream & stream, const Options * options = nullptr) const override;

    virtual ReadResult readImage(const std::string & file, const Options * options) const override;

    virtual ReadResult readImage(std::istream & stream, const Options * options = nullptr) const override;

    // write
    #if 0
    virtual WriteResult writeObject(const osg::Object & object, const std::string & file, const Options * options = nullptr) const override;

    virtual WriteResult writeObject(const osg::Object & object, std::ostream & stream, const Options * options = nullptr) const override;
    #endif

    virtual WriteResult writeImage(const osg::Image & image, const std::string & file, const Options * options = nullptr) const override;

    virtual WriteResult writeImage(const osg::Image & image, std::ostream & stream, const Options * options = nullptr) const override;
};

const uint32_t ReaderWriterRAW::RAW_MAGIC = 0x52617069;
const uint32_t ReaderWriterRAW::RAW_FORMAT_8888 = 0x38383838;

ReaderWriterRAW::ReaderWriterRAW()
{
    supportsExtension("raw", "Dungeon Siege Raw image format");
}

const char * ReaderWriterRAW::className() const
{
    return "Dungeon Siege RAW Reader/Writer.";
}

osgDB::ReaderWriter::ReadResult ReaderWriterRAW::readObject(const std::string & file, const osgDB::ReaderWriter::Options * options) const
{
    return readImage(file, options);
}

osgDB::ReaderWriter::ReadResult ReaderWriterRAW::readObject(std::istream & stream, const osgDB::ReaderWriter::Options * options) const
{
    return readImage(stream, options);
}

osgDB::ReaderWriter::ReadResult ReaderWriterRAW::readImage(const std::string & file, const osgDB::ReaderWriter::Options * options) const
{
    const std::string ext = osgDB::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

    const std::string fileName = osgDB::findDataFile(file, options);
    if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

    osgDB::ifstream stream(fileName.c_str(), std::ios::in | std::ios::binary);
    if (!stream) return ReadResult::FILE_NOT_HANDLED;

    ReadResult rr = readImage(stream, options);
    if (rr.validImage()) rr.getImage()->setFileName(file);

    return rr;
}

osgDB::ReaderWriter::ReadResult ReaderWriterRAW::readImage(std::istream & stream, const osgDB::ReaderWriter::Options * options) const
{
    uint32_t magic, format;
    uint16_t flags, surfaceCount, width, height;

    stream.read((char *) &magic, sizeof(uint32_t));
    stream.read((char *) &format, sizeof(uint32_t));
    stream.read((char *) &flags, sizeof(uint16_t));
    stream.read((char *) &surfaceCount, sizeof(uint16_t));
    stream.read((char *) &width, sizeof(uint16_t));
    stream.read((char *) &height, sizeof(uint16_t));

    const bool bSwap = (osg::getCpuByteOrder() == osg::BigEndian);

    if (bSwap)
    {
        osg::swapBytes4((char *) &magic);
        osg::swapBytes4((char *) &format);
        osg::swapBytes2((char *) &flags);
        osg::swapBytes2((char *) &surfaceCount);
        osg::swapBytes2((char *) &width);
        osg::swapBytes2((char *) &height);
    }

    if (magic != RAW_MAGIC)
    {
        return ReadResult::FILE_NOT_HANDLED;
    }

    // TODO: handle the different possible formats available here
    if (format != RAW_FORMAT_8888)
    {
        return ReadResult::FILE_NOT_HANDLED;
    }

    // with the 8888 format each pixel is 4 bytes
    const uint32_t size = 4 * static_cast<uint32_t>(width) * static_cast<uint32_t>(height);

    uint32_t * data = new (std::nothrow) uint32_t[size];

    if (data != nullptr)
    {
        stream.read(static_cast<char *>(static_cast<void *>(data)), sizeof(uint32_t) * size);

        /*
         * 6.5.5 Multiplicative operators
         *
         * When integers are divided, the result of the / operator is the algebraic quotient with any fractional part discarded. If the quotient a/b is representable, the expression (a/b)*b + a%b shall equal a.
         */
        const uint16_t halfHeight = height / 2;

        // account for vertical flip in RAW images
        for (uint16_t j = 0; j < halfHeight; j++)
        {
            // two methods to do the exact same thing, take your pick
#if 0
            for (uint16_t i = 0; i < width; i++)
            {
                std::swap(data[i + (j * width)], data[i + (height - (j + 1)) * width]);
            }
#else
            const ptrdiff_t first1 = j * width;
            const ptrdiff_t first2 = (height - (j - 1)) * width;
            const ptrdiff_t length = width - 1;

            std::swap_ranges(data + first1, data + first1 + length, data + first2);
#endif
        }

        osg::ref_ptr<osg::Image> image = new osg::Image;

        image->setImage(width, height, 1,
                        GL_RGBA,
                        GL_BGRA_EXT,
                        GL_UNSIGNED_BYTE,
                        static_cast<unsigned char *>(static_cast<void *>(data)),
                        osg::Image::USE_NEW_DELETE);

        return image.release();
    }

    return ReadResult::INSUFFICIENT_MEMORY_TO_LOAD;
}

#if 0
osgDB::ReaderWriter::WriteResult ReaderWriterRAW::writeObject(const osg::Object & object, const std::string & file, const osgDB::ReaderWriter::Options * options) const
{
}

osgDB::ReaderWriter::WriteResult ReaderWriterRAW::writeObject(const osg::Object & object, std::ostream & stream, const osgDB::ReaderWriter::Options * options) const
{
}
#endif

osgDB::ReaderWriter::WriteResult ReaderWriterRAW::writeImage(const osg::Image & image, const std::string & file, const osgDB::ReaderWriter::Options * options) const
{
    const std::string ext = osgDB::getFileExtension(file);
    if ( !osgDB::equalCaseInsensitive(ext, "raw") ) return WriteResult::FILE_NOT_HANDLED;

    // only support rgba images right now.
    if (image.getPixelFormat() != GL_RGBA || image.getDataType() != GL_UNSIGNED_BYTE) return WriteResult("Error image pixel format not supported by raw writer.");

    osgDB::ofstream stream(file.c_str(), std::ios::out | std::ios::binary);
    if(!stream) return WriteResult::ERROR_IN_WRITING_FILE;

    return writeImage(image, stream, options);
}

osgDB::ReaderWriter::WriteResult ReaderWriterRAW::writeImage(const osg::Image & image, std::ostream & stream, const osgDB::ReaderWriter::Options * options) const
{
    // TODO: write mip maps?
    uint16_t flags = 0;
    uint16_t surfaceCount = 1;
    uint16_t width = image.s();
    uint16_t height = image.t();

    stream.write((char *) &RAW_MAGIC, sizeof(uint32_t));
    stream.write((char *) &RAW_FORMAT_8888, sizeof(uint32_t));
    stream.write((char *) &flags, sizeof(uint16_t));
    stream.write((char *) &surfaceCount, sizeof(uint16_t));
    stream.write((char *) &width, sizeof(uint16_t));
    stream.write((char *) &height, sizeof(uint16_t));

    for (int j = image.t() - 1; j >= 0; --j)
    {
        const unsigned char * data = image.data(0, j);

        for (int i = 0; i < image.s(); ++i)
        {
            stream.put(*(data++));
            stream.put(*(data++));
            stream.put(*(data++));
            stream.put(*(data++));
        }
    }

    return WriteResult::FILE_SAVED;
}

REGISTER_OSGPLUGIN(raw, ReaderWriterRAW);
