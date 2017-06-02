
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

        // read
        virtual ReadResult readObject(const std::string & file, const Options * options = nullptr) const override;

        virtual ReadResult readObject(std::istream & stream, const Options * options = nullptr) const override;

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

REGISTER_OSGPLUGIN(asp, ReaderWriterASP);
