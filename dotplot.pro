TEMPLATE	= app
CONFIG		+= qt release thread
HEADERS		= \
		dotter.h \
                indexDotter.h \
		dotterWidget.h \
		dotterWindow.h \
		fastaReader.h \
		sequence.h
SOURCES		= \
		dotter.cpp \
                indexDotter.cpp \
		dotterWidget.cpp \
		dotterWindow.cpp \
		fastaReader.cpp \
		main.cpp
TARGET		= megaDotter
