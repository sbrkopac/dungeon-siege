
#include <cstdint>

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include "siege/siege.hpp"

namespace ehb
{
    using namespace std;

    using uint = uint32_t;
    using byte = uint8_t;

    class SubMesh
    {
        enum
        {
            BSMM = 1296913218, BVTX = 1481922114,
            BCRN = 1314014018, WCRN = 1314014039,
            BVMP = 1347245634, BTRI = 1230132290,
            BVWL = 1280792130, STCH = 1212372051,
            V1_2 = 513,
            V1_3 = 769, // introduced WCRN section
            V2_0 = 2,   // introduced STCH section
            V2_1 = 258, // introduced BBOX section
            V2_2 = 514,
            V2_3 = 770,
            V2_4 = 1026,
            V2_5 = 1282
        };

        typedef struct
        {
            uint index;
            uint numFaces;
            uint startCorner;
            uint spanCorner;
        } SubTextureInfo;

        typedef struct
        {
            float pos[3];
        } VertexInfo;

        typedef struct
        {
            float pos[3];
            float nrm[3];
            float txc[2];
        } BCornerInfo;

        typedef struct
        {
            float pos[3];
            float bwt[4];
            byte  bid[4];
            float nrm[3];
            float txc[2];
        } WCornerInfo;

        typedef struct
        {
            uint corner[3];
        } FaceInfo;

        typedef struct
        {
            uint  count;
            uint * corner; // [count]
        } VCMapInfo;

        typedef struct
        {
            uint   count;
            uint  * vertex; // [count]
            float * weight; // [count]
        } BVWeightInfo;

        typedef struct
        {
            uint  token;
            uint  count;
            uint * vertex; // [count]
        } StitchInfo;

        bool readBSMM(ifstream & in);
        bool readBVTX(ifstream & in);
        bool readBCRN(ifstream & in);
        bool readWCRN(ifstream & in);
        bool readBVMP(ifstream & in);
        bool readBTRI(ifstream & in);
        bool readBVWL(ifstream & in);
        bool readSTCH(ifstream & in);
        uint versionOf(uint v);



        public:

            uint numSubTextures;
            uint numVertices;
            uint numCorners;
            uint numFaces;
            uint numStitches;
            uint numBones;
            SubTextureInfo * subTextureInfo; // [numSubTextures]
            VertexInfo     * vertexInfo;     // [numVetices]
            BCornerInfo    * bcornerInfo;    // [numCorners]
            WCornerInfo    * wcornerInfo;    // [numCorners]
            FaceInfo       * faceInfo;       // [numFaces]
            VCMapInfo      * vcMapInfo;      // [numVertices]
            BVWeightInfo   * bvWeightInfo;   // [numBones]
            StitchInfo     * stitchInfo;     // [numStitches]
            SubMesh();
            ~SubMesh();
            bool readBSUB(ifstream & in, uint nB);
    };

    class Aspect
    {
        enum
        {
            BMSH = 1213418818, BONH = 1213091650,
            BSUB = 1112888130, RPOS = 1397706834,
            BBOX = 1481589314, BEND = 1145980226,
            INFO = 1330007625,
        };

        typedef struct
        {
            char name[64];
            uint parent;
            uint flags;
        } BoneInfo;

        typedef struct
        {
            char name[64];
        } TextureInfo;

        typedef struct
        {
            float rot[4];
            float pos[3];
        } RPosInfo;

        typedef struct
        {
            char key[32];
            char value[256];
        } InfoInfo;

        bool readBMSH(ifstream & in);
        bool readBONH(ifstream & in);
        bool readSubMesh(ifstream & in);
        bool readRPOS(ifstream & in);
        bool readBBOX(ifstream & in);
        bool readBEND(ifstream & in);

        public:

            uint numSubMeshes;
            uint numBones;
            uint numTextures;
            uint numVerticesTotal;
            uint numInfos;
            SubMesh     * subMesh;
            BoneInfo    * boneInfo;
            TextureInfo * textureInfo;
            RPosInfo    * rposInfoAbI; // [numBones]
            RPosInfo    * rposInfoRel; // [numBones]
            InfoInfo    * infoInfo; // [numInfo]

            Aspect();
            ~Aspect();
            bool readFile(const char * filename);
            const uint getNumBones() const { return numBones; }
    };
}

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

namespace ehb
{
    Aspect::Aspect()
    {
        numSubMeshes = 0;
        numBones = 0;
        numTextures = 0;
        numVerticesTotal = 0;
        numInfos = 0;
        subMesh = NULL;
        boneInfo = NULL;
        textureInfo = NULL;
        rposInfoAbI = NULL;
        rposInfoRel = NULL;
        infoInfo = NULL;
    }

    Aspect::~Aspect()
    {
        if (subMesh)     delete[] subMesh;
        if (boneInfo)    delete[] boneInfo;
        if (textureInfo) delete[] textureInfo;
        if (rposInfoAbI) delete[] rposInfoAbI;
        if (rposInfoRel) delete[] rposInfoRel;
        if (infoInfo)    delete[] infoInfo;
    }

    bool Aspect::readBMSH(ifstream &in)
    {
        uint version, sizeText, dummy, i;
        char *text, *ptr;

        in.read((char *)&version, 4);
        in.read((char *)&sizeText, 4);
        in.read((char *)&numBones, 4);
        in.read((char *)&numTextures, 4);
        in.read((char *)&numVerticesTotal, 4);
        in.read((char *)&numSubMeshes, 4);
        in.read((char *)&dummy, 4); // 0
        text = new char[sizeText];
        in.read(text, sizeText);
        ptr = text;

        textureInfo = new TextureInfo[numTextures];
        for (i = 0; i < numTextures; i++)
        {
            while (!*ptr) ptr++;
            assert(strlen(ptr) < 64);
            strcpy(textureInfo[i].name, ptr);
            ptr += strlen(textureInfo[i].name);
        }

        boneInfo = new BoneInfo[numBones];
        for (i = 0; i < numBones; i++)
        {
            while (!*ptr) ptr++;
            assert(strlen(ptr) < 64);
            strcpy(boneInfo[i].name, ptr);
            ptr += strlen(boneInfo[i].name);
        }
        delete[] text;

        subMesh = new SubMesh[numSubMeshes];

        return true;
    }

    bool Aspect::readBONH(ifstream &in)
    {
        uint version, bone, parent, flags;

        in.read((char *)&version, 4);
        for (uint i = 0; i < numBones; i++)
        {
            in.read((char *)&bone, 4);
            in.read((char *)&parent, 4);
            in.read((char *)&flags, 4); // ???
            boneInfo[bone].parent = parent;
            boneInfo[bone].flags = flags;
        }

        return true;
    }

    bool Aspect::readSubMesh(ifstream &in)
    {
        uint version;
        int subMeshID;
        bool r;

        in.read((char *)&version, 4);
        in.read((char *)&subMeshID, 4);
        // subMeshID: -1 -> first, 0 -> second, 1 -> third
        r = subMesh[subMeshID + 1].readBSUB(in, numBones);

        return r;
    }

    bool Aspect::readRPOS(ifstream &in)
    {
        uint version, dummy;

        in.read((char *)&version, 4);
        in.read((char *)&dummy, 4);
        assert(dummy == numBones);
        rposInfoAbI = new RPosInfo[numBones];
        rposInfoRel = new RPosInfo[numBones];
        for (uint i = 0; i < numBones; i++)
        {
            in.read((char *)&rposInfoAbI[i], sizeof(RPosInfo));
            in.read((char *)&rposInfoRel[i], sizeof(RPosInfo));
        }

        return true;
    }

    //---------------------------------------------------------------------------

    bool Aspect::readBBOX(ifstream &in)
    {
        uint version, dummy;

        in.read((char *)&version, 4);
        in.read((char *)&dummy, 4);
        assert(dummy == 0);

        return true;
    }

    bool Aspect::readBEND(ifstream &in)
    {
        uint mark;
        char *ptr;

        in.read((char *)&mark, 4);
        assert(mark == INFO);
        in.read((char *)&numInfos, 4);
        infoInfo = new InfoInfo[numInfos];
        for (uint i = 0; i < numInfos; i++)
        {
            // read key
            ptr = infoInfo[i].key;
            while (true)
            {
                in.read(ptr, 1);
                if (*ptr == '=')
                    break;
                ptr++;
            }
            *ptr = '\0';
            // read value
            ptr = infoInfo[i].value;
            while (true)
            {
                in.read(ptr, 1);
                if (*ptr == '\0')
                    break;
                ptr++;
            }
        }

        return true;
    }

    SubMesh::SubMesh()
    {
        numSubTextures = 0;
        numVertices = 0;
        numCorners = 0;
        numFaces = 0;
        numStitches = 0;
        numBones = 0;
        subTextureInfo = NULL;
        vertexInfo = NULL;
        bcornerInfo = NULL;
        wcornerInfo = NULL;
        faceInfo = NULL;
        vcMapInfo = NULL;
        bvWeightInfo = NULL;
        stitchInfo = NULL;
    }

    SubMesh::~SubMesh()
    {
        if (subTextureInfo) delete[] subTextureInfo;
        if (vertexInfo)     delete[] vertexInfo;
        if (bcornerInfo)    delete[] bcornerInfo;
        if (wcornerInfo)    delete[] wcornerInfo;
        if (faceInfo)       delete[] faceInfo;
        if (vcMapInfo)
        {
            for (uint vc = 0; vc < numVertices; vc++)
                if (vcMapInfo[vc].corner)
                    delete[] vcMapInfo[vc].corner;
            delete[] vcMapInfo;
        }
        if (bvWeightInfo)
        {
            for (uint bv = 0; bv < numBones; bv++)
            {
                if (bvWeightInfo[bv].vertex)
                    delete[] bvWeightInfo[bv].vertex;
                if (bvWeightInfo[bv].weight)
                    delete[] bvWeightInfo[bv].weight;
            }
            delete[] bvWeightInfo;
        }
        if (stitchInfo)
        {
            for (uint st = 0; st < numStitches; st++)
                if (stitchInfo[st].vertex)
                    delete[] stitchInfo[st].vertex;
            delete[] stitchInfo;
        }
    }

    bool SubMesh::readBSUB(ifstream &in, uint nB)
    {
        uint mark;
        bool r, done;

        numBones = nB;

        in.read((char *)&numSubTextures, 4);
        in.read((char *)&numVertices, 4);
        in.read((char *)&numCorners, 4);
        in.read((char *)&numFaces, 4);

        done = false;
        while (!done)
        {
            in.read((char *)&mark, 4);
            switch (mark)
            {
                case BSMM: r = readBSMM(in); break;
                case BVTX: r = readBVTX(in); break;
                case BCRN: r = readBCRN(in); break;
                case WCRN: r = readWCRN(in); break;
                case BVMP: r = readBVMP(in); break;
                case BTRI: r = readBTRI(in); break;
                case BVWL: r = readBVWL(in); break;
                case STCH: r = readSTCH(in); done = true; break;
                default: r = false;
            }
            if (!r)
                return false;
        }

        return true;
    }

    bool SubMesh::readBSMM(ifstream &in)
    {
        uint version;

        in.read((char *)&version, 4);
        in.read((char *)&numSubTextures, 4);
        subTextureInfo = new SubTextureInfo[numSubTextures];
        for (uint i = 0; i < numSubTextures; i++)
        {
            in.read((char *)&subTextureInfo[i].index, 4);
            in.read((char *)&subTextureInfo[i].numFaces, 4);
        }

        return true;
    }

    bool SubMesh::readBVTX(ifstream &in)
    {
        uint version, dummy;

        in.read((char *)&version, 4);
        in.read((char *)&dummy, 4);
        assert(dummy == numVertices);
        vertexInfo = new VertexInfo[numVertices];
        for (uint i = 0; i < numVertices; i++)
        {
            in.read((char *)&vertexInfo[i].pos[0], 4);
            in.read((char *)&vertexInfo[i].pos[1], 4);
            in.read((char *)&vertexInfo[i].pos[2], 4);
        }

        return true;
    }

    bool SubMesh::readBCRN(ifstream &in)
    {
        uint version, dummy, index;

        in.read((char *)&version, 4);
        in.read((char *)&dummy, 4);
        assert(dummy == numCorners);
        bcornerInfo = new BCornerInfo[numCorners];
        for (uint i = 0; i < numCorners; i++)
        {
            in.read((char *)&index, 4);
            bcornerInfo[i].pos[0] = vertexInfo[index].pos[0];
            bcornerInfo[i].pos[1] = vertexInfo[index].pos[1];
            bcornerInfo[i].pos[2] = vertexInfo[index].pos[2];
            in.read((char *)&bcornerInfo[i].nrm[0], 4);
            in.read((char *)&bcornerInfo[i].nrm[1], 4);
            in.read((char *)&bcornerInfo[i].nrm[2], 4);
            in.read((char *)&dummy, 4); // float
            in.read((char *)&dummy, 4); // int
            in.read((char *)&bcornerInfo[i].txc[0], 4);
            in.read((char *)&bcornerInfo[i].txc[1], 4);
        }

        return true;
    }

    bool SubMesh::readWCRN(ifstream &in)
    {
        uint version, dummy;

        in.read((char *)&version, 4);
        in.read((char *)&dummy, 4);
        assert(dummy == numCorners);
        wcornerInfo = new WCornerInfo[numCorners];
        for (uint i = 0; i < numCorners; i++)
        {
            in.read((char *)&wcornerInfo[i].pos[0], 4);
            in.read((char *)&wcornerInfo[i].pos[1], 4);
            in.read((char *)&wcornerInfo[i].pos[2], 4);
            in.read((char *)&wcornerInfo[i].bwt[0], 4);
            in.read((char *)&wcornerInfo[i].bwt[1], 4);
            in.read((char *)&wcornerInfo[i].bwt[2], 4);
            in.read((char *)&wcornerInfo[i].bwt[3], 4);
            in.read((char *)&wcornerInfo[i].bid[0], 1);
            in.read((char *)&wcornerInfo[i].bid[1], 1);
            in.read((char *)&wcornerInfo[i].bid[2], 1);
            in.read((char *)&wcornerInfo[i].bid[3], 1);
            in.read((char *)&wcornerInfo[i].nrm[0], 4);
            in.read((char *)&wcornerInfo[i].nrm[1], 4);
            in.read((char *)&wcornerInfo[i].nrm[2], 4);
            in.read((char *)&dummy, 4); // float
            in.read((char *)&wcornerInfo[i].txc[0], 4);
            in.read((char *)&wcornerInfo[i].txc[1], 4);
            wcornerInfo[i].txc[1] = 1 - wcornerInfo[i].txc[1];
        }

        return true;
    }

    bool SubMesh::readBVMP(ifstream &in)
    {
        uint version;

        in.read((char *)&version, 4);

        vcMapInfo = new VCMapInfo[numVertices];
        memset(vcMapInfo, 0, sizeof(VCMapInfo) * numVertices);
        for (uint i = 0; i < numVertices; i++)
        {
            in.read((char *)&vcMapInfo[i].count, 4);
            if (vcMapInfo[i].count)
            {
                vcMapInfo[i].corner = new uint[vcMapInfo[i].count];
                for (uint j = 0; j < vcMapInfo[i].count; j++)
                    in.read((char *)&vcMapInfo[i].corner[j], 4);
            }
        }

        return true;
    }

    bool SubMesh::readBTRI(ifstream &in)
    {
        uint version, dummy, i;

        in.read((char *)&version, 4);

        in.read((char *)&dummy, 4);
        assert(dummy == numFaces);
        if (versionOf(version) == 22)
        {
            for (i = 0; i < numSubTextures; i++)
                in.read((char *)&subTextureInfo[i].spanCorner, 4);
            subTextureInfo[0].startCorner = 0;
            for (i = 0; i < numSubTextures - 1; i++)
                subTextureInfo[i + 1].startCorner = subTextureInfo[i].startCorner + subTextureInfo[i].spanCorner;
        }
        else if (versionOf(version) > 22)
        {
            for (i = 0; i < numSubTextures; i++)
            {
                in.read((char *)&subTextureInfo[i].startCorner, 4);
                in.read((char *)&subTextureInfo[i].spanCorner, 4);
            }
        }
        faceInfo = new FaceInfo[numFaces];
        for (i = 0; i < numFaces; i++)
        {
            in.read((char *)&faceInfo[i].corner[0], 4);
            in.read((char *)&faceInfo[i].corner[1], 4);
            in.read((char *)&faceInfo[i].corner[2], 4);
        }

        return true;
    }

    bool SubMesh::readBVWL(ifstream &in)
    {
        uint version;

        in.read((char *)&version, 4);
        bvWeightInfo = new BVWeightInfo[numBones];
        memset(bvWeightInfo, 0, sizeof(BVWeightInfo) * numBones);
        for (uint i = 0; i < numBones; i++)
        {
            in.read((char *)&bvWeightInfo[i].count, 4);
            if (bvWeightInfo[i].count == 0xffffffff)
            {
                bvWeightInfo[i].count = numVertices;
                bvWeightInfo[i].vertex = new uint[bvWeightInfo[i].count];
                bvWeightInfo[i].weight = new float[bvWeightInfo[i].count];
                for (uint j = 0; j < bvWeightInfo[i].count; j++)
                {
                    bvWeightInfo[i].vertex[j] = j;
                    bvWeightInfo[i].weight[j] = 1.0f;
                }
            }
            else if (bvWeightInfo[i].count)
            {
                bvWeightInfo[i].vertex = new uint[bvWeightInfo[i].count];
                bvWeightInfo[i].weight = new float[bvWeightInfo[i].count];
                for (uint j = 0; j < bvWeightInfo[i].count; j++)
                {
                    in.read((char *)&bvWeightInfo[i].vertex[j], 4);
                    in.read((char *)&bvWeightInfo[i].weight[j], 4);
                }
            }
        }

        return true;
    }

    bool SubMesh::readSTCH(ifstream &in)
    {
        uint version;

        in.read((char *)&version, 4);
        in.read((char *)&numStitches, 4);
        stitchInfo = new StitchInfo[numStitches];
        memset(stitchInfo, 0, sizeof(StitchInfo) * numStitches);
        for (uint i = 0; i < numStitches; i++)
        {
            in.read((char *)&stitchInfo[i].token, 4);
            in.read((char *)&stitchInfo[i].count, 4);
            if (stitchInfo[i].count)
            {
                stitchInfo[i].vertex = new uint[stitchInfo[i].count];
                for (uint j = 0; j < stitchInfo[i].count; j++)
                    in.read((char *)&stitchInfo[i].vertex[j], 4);
            }
        }

        return true;
    }

    uint SubMesh::versionOf(uint v)
    {
        switch (v)
        {
            case V1_2: return 12;
            case V1_3: return 13;
            case V2_0: return 20;
            case V2_1: return 21;
            case V2_2: return 22;
            case V2_3: return 23;
            case V2_4: return 24;
            case V2_5: return 25;
        }

        return 0;
    }
}
