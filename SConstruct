# - Glob() adds all the files with the extension to the list
# - CPPPATH define the path where to look for source files which have include dependendcies
#		so that when these include files change, a recompilation of the source file is triggered
#		CPPPATH = ['include', '/home/project/inc']



env = Environment()
env.Program(    target='gateway',
                source=[Glob('*.cpp'),
                            [   'pilibs/serial.cpp',
                                'pilibs/utils.cpp',
                                'pilibs/mesh.cpp',
                                'pilibs/mqtt_rf.cpp',
                                'pilibs/bme280_server.cpp',
                                'pilibs/log.cpp'
                            ],
                        ],
                CPPPATH = ['.','pilibs/'],
                CCFLAGS = ['-std=c++11'],
				LIBS=['libmosquittopp'],
				LIBPATH='/usr/local/lib/',
				LINKFLAGS=[]
            )
