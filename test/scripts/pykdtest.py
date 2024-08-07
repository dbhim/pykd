#
#
#

import sys
import os
import unittest

# Dynamically append current pykd.pyd path to PYTHONPATH
sys.path.insert(0, os.path.abspath(os.curdir) )

import pykd

import target

#import intbase
import memtest
import moduletest
import typeinfo
import typedvar
import breakpoint
import regtest
import stacktest
import customtypestest
import mspdbtest
import excepttest
import targetprocess
import ehloadtest
import synsymtest
import taggedtest
import arm64dumptest
import armdumptest
import eventtest

pykd.initialize()

class StartProcessWithoutParamsTest(unittest.TestCase):
    def testStart(self):
       target.processId = pykd.startProcess( target.appPath )
       target.module = pykd.module( target.moduleName )
       target.module.reload();
      #  print "\n" + str( pykd.getSystemVersion() )
       pykd.go()

class TerminateProcessTest(unittest.TestCase):
    def testKill(self):
        pykd.killProcess( target.processId )

def getTestSuite( singleName = "" ):
    if singleName == "":
        return unittest.TestSuite(
           [
                #unittest.TestLoader().loadTestsFromTestCase( intbase.IntBaseTest ),

                unittest.TestLoader().loadTestsFromTestCase( StartProcessWithoutParamsTest ),
                # *** Test without start/kill new processes
                #unittest.TestLoader().loadTestsFromTestCase( moduletest.ModuleTest ),
                unittest.TestLoader().loadTestsFromTestCase( memtest.MemoryTest ),
                unittest.TestLoader().loadTestsFromTestCase( typeinfo.TypeInfoTest ),
                unittest.TestLoader().loadTestsFromTestCase( typedvar.TypedVarTest ),
                unittest.TestLoader().loadTestsFromTestCase( regtest.CpuRegTest ),
                unittest.TestLoader().loadTestsFromTestCase( customtypestest.CustomTypesTest ),
                unittest.TestLoader().loadTestsFromTestCase( synsymtest.SynSymTest ),
                # ^^^
                unittest.TestLoader().loadTestsFromTestCase( TerminateProcessTest ),

                unittest.TestLoader().loadTestsFromTestCase( breakpoint.BreakpointTest ),
                unittest.TestLoader().loadTestsFromTestCase( stacktest.StackTest ),
                unittest.TestLoader().loadTestsFromTestCase( stacktest.InlineStackTest ),
                unittest.TestLoader().loadTestsFromTestCase( stacktest.DiaRegToRegRelativeI386Test ),
                unittest.TestLoader().loadTestsFromTestCase( stacktest.DiaRegToRegRelativeAmd64Test ),
                unittest.TestLoader().loadTestsFromTestCase( mspdbtest.MsPdbTest ),
                unittest.TestLoader().loadTestsFromTestCase( targetprocess.ProcessTest ),
                unittest.TestLoader().loadTestsFromTestCase( ehloadtest.EhLoadTest ),
                unittest.TestLoader().loadTestsFromTestCase( taggedtest.TaggedTest ),
                unittest.TestLoader().loadTestsFromTestCase( arm64dumptest.ARM64DumpTest ),
                unittest.TestLoader().loadTestsFromTestCase( armdumptest.ARMDumpTest ),
                unittest.TestLoader().loadTestsFromTestCase( eventtest.OutputHandlerTest ),
               
                #unittest.TestLoader().loadTestsFromTestCase( excepttest.ExceptionTest ),
            ] ) 
    else:
       return unittest.TestSuite(
          [
                unittest.TestLoader().loadTestsFromTestCase( StartProcessWithoutParamsTest ),
                unittest.TestLoader().loadTestsFromName( singleName ),
                unittest.TestLoader().loadTestsFromTestCase( TerminateProcessTest )
          ] )
          
if __name__ == "__main__":

    print( "\nTesting PyKd ver. %s" % pykd.__version__ )
    print( "Directory: %s" % os.path.dirname(pykd.__file__) )

    target.appPath = os.path.join( os.path.dirname(pykd.__file__), "targetapp.exe" )
    target.moduleName = os.path.splitext(os.path.basename(target.appPath))[0]

    unittest.TextTestRunner(stream=sys.stdout, verbosity=2).run( getTestSuite() )
    #unittest.TextTestRunner(stream=sys.stdout, verbosity=2).run( getTestSuite("typedvar.TypedVarTest.testEvalExprScopeLong") )

    try: input = raw_input
    except NameError: pass
    
    input("Press <ENTER>...")
