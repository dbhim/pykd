
import sys
import os.path

from pykd import dprintln
from pykd import dprint

scriptPath = sys.argv[0]

def printCommand(sampleDescription, sampleName):
    dprintln( f'{sampleDescription} <link cmd="!pykd.py {scriptPath} run {sampleName}">Run</link> <link cmd="!pykd.py {scriptPath} source {sampleName}">Source</link>', True)

def printAllSamples():
    dprintln( "<b>User mode</b>", True)
    printCommand ("Get critical sections list", "um.critlist")
    printCommand ("Get module list from PEB", "um.ldr")
    #printCommand ("CreateFile", "um.createfile")
    dprintln( "<b>Kernel mode</b>", True )
    printCommand ("Get process list", "km.proclist")
    printCommand ("Get kernel service", "km.ssdt")
    printCommand ("Get driver object", "km.drvobj")
    dprintln( "" )

def runSample( sampleName ):

    try:
        packageName, moduleName = sampleName.split(".")

        module = __import__( name = sampleName, fromlist = moduleName  )

        module.__dict__[ "run" ]()

    except ImportError:
        dprintln("import error")
        pass

    dprintln( "" )
    dprintln( f'<link cmd="!pykd.py {scriptPath}">Sample list</link>', True )
    dprintln( "" )  

def printSample( sampleName ):

    try:
        packageName, moduleName = sampleName.split(".")

        module = __import__( name = sampleName, fromlist = moduleName )

        fileName = os.path.dirname( module.__dict__["__file__"] )
        fileName = os.path.join( fileName, moduleName + ".py" )

        with open( fileName ) as f:
            for line in f:
                dprint( line )            

    except ImportError:
        dprintln("import error")
        pass

    dprintln( "" )
    dprintln( f'<link cmd="!pykd.py {scriptPath}">Sample list</link>', True )
    dprintln( "" )  
  

def main():
    if len(sys.argv) <= 2:
        return printAllSamples()

    if sys.argv[1] == "run":
        runSample( sys.argv[2] )

    if sys.argv[1] == "source":
        printSample( sys.argv[2] )
        

if __name__ == "__main__":
    main()




