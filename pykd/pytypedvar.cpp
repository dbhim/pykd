#include "stdafx.h"

#include "pytypedvar.h"
#include "kdlib/exceptions.h"

#include "pytypeinfo.h"
#include "pydataaccess.h"
#include "kdlib/dataaccessor.h"


namespace pykd {

///////////////////////////////////////////////////////////////////////////////

kdlib::DataAccessorPtr getDumpAccessor (kdlib::MEMOFFSET_64 addr, const python::list &listValues, const std::wstring &locationName)
{
	std::vector<unsigned char> vectorValues = listToVector<unsigned char>(listValues);

	return kdlib::getDumpAccessor(vectorValues, addr, locationName);
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr getTypedVarFromDumpByTypeName(const std::wstring &typeName, kdlib::MEMOFFSET_64 addr, const python::list &list)
{
	std::vector<unsigned char> values = listToVector<unsigned char>(list);

	std::wostringstream location;

	location << L"dump_" << typeName << L'_' << std::hex << addr;

	AutoRestorePyState  pystate;
	return kdlib::loadTypedVar(typeName, kdlib::getDumpAccessor(values, addr, location.str()));
}


kdlib::TypedVarPtr getTypedVarFromDumpByTypeInfo(const kdlib::TypeInfoPtr &typeInfo, kdlib::MEMOFFSET_64 addr, const python::list &list)
{
	std::vector<unsigned char> values = listToVector<unsigned char>(list);

	std::wostringstream location;

	location << L"dump_" << typeInfo->getName() << L'_' << std::hex << addr;

	AutoRestorePyState  pystate;
	return kdlib::loadTypedVar(typeInfo, kdlib::getDumpAccessor(values, addr, location.str()));
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr getTypedVarFromAccessorByTypeName(const std::wstring &typeName, kdlib::DataAccessorPtr dataAccessor)
{
	AutoRestorePyState  pystate;
	return kdlib::loadTypedVar(typeName, dataAccessor);
}


kdlib::TypedVarPtr getTypedVarFromAccessorByTypeInfo(const kdlib::TypeInfoPtr &typeInfo, kdlib::DataAccessorPtr dataAccessor)
{
	AutoRestorePyState  pystate;
	return kdlib::loadTypedVar(typeInfo, dataAccessor);
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr getTypedVarByTypeName(const std::wstring &name, python::object& dataStorage)
{
    python::extract<kdlib::MEMOFFSET_64>  get_addr(dataStorage);
    if ( get_addr.check() )
    {
        kdlib::MEMOFFSET_64 offset = get_addr();
        AutoRestorePyState pystate;
        return kdlib::loadTypedVar( name, offset );
    }

    kdlib::DataAccessorPtr dataAccesor( new PythonObjectAccessor(dataStorage) );

    AutoRestorePyState pystate;
    return kdlib::loadTypedVar(name, dataAccesor);
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr getTypedVarByTypeInfo( const kdlib::TypeInfoPtr &typeInfo, python::object& dataStorage)
{
    python::extract<kdlib::MEMOFFSET_64>  get_addr(dataStorage);
    if ( get_addr.check() )
    {
        kdlib::MEMOFFSET_64   offset = get_addr();
        AutoRestorePyState  pystate;
        return kdlib::loadTypedVar(typeInfo, offset );
    }

    kdlib::DataAccessorPtr   dataAceesor( new PythonObjectAccessor(dataStorage) );

    AutoRestorePyState  pystate;
    return kdlib::loadTypedVar(typeInfo, dataAceesor );
}

///////////////////////////////////////////////////////////////////////////////

python::list getTypedVarListByTypeName( kdlib::MEMOFFSET_64 offset, const std::wstring &typeName, const std::wstring &fieldName )
{
    kdlib::TypedVarList  lst;

    do {
        AutoRestorePyState  pystate;
        lst = kdlib::loadTypedVarList( offset, typeName, fieldName );
    } while(false);

    return vectorToList( lst );
}

///////////////////////////////////////////////////////////////////////////////

python::list getTypedVarListByType( kdlib::MEMOFFSET_64 offset, kdlib::TypeInfoPtr &typeInfo, const std::wstring &fieldName )
{
    kdlib::TypedVarList  lst;

    do {
        AutoRestorePyState  pystate;
        lst = kdlib::loadTypedVarList( offset, typeInfo, fieldName );
    } while(false);

    return vectorToList( lst );
}

///////////////////////////////////////////////////////////////////////////////

python::list getTypedVarArrayByTypeName( kdlib::MEMOFFSET_64 offset, const std::wstring &typeName, size_t number )
{
    kdlib::TypedVarList  lst;
    
    do {
        AutoRestorePyState  pystate;
        lst = kdlib::loadTypedVarArray( offset, typeName, number );
    } while(false);

    return vectorToList( lst );
}

///////////////////////////////////////////////////////////////////////////////

python::list getTypedVarArrayByType( kdlib::MEMOFFSET_64 offset, kdlib::TypeInfoPtr &typeInfo, size_t number )
{
    kdlib::TypedVarList  lst;
    
    do {
        AutoRestorePyState  pystate;
        lst = kdlib::loadTypedVarArray( offset, typeInfo, number );
    } while(false);

    return vectorToList( lst );
}

///////////////////////////////////////////////////////////////////////////////

python::list TypedVarAdapter::getFields(const kdlib::TypedVarPtr& typedVar)
{
    typedef boost::tuple<std::wstring,kdlib::MEMOFFSET_32,kdlib::TypedVarPtr> FieldTuple;

    std::list<FieldTuple>  lst;

    do {

        AutoRestorePyState  pystate;

        for ( size_t i = 0; i < typedVar->getElementCount(); ++i )
        {
            std::wstring  name = typedVar->getElementName(i);
            kdlib::MEMOFFSET_32  offset = 0;

            if (typedVar->getType()->isConstMember(i))
                continue;

            if (!typedVar->getType()->isStaticMember(i) )
                offset = typedVar->getElementOffset(i);

            kdlib::TypedVarPtr  val = typedVar->getElement(i);

            lst.push_back( FieldTuple( name, offset, val ) );
        }

    } while(false);

    python::list pylst;
    
    for ( std::list<FieldTuple>::const_iterator it = lst.begin(); it != lst.end(); ++it)
        pylst.append( python::make_tuple( it->get<0>(), it->get<1>(), it->get<2>() ) );

    return pylst;
}

python::list TypedVarAdapter::getMembers(const kdlib::TypedVarPtr& typedVar)
{
    typedef boost::tuple<std::wstring, kdlib::MEMOFFSET_32, kdlib::TypedVarPtr> FieldTuple;

    std::list<FieldTuple>  lst;

    do {

        AutoRestorePyState  pystate;

        auto varType = typedVar->getType();

        for (size_t i = 0; i < typedVar->getElementCount(); ++i)
        {
            if (varType->isConstMember(i) || varType->isInheritedMember(i))
                continue;                     

            std::wstring  name = typedVar->getElementName(i);
            kdlib::MEMOFFSET_32  offset = 0;

            if (!varType->isStaticMember(i))
                offset = typedVar->getElementOffset(i);

            kdlib::TypedVarPtr  val = typedVar->getElement(i);

            lst.push_back(FieldTuple(name, offset, val));
        }

    } while (false);

    python::list pylst;

    for (std::list<FieldTuple>::const_iterator it = lst.begin(); it != lst.end(); ++it)
        pylst.append(python::make_tuple(it->get<0>(), it->get<1>(), it->get<2>()));

    return pylst;
}

///////////////////////////////////////////////////////////////////////////////

bool TypedVarAdapter::hasField(kdlib::TypedVarPtr& typedVar, const std::wstring &fieldName)
{
    AutoRestorePyState  pystate;

    for (size_t i = 0; i < typedVar->getElementCount(); ++i)
    {
        std::wstring  name = typedVar->getElementName(i);
        if (name == fieldName)
            return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

bool TypedVarAdapter::hasMethod(kdlib::TypedVarPtr& typedVar, const std::wstring &name)
{
    AutoRestorePyState  pystate;

    try {
        typedVar->getMethod(name);
        return true;
    }
    catch (kdlib::DbgException&)
    {
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////

python::list TypedVarAdapter::getElementsDir(kdlib::TypedVar& typedVar)
{
    std::list<std::wstring>  lst;
    python::list pylst;

    try {

        AutoRestorePyState  pystate;

        for (size_t i = 0; i < typedVar.getElementCount(); ++i)
        {
            std::wstring  name = typedVar.getElementName(i);
            lst.push_back(name);
        }

    } catch(kdlib::DbgException&)
    {
        return pylst;
    }

    for (std::list<std::wstring>::const_iterator it = lst.begin(); it != lst.end(); ++it)
        pylst.append(*it);

    return pylst;
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr TypedVarAdapter::getFieldAttr(kdlib::TypedVar& typedVar, const std::wstring &name)
{
    if (name == L"__name__")
        throw AttributeException("no __name__ attribute");

    AutoRestorePyState  pystate;

    try
    {
        return typedVar.getElement( name );
    }
    catch (kdlib::TypeException&)
    {}


    try
    {
        return typedVar.getMethod(name);
    }
    catch (kdlib::TypeException&)
    {
    }


    std::stringstream sstr;
    sstr << "typed var has no field " << '\'' << _bstr_t(name.c_str()) << '\'';
    throw AttributeException(sstr.str().c_str());
}

///////////////////////////////////////////////////////////////////////////////

python::list TypedVarAdapter::getRawBytes(kdlib::TypedVar& typedVar)
{

   std::vector<unsigned char>   rawBytes;

   {
        AutoRestorePyState  pystate;
        kdlib::DataAccessorPtr   dataStream = kdlib::getCacheAccessor(typedVar.getSize());
        typedVar.writeBytes(dataStream);
        dataStream->readBytes(rawBytes, typedVar.getSize());

   }

   return vectorToList( rawBytes );
 }

///////////////////////////////////////////////////////////////////////////////

void TypedVarAdapter::setField(kdlib::TypedVar& typedVar, const std::wstring &name, python::object&  object)
{

    kdlib::TypedValue  value = NumVariantAdaptor::convertToVariant(object);

    {
        AutoRestorePyState  pystate;
        typedVar.setElement(name, value);
    }
}

///////////////////////////////////////////////////////////////////////////////

void TypedVarAdapter::setElementByIndex(kdlib::TypedVar& typedVar, long index, python::object& object)
{
    kdlib::TypedValue  value = NumVariantAdaptor::convertToVariant(object);

    {
        AutoRestorePyState  pystate;
        typedVar.setElement(index, value);
    }
}

///////////////////////////////////////////////////////////////////////////////

void TypedVarAdapter::setFieldAttr(kdlib::TypedVar& typedVar, const std::wstring &name, python::object&  object)
{
    kdlib::TypedValue  value = NumVariantAdaptor::convertToVariant(object);

    try
    {
         AutoRestorePyState  pystate;
         typedVar.setElement(name, value);
         return;
    }
    catch (kdlib::TypeException&)
    {}

    std::wstringstream sstr;
    sstr << L"typed var has no field " << L'\'' << name << L'\'';
    throw AttributeException(std::string(_bstr_t(sstr.str().c_str())).c_str());
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedVarPtr TypedVarAdapter::getFieldByKey(kdlib::TypedVar& typedVar, const std::wstring &name)
{
    {
        AutoRestorePyState  pystate;

        try
        {
            return typedVar.getElement( name );
        }
        catch (kdlib::TypeException&)
        {}

        try
        {
            return typedVar.getMethod( name );
        }
        catch (kdlib::TypeException&)
        {}
    }

    std::wstringstream sstr;
    sstr << L"typed var has no field " << L'\'' << name << L'\'';
    throw KeyException(std::string(_bstr_t(sstr.str().c_str())).c_str());
}

///////////////////////////////////////////////////////////////////////////////

void TypedVarAdapter::setFieldByKey(kdlib::TypedVar& typedVar, const std::wstring &name, python::object&  object)
{
    kdlib::TypedValue  value = NumVariantAdaptor::convertToVariant(object);

    try
    {
         AutoRestorePyState  pystate;
         typedVar.setElement(name, value);
         return;
    }
    catch (kdlib::TypeException&)
    {}

    std::wstringstream sstr;
    sstr << L"typed var has no field " << L'\'' << name << L'\'';
    throw KeyException(std::string(_bstr_t(sstr.str().c_str())).c_str());
}

///////////////////////////////////////////////////////////////////////////////

class PyScope : public kdlib::Scope
{
public:

    kdlib::TypedValue get(const std::wstring& varName) const override
    {
        return getTypdedValueFromPyObj(m_scope[varName]);
    }

    virtual bool find(const std::wstring& varName, kdlib::TypedValue& value) const override
    {
        if (!m_scope.contains(varName))
            return false;

        value = getTypdedValueFromPyObj(m_scope[varName]);

        return true;
    }

    PyScope(const python::object& scope) :
        m_scope(scope)
    {}

private:

    python::object  m_scope;
};

kdlib::TypedVarPtr evalExpr(const std::string  expression, python::object&  scope, kdlib::TypeInfoProviderPtr& typeInfoProvider )
{
    if (scope)
    {
        return kdlib::evalExpr(expression, kdlib::ScopePtr(new PyScope(scope)), typeInfoProvider).get();
    }

    return kdlib::evalExpr(expression).get();
}

///////////////////////////////////////////////////////////////////////////////

kdlib::TypedValue  getTypdedValueFromPyObj(const python::object& value)
{
    python::extract<kdlib::TypedVarPtr>  getTypedVar(value);
    if (getTypedVar.check())
        return getTypedVar();

    return NumVariantAdaptor::convertToVariant(value);
}

///////////////////////////////////////////////////////////////////////////////


} // namespace pykd

