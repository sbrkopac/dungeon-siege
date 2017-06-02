
#include <cstdint>

#include <osg/Endian>
#include <osg/GL>
#include <osg/Image>
#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

class ReaderWriterASP : public osgDB::ReaderWriter
{
    public:

        ReaderWriterASP();

        virtual const char * className() const override;

        virtual ReadResult readNode(std::istream & stream, const Options * options = nullptr) const override;

};

ReaderWriterASP::ReaderWriterASP()
{
    supportsExtension("asp", "Dungeon Siege Aspect mesh format");
}

const char * ReaderWriterASP::className() const
{
    return "Dungeon Siege ASP Reader/Writer";
}

osgDB::ReaderWriter::ReadResult ReaderWriterASP::readNode(std::istream & stream, const Options * options) const
{
    return osgDB::ReaderWriter::ReadResult::FILE_NOT_HANDLED;
}

REGISTER_OSGPLUGIN(asp, ReaderWriterASP);
