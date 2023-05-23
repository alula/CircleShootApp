#ifndef __DATASYNC_H__
#define __DATASYNC_H__

#include <SexyAppFramework/Common.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

namespace Sexy
{
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataReaderException : public std::exception
    {
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataReader
    {
    public:
        FILE *mFileHandle;
        const void *mMemoryHandle;
        ulong mMemoryLength;
        ulong mMemoryPosition;
        bool mDeallocate;

        explicit DataReader();
        virtual ~DataReader();

        void OpenMemory(const void *theMemory, ulong theLength, bool deallocate);
        bool OpenFile(const std::string &theFileName);
        void Close();

        void ReadBytes(void *theBuffer, ulong theLength);
        ulong ReadLong();
        ushort ReadShort();
        uchar ReadByte();
        bool ReadBool();
        float ReadFloat();
        void ReadString(std::string &theString);
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataWriter
    {
    public:
        FILE *mFileHandle;
        void *mMemoryHandle;
        ulong mMemoryPosition;
        ulong mMemoryLength;

        explicit DataWriter();
        virtual ~DataWriter();

        void OpenMemory(ulong theLength);
        void Close();
        void EnsureCapacity(ulong theLength);

        void WriteBytes(const void *theBuffer, ulong theLength);
        void WriteLong(ulong theValue);
        void WriteShort(ushort theValue);
        void WriteByte(uchar theValue);
        void WriteBool(bool theValue);
        void WriteFloat(float theValue);
        void WriteString(const std::string &theString);
    };

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    class DataSync
    {
    public:
        typedef std::map<void *, int> PointerToIntMap;
        typedef std::map<int, void *> IntToPointerMap;

        DataReader *mReader;
        DataWriter *mWriter;

        explicit DataSync(DataReader &reader);
        explicit DataSync(DataWriter &writer);

        void SyncBytes(void *theValue, ulong theSize);
        void SyncLong(int &theValue);
        void SyncLong(unsigned int &theValue);
        void SyncLong(ulong &theValue);
        void SyncShort(int &theValue);
        void SyncShort(ushort &theValue);
        void SyncSShort(int &theValue);
        void SyncByte(int &theValue);
        void SyncByte(ushort &theValue);
        void SyncBool(bool &theValue);
        void SyncFloat(float &theValue);
        void SyncString(std::string &theValue);

        void SyncPointers();
        void SyncPointer(void **thePtr);

        void RegisterPointer(void *thePtr);
        void ResetPointerTable();

    private:
        PointerToIntMap mPointerToIntMap;
        IntToPointerMap mIntToPointerMap;
        std::vector<void **> mPointerSyncList;
        int mCurPointerIndex;
    };

    template <typename TContainer>
    void DataSync_SyncSTLContainer(DataSync &theSync, TContainer &theValue)
    {
        typedef TContainer type;

        if (theSync.mReader != NULL)
        {
            theValue.clear();
            ulong aLength = theSync.mReader->ReadLong();
            for (ulong i = 0; i < aLength; i++)
            {
                typename type::value_type aValue;
                theValue.push_back(aValue);

                typename type::value_type &aRef = theValue.back();
                aRef.SyncState(theSync);
            }
        }
        else
        {
            theSync.mWriter->WriteLong(theValue.size());
            for (typename type::iterator i = theValue.begin(); i != theValue.end(); ++i)
            {
                i->SyncState(theSync);
            }
        }
    }
};

#endif