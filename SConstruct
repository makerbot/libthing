import commands

env = Environment( )

CPPATH = 'cpp/src/'

#For json-cpp we expect it to be 'next door' at the same level

JSON_CPP_PATH = '../json-cpp'


libthing_cc = [ CPPATH+'Scalar.cc',
				CPPATH+'Vector2.cc', 
				CPPATH+'Vector3.cc',
				CPPATH+'Triangle3.cc',
				CPPATH+"LineSegment2.cc",
				CPPATH+"Mesh.cc",
				CPPATH+'StlReader.cc',
				CPPATH+'StlWriter.cc']
	
print 'Building libthing.a'
env.Library('./bin/thing', libthing_cc)

default_libs = ['thing',]
default_libs_path = ['./bin','/usr/lib', '/usr/local/lib',
					 './bin/lib', '/opt/local/lib']


print 'testing cppunit settings'
operating_system = commands.getoutput("uname")

if operating_system == "Linux":
    print " ** CPPUNIT version checK:", commands.getoutput("dpkg -l|grep cppunit-dev")

if operating_system.find("_NT") > 0:
    print " ** CPPUNIT version checK:", "N/A"#commands.getoutput("cygcheck -l cppunit")

if operating_system == "Darwin":
    print " ** CPPUNIT version checK:", commands.getoutput("port info --line cppunit | grep ^cppunit")

debug_libs=['cppunit',]
debug_libs_path=['bin',]
p = env.Program('./bin/unit_tests/libThingCoreUnitTest',
	('cpp/tests/src/libThingCoreUnitTest.cc','cpp/tests/src/UnitTestMain.cc'),
	LIBS = default_libs + debug_libs,
	LIBPATH= default_libs_path + debug_libs_path,
	CPPPATH=['./'])

