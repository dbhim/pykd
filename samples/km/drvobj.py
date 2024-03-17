from pykd import *
import sys


def loadSymbols():
   global nt
   nt = module( "nt" )


def getObjNameFromObjHeader( objHeader ):

    if hasattr( objHeader, "NameInfoOffset"):
        objName = typedVar( "nt!_OBJECT_HEADER_NAME_INFO",  objHeader.getAddress() - objHeader.NameInfoOffset )
    else:
        if (0 == (objHeader.InfoMask & 2)):
           return ""
      
        offsetNameInfo = ptrByte( nt.ObpInfoMaskToOffset + (objHeader.InfoMask & 3) )

        if (0 == offsetNameInfo):
            return ""

        objName = nt.typedVar("_OBJECT_HEADER_NAME_INFO", objHeader.getAddress() - offsetNameInfo)

    return loadUnicodeString( objName.Name.getAddress() )
    
    
def getTypeObjectByObjectHeader (objHeader):

    # Надо дизассемблировать функцию nt!ObGetObjectType
    if (ptrWord(nt.NtBuildNumber) < 10074):
        index = objHeader.TypeIndex
    else:
        # Начиная с "Windows 10 Technical Preview Build 10074" индекс в заголовке обфусцируют
        index = objHeader.TypeIndex ^ ((objHeader >> 8) & 0xFF) ^ ptrByte(nt.ObHeaderCookie)

    return ptrPtr( nt.ObTypeIndexTable + ptrSize() * index )


def getObjTypeFromObjHeader (objHeader):

    if hasattr (objHeader, "Type"):
        return objHeader.Type

    return getTypeObjectByObjectHeader (objHeader)


def isDirectory (obj):
    
    objHeader = containingRecord (obj, "nt!_OBJECT_HEADER", "Body")
    return getObjTypeFromObjHeader (objHeader) == ptrPtr (nt.ObpDirectoryObjectType)


def getObjectInDir( dirObj, objName ):

    # print ("%x %s" % (dirObj, getObjNameFromObjHeader(containingRecord(dirObj, "nt!_OBJECT_HEADER", "Body"))))
    if objName.find( "\\" ) != -1:
        ( dirSubName, objSubName ) =  objName.split("\\", 1)
        # print ("%s %s" % (dirSubName, objSubName))
    else:
        dirSubName = objName

 
    for i in range( 0, 37 ):

        if dirObj.HashBuckets[i] != 0:
            dirEntry = typedVar( "nt!_OBJECT_DIRECTORY_ENTRY", dirObj.HashBuckets[i] )

            while dirEntry != 0:
              
                curObj = dirEntry.Object

                curObjHeader = containingRecord( curObj, "nt!_OBJECT_HEADER", "Body" )

                curObjName = getObjNameFromObjHeader( curObjHeader )
                # print ("%d %s" % (i, curObjName))

                if curObjName.lower() == dirSubName.lower():

                    if isDirectory(curObj):
                        return getObjectInDir( typedVar( "nt!_OBJECT_DIRECTORY", curObj), objSubName )
                    else:
                        return curObj

                if dirEntry.ChainLink != 0:
                    dirEntry = typedVar( "nt!_OBJECT_DIRECTORY_ENTRY", dirEntry.ChainLink )
                else:
                    dirEntry = 0    


def getObjectByName( objName ):

    if len(objName)==0: 
        return

    if objName[0] != '\\':
        return

    rootDir = typedVar( "nt!_OBJECT_DIRECTORY", ptrPtr( nt.ObpRootDirectoryObject ) )
   
    return getObjectInDir( rootDir, objName[1:] )


def printDrvMajorTable( drvName ):

    objName = "\\Driver\\" + drvName
    drvObjPtr = getObjectByName( objName )
    
    if drvObjPtr == None:
        dprintln( "object not found" )
        return
        
    print ("%s %x" % (objName, drvObjPtr))
 
    drvObj = typedVar( "nt!_DRIVER_OBJECT", drvObjPtr )
     
    for i in range( len(drvObj.MajorFunction) ):
        dprintln( "MajorFunction[%d] = %s" % ( i, findSymbol( drvObj.MajorFunction[i] ) ) )


def run():

    if not isWindbgExt():
        if not loadDump( sys.argv[1] ):
             dprintln( sys.argv[1] + " - load failed" )
             return

    if not isKernelDebugging():
        dprintln( "not a kernel debugging" )
        return  

    loadSymbols();

    printDrvMajorTable( "afd" )
    printDrvMajorTable( "ntfs" )

if __name__ == "__main__":
    run()


