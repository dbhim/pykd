#pragma once 

#include "boost/python/object.hpp"
#include "boost/python/wrapper.hpp"

#include "kdlib/dataaccessor.h"
#include "kdlib/exceptions.h"

#include "pythreadstate.h"

namespace python = boost::python;

namespace pykd {

///////////////////////////////////////////////////////////////////////////////

using kdlib::ImplementException;

///////////////////////////////////////////////////////////////////////////////

class PythonObjectAccessor  : public kdlib::DataAccessor
{

public:

    PythonObjectAccessor(python::object& obj, size_t pos = 0) : m_object(obj) 
    {
        m_pystate = PyThreadState_Get();
        m_startPos = pos;
    }

public:

    size_t getLength() const final {
        AutoSavePythonState  pystate(&m_pystate);
        return python::len(m_object);
    }

    unsigned char readByte(size_t pos = 0) const final  {
        return readValue<unsigned char>(pos);
    }

    void writeByte(unsigned char value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    char readSignByte(size_t pos=0) const final {
        return readValue<char>(pos);
    }

    void writeSignByte(char value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    unsigned short readWord(size_t pos = 0) const final  {
        return readValue<unsigned short>(pos);
    }

    void writeWord(unsigned short value, size_t pos = 0) final  {
        writeValue(value,pos);
    }

    short readSignWord(size_t pos = 0) const final {
        return readValue<short>(pos);
    }

    void writeSignWord(short value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    unsigned long readDWord(size_t pos = 0) const final {
        return readValue<unsigned long>(pos);
    }

    void writeDWord(unsigned long value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    long readSignDWord(size_t pos = 0) const final {
        return readValue<long>(pos);
    }

    void writeSignDWord(long value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    unsigned long long readQWord(size_t pos = 0) const final {
          return readValue<unsigned long long>(pos);
    }

    void writeQWord(unsigned long long value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    long long readSignQWord(size_t pos = 0) const final {
        return readValue<long long>(pos);
    }

    void writeSignQWord(long long value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    float readFloat(size_t pos = 0) const final {
        return readValue<float>(pos);
    }

    void writeFloat(float value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    double readDouble(size_t pos = 0) const final {
        return readValue<double>(pos);
    }

    void writeDouble(double value, size_t pos = 0) final {
        writeValue(value,pos);
    }

    void readBytes( std::vector<unsigned char>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeBytes( const std::vector<unsigned char>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readWords( std::vector<unsigned short>& dataRange, size_t count, size_t pos) const final {
        readValues(dataRange, count, pos);
    }

    void writeWords( const std::vector<unsigned short>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readDWords( std::vector<unsigned long>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeDWords( const std::vector<unsigned long>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readQWords( std::vector<unsigned long long>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeQWords( const std::vector<unsigned long long>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readSignBytes( std::vector<char>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeSignBytes( const std::vector<char>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readSignWords( std::vector<short>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeSignWords( const std::vector<short>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readSignDWords( std::vector<long>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeSignDWords( const std::vector<long>& dataRange, size_t pos = 0) final {
        writeValues(dataRange, pos);
    }

    void readSignQWords( std::vector<long long>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeSignQWords( const std::vector<long long>& dataRange, size_t pos = 0) final {
        writeValues(dataRange,pos);
    }

    void readFloats( std::vector<float>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeFloats( const std::vector<float>& dataRange, size_t pos = 0) final {
        writeValues(dataRange, pos);
    }

    void readDoubles( std::vector<double>& dataRange, size_t count, size_t pos = 0) const final {
        readValues(dataRange, count, pos);
    }

    void writeDoubles( const std::vector<double>& dataRange, size_t pos = 0) final {
        writeValues(dataRange, pos);
    }

    kdlib::DataAccessorPtr nestedCopy( size_t startOffset = 0, size_t length = 0 ) final {
        AutoSavePythonState  pystate(&m_pystate);
        return kdlib::DataAccessorPtr( new PythonObjectAccessor(m_object, m_startPos + startOffset) );
    }

	kdlib::DataAccessorPtr externalCopy(kdlib::MEMOFFSET_64 startAddr = 0, size_t length = 0) final {
		AutoSavePythonState  pystate(&m_pystate);
		return kdlib::DataAccessorPtr(new PythonObjectAccessor(m_object, startAddr));
	}

	virtual bool checkRange(kdlib::MEMOFFSET_64 startAddr, size_t length) const
	{
		return startAddr >= m_startPos;
	}

    std::wstring getLocationAsStr() const final {
        return L"python byte sequance";
    }

    kdlib::MEMOFFSET_64 getAddress() const final {
        throw kdlib::DbgException("python accessor error");
    }

    kdlib::VarStorage getStorageType() const final {
        throw kdlib::DbgException("python accessor error");
    }

    std::wstring getRegisterName() const final {
        throw kdlib::DbgException("python accessor error");
    }

private:

    python::object   m_object;

    mutable PyThreadState*  m_pystate;

    size_t  m_startPos;

    template<typename T>
    T readValue(size_t pos) const 
    {
        AutoSavePythonState  pystate(&m_pystate);
        try
        {
            return readValueUnsafe<T>(pos);
        }
        catch( python::error_already_set const & )
        {}

        throw kdlib::DbgException("python accessor error");
    }

    template<typename T>
    T readValueUnsafe(size_t pos) const 
    {
        unsigned long long  value = 0;
        for ( size_t i = sizeof(T); i > 0; --i)
            value = ( value << 8 ) + python::extract<unsigned char>(m_object[m_startPos + pos*sizeof(T) + i - 1])();

        return *reinterpret_cast<T*>(&value);
    }

    template<typename T>
    void writeValue(T value, size_t pos)
    {
        AutoSavePythonState  pystate(&m_pystate);
        try
        {
            writeValueUnsafe(value,pos);
            return;
        }
        catch( python::error_already_set const & )
        {}

        throw kdlib::DbgException("python accessor error");
    }

    template<typename T>
    void writeValueUnsafe(T value, size_t pos)
    {
        unsigned long long  val = *reinterpret_cast<unsigned long long*>(&value);
        for ( size_t i = 0; i < sizeof(T); ++i )
            m_object[m_startPos + pos*sizeof(T) + i] = static_cast<unsigned char>( ( val >> (8*i)) & 0xFF );
    }

    template<typename T>
    void readValues( std::vector<T>&  dataRange, size_t count, size_t pos) const
    {
        AutoSavePythonState  pystate(&m_pystate);
        try 
        {
            dataRange.resize(count);
            for ( size_t  i = 0; i < count; ++i )
                dataRange[i] = readValueUnsafe<T>(pos + i);
            return;
        }
        catch( python::error_already_set const & )
        {}

        throw kdlib::DbgException("python accessor error");
    }

    template<typename T>
    void writeValues( const std::vector<T>&  dataRange, size_t pos) 
    {
        AutoSavePythonState  pystate(&m_pystate);
        try 
        {
            for ( size_t  i = 0; i < dataRange.size(); ++i )
                writeValueUnsafe(dataRange[i], pos + i );
            return;
        }
        catch( python::error_already_set const & )
        {}

        throw kdlib::DbgException("python accessor error");
    }
};


///////////////////////////////////////////////////////////////////////////////


}
