#pragma  once

#include "kdlib/typeinfo.h"
#include "pythreadstate.h"

namespace pykd {

inline kdlib::MEMOFFSET_64 getSymbolOffset( const std::wstring &name )
{
    AutoRestorePyState  pystate;
    return kdlib::getSymbolOffset(name);
}

std::wstring findSymbol(  kdlib::MEMOFFSET_64 offset, bool showDisplacement = true );

python::tuple findSymbolAndDisp( ULONG64 offset );

inline size_t getSymbolSize( const std::wstring &name )
{
    AutoRestorePyState  pystate;
    return kdlib::getSymbolSize(name);
}

inline kdlib::TypeInfoPtr defineStruct( const std::wstring &structName, size_t align = 0 ) 
{
    AutoRestorePyState  pystate;
    return kdlib::defineStruct(structName, align);
}

inline kdlib::TypeInfoPtr defineUnion( const std::wstring& unionName )
{
    AutoRestorePyState  pystate;
    return kdlib::defineUnion(unionName);
}


inline kdlib::TypeInfoPtr getTypeInfoByName( const std::wstring &name )
{
    AutoRestorePyState  pystate;
    return kdlib::loadType( name );
}

struct TypeInfoAdapter : public kdlib::TypeInfo {

    static std::wstring getName( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getName();
    }

    static size_t getSize( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getSize();
    }

    static kdlib::BITOFFSET getBitOffset( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getBitOffset();
    }

    static kdlib::BITOFFSET getBitWidth( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.getBitWidth();
    }

    static kdlib::TypeInfoPtr getBitType( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.getBitType();
    }

    static size_t getElementCount( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElementCount();
    }

    static kdlib::MEMOFFSET_32 getElementOffset( kdlib::TypeInfo &typeInfo, const std::wstring &name ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElementOffset( name );
    }

    static std::wstring getElementName( kdlib::TypeInfo &typeInfo, size_t index ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElementName(index);
    }

    static kdlib::MEMOFFSET_64 getStaticOffset( kdlib::TypeInfo &typeInfo, const std::wstring &name )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElementVa( name );
    }


    static kdlib::TypeInfoPtr getElementByName( kdlib::TypeInfo &typeInfo, const std::wstring &name ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElement(name);
    }


    static kdlib::TypeInfoPtr getElementByIndex( kdlib::TypeInfo &typeInfo, size_t index )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getElement(index);
    }

    static kdlib::TypeInfoPtr ptrTo( kdlib::TypeInfo &typeInfo, size_t ptrSize = 0 ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.ptrTo(ptrSize);
    }

    static kdlib::TypeInfoPtr deref( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.deref();
    }

    static kdlib::TypeInfoPtr arrayOf( kdlib::TypeInfo &typeInfo, size_t size ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.arrayOf(size);
    }

    static bool isArray( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.isArray();
    }

    static bool isPointer( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isPointer();
    }

    static bool isVoid( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isVoid();
    }

    static bool isBase( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isBase();
    }

    static bool isUserDefined( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isUserDefined();
    }

    static bool isConstant( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isConstant();
    }

    static bool isEnum( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.isEnum();
    }

    static bool isBitField( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isBitField();
    }

    static bool isFunction( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.isFunction();
    }
    
    static void appendField( kdlib::TypeInfo &typeInfo, const std::wstring &fieldName, kdlib::TypeInfoPtr &fieldType )
    {
        AutoRestorePyState  pystate;
        typeInfo.appendField( fieldName, fieldType );
    }

    static kdlib::CallingConventionType getCallingConvention( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getCallingConvention();
    }

    static kdlib::TypeInfoPtr getClassParent( kdlib::TypeInfo &typeInfo )
    {
        AutoRestorePyState  pystate;
        return typeInfo.getClassParent();
    }

    static std::wstring str( kdlib::TypeInfo &typeInfo ) 
    {
        AutoRestorePyState  pystate;
        return typeInfo.str();
    }

};

struct BaseTypesEnum {
    static kdlib::TypeInfoPtr getUInt1B() { return pykd::getTypeInfoByName(L"UInt1B"); }
    static kdlib::TypeInfoPtr getUInt2B() { return pykd::getTypeInfoByName(L"UInt2B");  }
    static kdlib::TypeInfoPtr getUInt4B() { return pykd::getTypeInfoByName(L"UInt4B");  }
    static kdlib::TypeInfoPtr getUInt8B() { return pykd::getTypeInfoByName(L"UInt8B");  }
    static kdlib::TypeInfoPtr getInt1B() { return pykd::getTypeInfoByName(L"Int1B"); }
    static kdlib::TypeInfoPtr getInt2B() { return pykd::getTypeInfoByName(L"Int2B"); }
    static kdlib::TypeInfoPtr getInt4B() { return pykd::getTypeInfoByName(L"Int4B"); }
    static kdlib::TypeInfoPtr getInt8B() { return pykd::getTypeInfoByName(L"Int8B"); }
    static kdlib::TypeInfoPtr getLong() { return pykd::getTypeInfoByName(L"Long"); }
    static kdlib::TypeInfoPtr getULong() { return pykd::getTypeInfoByName(L"ULong"); }
    static kdlib::TypeInfoPtr getBool() { return pykd::getTypeInfoByName(L"Bool"); }
    static kdlib::TypeInfoPtr getChar() { return pykd::getTypeInfoByName(L"Char"); }
    static kdlib::TypeInfoPtr getWChar() { return pykd::getTypeInfoByName(L"WChar"); }
    static kdlib::TypeInfoPtr getVoidPtr() { return pykd::getTypeInfoByName(L"Void*"); }
};

} // end namespace pykd