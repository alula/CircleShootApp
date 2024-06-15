#include "Zuma_Prefix.pch"

#include <SexyAppFramework/Debug.h>

#include "DataSync.h"

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DataReader::DataReader() : mFileHandle(NULL), mMemoryHandle(NULL), mMemoryLength(0), mMemoryPosition(0), mDeallocate(false)
{
}

DataReader::~DataReader()
{
    Close();
}

void DataReader::OpenMemory(const void *theMemory, ulong theLength, bool deallocate)
{
    Close();

    mMemoryHandle = theMemory;
    mMemoryLength = theLength;
    mDeallocate = deallocate;
}

bool DataReader::OpenFile(const std::string &theFileName)
{
    Close();

    mFileHandle = fopen(theFileName.c_str(), "rb");
    return mFileHandle != NULL;
}

void DataReader::Close()
{
    if (mFileHandle)
    {
        fclose(mFileHandle);
        mFileHandle = NULL;
    }

    if (mDeallocate && mMemoryHandle)
    {
        delete[] mMemoryHandle;
    }

    mMemoryHandle = NULL;
    mMemoryLength = 0;
    mMemoryPosition = 0;
    mDeallocate = false;
}

void DataReader::ReadBytes(void *theBuffer, ulong theLength)
{
    if (mMemoryHandle)
    {
        mMemoryPosition += theLength;
        if (mMemoryPosition <= mMemoryLength)
        {
            memcpy(theBuffer, mMemoryHandle, theLength);
            mMemoryHandle = (char *)mMemoryHandle + theLength;
            return;
        }
    }

    if (mFileHandle && theLength == fread(theBuffer, 1, theLength, mFileHandle))
        return;

    throw DataReaderException();
}

ulong DataReader::ReadLong()
{
    ulong result;
    ReadBytes(&result, sizeof(result));
    return LONG_LITTLEE_TO_NATIVE(result);
}

ushort DataReader::ReadShort()
{
    ushort result = 0;
    ReadBytes(&result, sizeof(result));
    return WORD_LITTLEE_TO_NATIVE(result);
}

uchar DataReader::ReadByte()
{
    uchar result = 0;
    ReadBytes(&result, sizeof(result));
    return result;
}

bool DataReader::ReadBool()
{
    return ReadByte() != 0;
}

float DataReader::ReadFloat()
{
    ulong result;
    ReadBytes(&result, sizeof(result));
    result = LONG_LITTLEE_TO_NATIVE(result);
    return reinterpret_cast<float &>(result);
}

void DataReader::ReadString(std::string &theString)
{
    ulong length = ReadShort();
    theString.resize(length, 0);
    ReadBytes(&theString[0], length);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DataWriter::DataWriter() : mFileHandle(NULL), mMemoryHandle(NULL), mMemoryLength(0), mMemoryPosition(0) {}

DataWriter::~DataWriter()
{
    Close();
}

void DataWriter::OpenMemory(ulong theLength) {
    Close();

    if (theLength < 32)
        theLength = 32;

    mMemoryLength = theLength;
    mMemoryHandle = new char[theLength];
}

void DataWriter::Close()
{
    if (mFileHandle)
    {
        fclose(mFileHandle);
        mFileHandle = NULL;
    }

    if (mMemoryHandle)
    {
        delete[] mMemoryHandle;
    }

    mMemoryHandle = NULL;
    mMemoryLength = 0;
    mMemoryPosition = 0;
}

void DataWriter::EnsureCapacity(ulong theLength)
{
    if (mMemoryLength < theLength)
    {
        ulong newLength = mMemoryLength;
        do
        {
            newLength *= 2;
        } while (newLength < theLength);
        mMemoryLength = newLength;

        void *newMemory = new char[newLength];
        memcpy(newMemory, mMemoryHandle, mMemoryPosition);
        delete[] mMemoryHandle;

        mMemoryHandle = newMemory;
    }
}

void DataWriter::WriteBytes(const void *theBuffer, ulong theLength)
{
    if (mMemoryHandle)
    {
        EnsureCapacity(mMemoryPosition + theLength);
        memcpy((char*)mMemoryHandle + mMemoryPosition, theBuffer, theLength);
        mMemoryPosition += theLength;
    }
    else if (mFileHandle)
    {
        fwrite(theBuffer, 1, theLength, mFileHandle);
    }
}

void DataWriter::WriteLong(ulong theValue)
{
    theValue = LONG_NATIVE_TO_LITTLEE(theValue);
    WriteBytes(&theValue, sizeof(theValue));
}

void DataWriter::WriteShort(ushort theValue)
{
    theValue = WORD_NATIVE_TO_LITTLEE(theValue);
    WriteBytes(&theValue, sizeof(theValue));
}

void DataWriter::WriteByte(uchar theValue)
{
    WriteBytes(&theValue, sizeof(theValue));
}

void DataWriter::WriteBool(bool theValue)
{
    WriteBytes(&theValue, sizeof(theValue));
}

void DataWriter::WriteFloat(float theValue)
{
    ulong result = reinterpret_cast<ulong &>(theValue);
    result = LONG_NATIVE_TO_LITTLEE(result);
    WriteBytes(&result, sizeof(result));
}

void DataWriter::WriteString(const std::string &theString)
{
    WriteShort(theString.length());
    WriteBytes(theString.c_str(), theString.length());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DataSync::DataSync(DataReader &reader)
{
    mReader = NULL;
    mWriter = NULL;
    ResetPointerTable();
    mReader = &reader;
}

DataSync::DataSync(DataWriter &writer)
{
    mReader = NULL;
    mWriter = NULL;
    ResetPointerTable();
    mWriter = &writer;
}

void DataSync::SyncBytes(void *theValue, ulong theSize)
{
    if (mReader)
    {
        mReader->ReadBytes(theValue, theSize);
    }
    else if (mWriter)
    {
        mWriter->WriteBytes(theValue, theSize);
    }
}

void DataSync::SyncLong(int &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadLong();
    }
    else
    {
        mWriter->WriteLong(theValue);
    }
}

void DataSync::SyncLong(unsigned int &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadLong();
    }
    else
    {
        mWriter->WriteLong(theValue);
    }
}

void DataSync::SyncLong(unsigned long &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadLong();
    }
    else
    {
        mWriter->WriteLong(theValue);
    }
}

void DataSync::SyncShort(int &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadShort();
    }
    else
    {
        mWriter->WriteShort(theValue);
    }
}

void DataSync::SyncShort(unsigned short &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadShort();
    }
    else
    {
        mWriter->WriteShort(theValue);
    }
}

void DataSync::SyncSShort(int &theValue)
{
    if (mReader)
    {
        theValue = static_cast<short>(mReader->ReadShort());
    }
    else
    {
        mWriter->WriteShort(static_cast<ushort>(theValue));
    }
}

void DataSync::SyncByte(int &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadByte();
    }
    else
    {
        mWriter->WriteByte(static_cast<uchar>(theValue));
    }
}

void DataSync::SyncByte(unsigned short &theValue)
{
    if (mReader)
    {
        theValue = static_cast<uchar>(mReader->ReadByte());
    }
    else
    {
        mWriter->WriteByte(static_cast<uchar>(theValue));
    }
}

void DataSync::SyncBool(bool &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadBool();
    }
    else
    {
        mWriter->WriteBool(theValue);
    }
}

void DataSync::SyncFloat(float &theValue)
{
    if (mReader)
    {
        theValue = mReader->ReadFloat();
    }
    else
    {
        mWriter->WriteFloat(theValue);
    }
}

void DataSync::SyncString(std::string &theValue)
{
    if (mReader)
    {
        mReader->ReadString(theValue);
    }
    else
    {
        mWriter->WriteString(theValue);
    }
}

void DataSync::SyncPointers()
{
    if (mReader)
    {
        for (std::vector<void **>::iterator i = mPointerSyncList.begin(); i != mPointerSyncList.end(); ++i)
        {
            int a = (int)mReader->ReadLong();
            IntToPointerMap::iterator aFindItr = mIntToPointerMap.find(a);

            if (aFindItr == mIntToPointerMap.end())
            {
                throw DataReaderException();
            }
            **i = aFindItr->second;
        }
    }
    else
    {
        for (std::vector<void **>::iterator i = mPointerSyncList.begin(); i != mPointerSyncList.end(); ++i)
        {
            PointerToIntMap::iterator aFindItr = mPointerToIntMap.find(**i);
            assert(aFindItr != mPointerToIntMap.end());
            mWriter->WriteLong(aFindItr->second);
        }
    }

    ResetPointerTable();
}

void DataSync::SyncPointer(void **thePointer)
{
    mPointerSyncList.push_back(thePointer);
}

void DataSync::RegisterPointer(void *thePtr)
{
    DBG_ASSERT(thePtr != NULL);

    std::pair<PointerToIntMap::iterator, bool> aPTIEntry = mPointerToIntMap.insert(std::make_pair(thePtr, 0));
    if (aPTIEntry.second)
    {
        int num = mCurPointerIndex++;
        aPTIEntry.first->second = num;
        mIntToPointerMap[num] = thePtr;
    }
}

void DataSync::ResetPointerTable()
{
    mIntToPointerMap.clear();
    mPointerToIntMap.clear();
    mPointerSyncList.clear();
    mCurPointerIndex = 1;
    mPointerToIntMap[NULL] = 0;
    mIntToPointerMap[0] = NULL;
}