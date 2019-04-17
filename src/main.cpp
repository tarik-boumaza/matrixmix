
//#include <QApplication>
#include <iostream>
//#include <QDir>
//#include <QDebug>
//#include "MainWindow.h"
//#include "LibraryWindow.h"
#include "Matrix.h"
#include "MatriXMiXTXT.h"
#include "MatrixLibrary.h"

using namespace std;


//extern const string APP_PATH = QDir(QApplication::applicationDirPath() + "/../../../").absolutePath().toStdString();

int main (int argc, char* argv[])
{

    MatrixLibrary lib;
    vector<string> resultat;
    lib.cleanSaves();
    string expression1("((objet-1)+x*tmp/8)/25-12");

    Matrix a (2,2,{1,0,0,1});
    Matrix b (2,2,{0,0,0,1});
    Matrix res;

    lib.addMatrix("identite",a);
    lib.addMatrix("matrice",b);
    lib.saveMatrix("identite");
    lib.saveMatrix("matrice");



/*
    resultat = lib.decoupe("matrice+identite-2*matrice-3*identite");
    lib.polonaise("matrice+identite-2*matrice-3*identite",resultat);

    for(unsigned int i = 0 ; i < resultat.size() ; i++)
    {
        cout << resultat[i] ;
    }
*/

    res = lib.expressionCalcul("(matrice+identite-2*matrice-3*identite)/matrice");
    cout << res << endl;



    //lib.addMatrix("I+B",res);
    //lib.saveMatrix("I+B");

    //cout << lib.readMatrix("I+B") << endl << endl;




    return 0;

}


