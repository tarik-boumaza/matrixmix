######################################################################
# Automatically generated by qmake (3.1) Wed Mar 6 22:16:58 2019
######################################################################

TEMPLATE = app
CONFIG += link_prl qt debug_and_release
QT += widgets

include(ext/latex/jkqtmathtext.pri);

DESTDIR = ./bin
OBJECTS_DIR = ./obj/
MOC_DIR = ./moc
TARGET = MatriXMiX

INCLUDEPATH += ext/Eigen/

DEFINES += QT_DEPRECATED_WARNINGS

# Input

HEADERS += src/Matrix.h \
	src/VectorX.h \
	src/Polynomial.h \
	src/Fraction.h \
	src/MatriXMiXTXT.h \
	src/MatrixLibrary.h \
	src/MainWindow.h \
    src/LibraryWindow.h \
    src/AddMatrixWidget.h \
    src/ShowMatrixWidget.h \
    src/AdditionWindow.h \
    src/SoustractionWindow.h \
    src/MultiplicationWindow.h \
    src/DivisionWindow.h \
    src/PowerWindow.h \
    src/Error.h \
    src/ScalarMultiplicationWindow.h \
    src/DeterminantWindow.h \
    src/MatrixViewWidget.h \
    src/MenuBar.h \
    src/Gauss.h \
    src/TraceWindow.h \
    src/RowReducedWindow.h \
    src/InverseWindow.h \
    src/KerImgDimWindow.h \
    src/PolynomialWindow.h \
    src/EigenWindow.h \
    src/DiagonalisationWindow.h \
    src/ExprEvalWindow.h

SOURCES += src/main.cpp \ 
	src/Matrix.cpp \
	src/VectorX.cpp \ 
	src/Polynomial.cpp \
	src/Fraction.cpp \
	src/MatriXMiXTXT.cpp \
	src/MatrixLibrary.cpp \
	src/MainWindow.cpp \
    src/LibraryWindow.cpp \
    src/AddMatrixWidget.cpp \
    src/ShowMatrixWidget.cpp \
    src/MatrixViewWidget.cpp \
    src/AdditionWindow.cpp \
    src/SoustractionWindow.cpp \
    src/MultiplicationWindow.cpp \
    src/DivisionWindow.cpp \
    src/PowerWindow.cpp \
    src/Error.cpp \
    src/ScalarMultiplicationWindow.cpp \
    src/DeterminantWindow.cpp \
    src/MenuBar.cpp \
    src/Gauss.cpp \
    src/TraceWindow.cpp \
    src/RowReducedWindow.cpp \
    src/InverseWindow.cpp \
    src/KerImgDimWindow.cpp \
    src/PolynomialWindow.cpp \
    src/EigenWindow.cpp \
    src/DiagonalisationWindow.cpp \
    src/ExprEvalWindow.cpp

RESOURCES += \
    data/data.qrc


