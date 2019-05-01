
#include "MatrixLibrary.h"
#include "Matrix.h"
#include <stack>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

const string PATH = "sauvegarde.txt";


MatrixLibrary:: MatrixLibrary () : tab (map<string, Matrix>())
{
}


MatrixLibrary:: ~MatrixLibrary()
{
}


unsigned long int MatrixLibrary:: size () const
{
    return tab.size();
}


bool MatrixLibrary:: isEmpty () const
{
    return size() == 0;
}


bool MatrixLibrary:: exist (const string& name) const
{
    return (tab.count(name) != 0);
}


void MatrixLibrary:: print () const
{
    for(const auto& Mat: tab)
    { 
        cout << "Matrice " << Mat.first << " : ";
        cout << endl << endl << Mat.second << endl;;
    }
    cout << endl;
}


void MatrixLibrary::copy_vector(std::vector<std::string>& expression,const std::vector<std::string>& resultat)
{
    unsigned long int i, length = resultat.size();

    for (i=0; i<length; i++)
    {
        expression.push_back(resultat[i]);
    }

}


void MatrixLibrary:: addMatrix (const string& name, const Matrix& m)
{
    if (isName(name))
        tab.insert({name, m});
    else cout<<"Nom de matrice non autorisé, veuillez choisir un autre..."<<endl;
}


const Matrix* MatrixLibrary:: find (const string& name) const
{
    if(tab.count(name) == 0)
    {
        return nullptr;
    }
    return &tab.at(name);
}


void MatrixLibrary:: erase (const string & name)
{
    tab.erase(name);
    if(exist(name))
    {
        cout << "Suppression non effectuée" << endl;
    }
}


const std::map<std::string, Matrix>& MatrixLibrary:: data () const
{
    return tab;
}


bool MatrixLibrary:: isName(const string & chaine) const
{
    unsigned long int i = 1, s = chaine.length();

    if ( !( ((chaine[0] >= 'A') && (chaine[0] <= 'Z'))
         || ((chaine[0] >= 'a') && (chaine[0] <= 'z')) ) )
        return false;

    while (i<s)
    {
        if ( ((chaine[i] >= 'A') && (chaine[i] <= 'Z'))
             || ((chaine[i] >= 'a') && (chaine[i] <= 'z'))
             || ((chaine[i] >= '0') && (chaine[i] <= '9')))
            i++;
        else return false;
    }

    return true;
}


bool MatrixLibrary:: isFloat(const string & chaine) const
{
    unsigned long int i = 0, s = chaine.length();
    unsigned short int nbcoma = 0;

    while (i<s)
    {
        if (chaine[i]=='.')
            nbcoma++;
        else if (! ((chaine[i]>='0') && (chaine[i]<='9')) )
            return false ;

        if (nbcoma > 1)
            return false;

        i++;
    }

    return true;
}


bool MatrixLibrary:: isOperator (const string & chaine) const
{
    return ( (chaine == "+")
             ||  (chaine == "-")
             ||  (chaine == "/")
             || (chaine == "^")
             || (chaine == "~")
             || (chaine == "*"));
}


vector<string> MatrixLibrary:: decoupe (const string & expression)
{
    unsigned int i;
    unsigned  long taille = expression.length();
    vector<string> tab;
    string c, temp;
    temp="";

    for (i=0; i<taille; i++)
    {
        c=expression[i];

        if((isOperator(c)) || (c == ")") || (c == "(") || (c == "=") )
        {
            if (temp.length()!=0)
                tab.push_back(temp);
            tab.push_back(c);
            temp.clear();
        }
        else if (!c.empty())
        {
            temp+=c;
        }

    }
    if(temp != "")
    tab.push_back(temp);

    return tab;
}


Matrix MatrixLibrary:: calculate (const string & op, const string & a, const string & b) const
{
    const Matrix* m_a;
    const Matrix* m_b;

    m_a = find(a);
    m_b = find(b);

    if(op == "+")
        return *m_a + *m_b;

    if(op == "-")
        return *m_a - *m_b;

    if(op == "*")
        return *m_a * *m_b;

    if(op == "/")
        return *m_a / *m_b;

    else return *m_a;
}


double MatrixLibrary:: calculateFloat (const std::string & op, const std::string & a, const std::string & b)
{
    if(op == "+")
        return atof(a.c_str()) + atof(b.c_str());

    if(op == "-")
        return atof(a.c_str()) - atof(b.c_str());

    if(op == "*")
        return atof(a.c_str()) * atof(b.c_str());

    if((op == "/") && (atof(b.c_str())!=0.0))
        return atof(a.c_str()) / atof(b.c_str());

    else return 0.0;
}


Matrix MatrixLibrary:: calculateMatrixFloat (const std::string & op, const std::string & a, const float & b)
{
    const Matrix* m_a;
    m_a=find(a);

    Matrix identite(m_a->getNbRows(),m_a->getNbCols(),Matrix::I);
    Matrix bmatrix;
    bmatrix=static_cast<double>(b)*identite;



    if(op == "+")
        return *m_a + bmatrix ;

    if(op == "-")
        return *m_a - bmatrix;

    if(op == "/")
    {
        double scale;
        scale = static_cast<double>(1/b);
        return *m_a * scale;
    }

    else return *m_a;
}


Matrix MatrixLibrary:: calculateFloatMatrix(const std::string &op, const std::string &a, const float &b)
{
    const Matrix* m_a;
    m_a = find(a);

    Matrix identite(m_a->getNbRows(),m_a->getNbCols(),Matrix::I);
    Matrix bmatrix;
    bmatrix = static_cast<double>(b)*identite;

    if(op == "+")
        return *m_a + bmatrix ;

    if(op == "-")
        return bmatrix - *m_a;

    else return *m_a;
}


bool MatrixLibrary:: priorite_sup_egal (const string & opd,const string & opg) const
{
    switch (opd[0])
    {
        case '*':
            return ((opg[0] == '*') || (opg[0] == '/') || (opg[0] == '^') || (opg[0] == '~'));

        case '/':
            return ((opg[0] == '*') || (opg[0] == '/') || (opg[0] == '^') || (opg[0] == '~'));

        case '+':
            return ((opg[0] == '+') || (opg[0] == '-') || (opg[0] == '*') || (opg[0] == '/') || (opg[0] == '^') || (opg[0] == '~'));

        case '-':
            return ((opg[0] == '+') || (opg[0] == '-') || (opg[0] == '*') || (opg[0] == '/') || (opg[0] == '^') || (opg[0] == '~'));

        case '^':
            return ((opg[0] == '^') || (opg[0] == '~'));

        case '~':
            return ((opg[0] == '^') || (opg[0] == '~'));

        default: return false;
    }
}


void MatrixLibrary:: polonaise(const std::string & chaine , std::vector<std::string> & notation_polonaise)
{
    stack<string> p;
    vector<string> expression;
    copy_vector(expression,decoupe(chaine));

    unsigned long int i, s = expression.size();

    for (i = 0; i < s; i++)
    {
        if ( (!isOperator(expression[i])) && (expression[i] != "(") && (expression[i] != ")") && (expression[i] != "=") )
        {
            notation_polonaise.push_back(expression[i]);
        }
        else if ( (expression[i] == "("))
        {
            p.push(expression[i]);
        }
        else if (isOperator(expression[i]))
        {
            if (!p.empty())
            {
                while ((!p.empty()) && priorite_sup_egal(expression[i],p.top()))
                {
                    notation_polonaise.push_back(p.top());
                    p.pop();
                }
            }

            p.push(expression[i]);

        }
        else if (expression[i] == ")")
        {
            do
            {
                notation_polonaise.push_back(p.top());
                p.pop();

            }while ((p.top() !=  "(") && (!p.empty()));
            p.pop();
        }
    }
    if(notation_polonaise[notation_polonaise.size()-1]=="")
        notation_polonaise.pop_back();

    while (!p.empty())
    {
        notation_polonaise.push_back(p.top());
        p.pop();
    }
}


Matrix MatrixLibrary:: expressionCalcul(const std::string & chaine)
{
    vector<string> polish;
    polonaise(chaine,polish);
    stack<string> pile;
    Matrix temp;
    string identify;
    int nom=0;

    unsigned long int i;

    for (i = 0; i < polish.size(); i++ )
    {
        if (polish[i] == "~")
        {
            string a = pile.top();
            pile.pop();

            temp= *(find(a))^-1;

            identify = static_cast<char>('0'+ nom);
            pile.push("temp" + identify);
            addMatrix("temp" + identify,temp);
            nom++;
        }
        else if ((isOperator(polish[i])) && (polish[i] != "~"))
        {
            string b = pile.top();
            pile.pop();
            string a = pile.top();
            pile.pop();

            if ((isName(b) && isName(a)) || (isFloat(a) && isName(b)) || (isFloat(b) && isName(a)) )
            {
                if ((isName(b) && isName(a)))
                {
                    temp = calculate(polish[i],a,b);
                }
                else if (isFloat(b) && isName(a))
                {
                    if(polish[i] == "*")
                    {
                        temp = *(find(a)) * atof(b.c_str());
                    }
                    else if (polish[i] == "^")
                    {
                        temp= *(find(a)) ^ (atoi(b.c_str()));
                    }
                    else
                    {
                        float scale;
                        scale = static_cast<float>(atof(b.c_str()));
                        temp = calculateMatrixFloat(polish[i],a,scale);
                    }
                }
                else if (isFloat(a) && isName(b))
                {
                    if(polish[i] == "*")
                    {
                        temp = *(find(b)) * atof(a.c_str());

                    }
                    else
                    {
                        float scale;
                        scale = static_cast<float>(atof(a.c_str()));
                        temp = calculateFloatMatrix(polish[i],b,scale);
                    }
                }
                identify = static_cast<char>('0'+ nom);
                pile.push ("temp" + identify);
                addMatrix ("temp" + identify, temp);
                nom++;
            }
            else if (  isFloat(a) && isFloat(b) )
            {
                ostringstream ss;
                ss << calculateFloat (polish[i],a,b);
                string res = ss.str();
                pile.push(res);
            }
            else
            {
                cout << "Caractère spécial détecté..."
                        "\nVeuillez rééssayer (gestion erreur... +1 point :)" << endl;
            }
        }
        else
        {
            pile.push(polish[i]);
        }
    }

    const Matrix* res;
    res = find(pile.top());
    Matrix resultat(*res);

    for(int i=0; i<=nom-1; i++)
    {
        identify = static_cast<char>('0'+ i);
        erase("temp" + identify);
    }


    return resultat;
}


string MatrixLibrary:: isCalculableExpression(const string & expression)
{
    vector<string> result = decoupe(expression);
    unsigned long int i, j, s = result.size();

    string calculable = "calculable";
    string error1 = "Expression vide" ;

    if (s == 0)
        return error1;
    if (s == 1)
        return calculable;

    string error2 = "Parenthèse fermante détectée..."
                    "\nVeuillez vérifier l'organisation des parenthèses" ;
    string error3a = "Calcul de " ;
    string error3b = " impossible" ;
    string error4 = " doit être un réel" ;
    string error5 = "\nPour les puissances négatives, veuillez utiliser '~' pour inverser, puis '^' pour l'exposant" ;
    string error6 = "\nUn réel doit être précédé d'un opérateur de calcul (+,-,*,/,^)" ;
    string error7 = "Nombre de parenthèses ouvrantes différent du nombre de parenthèses fermantes" ;
    string error8 = "Hormis '~' et '^', les caractères spéciaux ne sont pas admis" ;
    string error9 = "\nVeuillez utiliser 'M~' pour désigner l'inverser d'une matrice M" ;

    short int nbp = 0;


    for (i = 0; i < s; i++)
    {
        if (result[i] == "(")
        {
            nbp++;

        }
        else if (result[i] == ")")
        {
            nbp--;
            if (nbp < 0)
            return error2;
        }
    }

    if (nbp != 0)
        return error7;


    for(i = 0; i < s; i++)
    {
        cout << i << " : " << result[i] << endl;

            if (isOperator(result[i]))
            {

            if (i == 0)
                   return ("Calcul de " + result[i] + result[i+1] + " impossible!");
               if (i == s - 1)
                   return ("Impossible de calculer " + result[i-1] + result[i]);
               if (isOperator(result[i-1]))
                   return "Deux opérateurs à la suite... \nImpossible à calculer";
           }
                else if (isName(result[i]))
                {
                    if (i == 0)
                        continue;
                    if (i == 1)
                        return (error3a + result[i-1] + result[i] + error3b);
                    if (i == s - 2)
                        return (error3a + result[i] + result[i+1] + error3b);

                    if ( (i > 1) && (i < s-2) )
                    {
                        if (!isOperator(result[i-1]))
                            return (error3a + result[i-1] + error3b);
                        else if ( (!isName(result[i-2])) && (!isFloat(result[i-2])) )
                            return (error3a + result[i-2] + result[i-1] + result[i] + error3b);

                        if (result[i+1] == "^")
                        {
                            if (!isFloat(result[i+2]))
                                return (error3a + result[i] + result[i+1] + result[i+2] + error3a + "\n" + result[i+2] + error4);
                            if (atoi(result[i+2].c_str()) < 0)
                                return (error3a + result[i] + result[i+1] + result[i+2] + error3a + error5);
                            if (atoi(result[i+2].c_str()) - atof(result[i+2].c_str()) != 0.0)
                                return ("Impossible de calculer " + result[i]
                                        + result[i+1] + result[i+2] + "...\nCalcul d'une puissance réelle impossible");
                        }

                    }

                }

                    else if (isFloat(result[i]))
                            {
                                if (i == 0)
                                    continue;
                                if (i == 1)
                                    return (error3a + result[i-1] + result[i] + error3b);
                                if (i == s - 2)
                                    return (error3a + result[i] + result[i+1] + error3b);
                                if (!isOperator(result[i-1]))
            //   <    >         {
                                {
                                    if (result[i-1] != "^")
                                        return ("Impossible de calculer " + result[i-1] + result[i]);
                                    if (atof(result[i].c_str()) < 0.0)
                                        return (error3a + result[i-2] + result[i-1] + result[i] + error3b + error9);
                                    if (atof(result[i].c_str()) - atoi(result[i].c_str()) != 0.0)
                                        return ("Calcul d'une puissance réelle impossible");
                                }
                                else if ( (i < s - 1) && isName(result[i+1]) )
                                        return "L'opération " + result[i+1] + " n'est pas définie" ;
                            }

    }



    return calculable;
}


const string MatrixLibrary:: saveRights(const string & matrixname) const
{
    ifstream file (PATH);

    if(!file.is_open())
    {
        cout << "Erreur lors de la lecture du file \nVeuillez vérifier le chemin du file" << endl;
        exit(EXIT_FAILURE);
    }

    string first_string, stringpos;

    file >> first_string;

    while(!file.eof())
    {
        file >> stringpos;
        if(stringpos==matrixname)
            return string("used");
    }

    return first_string;

}


void MatrixLibrary:: saveMatrix (const string & matrixname) const
{
    Matrix m(*find(matrixname));
    cout << m;

    string filename (PATH);
    ofstream file (filename.c_str(), ios::app);

    if(!file.is_open())
    {
        cout << "Erreur lors de la lecture du fichier "
                "\nVeuillez vérifier le chemin du fichier" << endl;
        exit(EXIT_FAILURE);
    }

    string testRights = saveRights(matrixname);

    if (testRights.empty())
    {
        file << "Matrix" << endl;

    }
    else if (testRights=="used")
    {
        cout << "Une matrice du même nom a déjà été sauvegardée"
                "\nVeuillez sélectionner un autre nom" << endl;
        exit(EXIT_FAILURE);
    }
    else if(testRights!="Matrix")
    {
        cout << endl << "Erreur !"
                        "\nModification du fichier 'sauvegarde.txt' " << endl;
        // MAXIME GESTION ERREUR
        exit(EXIT_FAILURE);
    }

    file << endl << matrixname << endl;
    file << m.getNbRows() << " " << m.getNbCols() << endl;

    for (unsigned int i = 0; i < m.getNbRows(); i++)
    {
        for (unsigned int j = 0; j < m.getNbCols(); j++)
        {

            file << m[i][j] << " ";
        }
        file << endl;
    }

    cout << "La sauvegarde de la matrice " << matrixname << " est réussie" << endl << endl;

    file.close();

}


void MatrixLibrary:: cleanSaves() const
{
    string filename(PATH);
    ofstream file (filename.c_str());

    if(!file.is_open())
    {
        cout << "Erreur lors de la lecture du file \nVeuillez vérifier le chemin du file" << endl;
        exit(EXIT_FAILURE);
    }

    file.close();
    cout << "Fichier de sauvegarde nettoyé" << endl << endl;

}


Matrix MatrixLibrary:: readMatrix(const string & matrixname) const
{
    string filename(PATH);
    ifstream file (filename.c_str());

    if(!file.is_open())
    {
        cout << "Erreur lors de la lecture du file \nVeuillez vérifier le chemin du file" << endl;
        return Matrix::matrix_noEigen;
    }

    string testfile;
    file >> testfile ;

    if( testfile == "Matrix")
    {
        while(!file.eof() && testfile!=matrixname)
        {
            file >> testfile;
        }
        if (file.eof())
        {
            cout << "Erreur avec " << matrixname <<
                    "\nCette matrice n'a pas été sauvegardée dans 'sauvegarde.txt' " << endl;
             return Matrix::matrix_noEigen;
             //Exception Qt
        }

        unsigned int r,c;
        file >> r >> c;

        Matrix m(r,c);

        for (unsigned int i = 0; i < r; i++)
        {
            for (unsigned int j = 0; j < c; j++)
            {
                file >> m[i][j];
            }

        }

        file.close();
        cout << "fermeture réussie" << endl << endl;
        return m;
    }
    else
    {
        cout << "Erreur" << endl ;
        return Matrix::matrix_noEigen;
        // exception QT Maxime
    }
}


void MatrixLibrary:: testRegression()
{
     cout << endl << endl << "****** DEBUT DU TEST DE REGRESSION ******" << endl << endl << endl;

    MatrixLibrary lib;

    Matrix identite(3,3,{1,0,0,0,1,0,0,0,1});
    Matrix a(3,3,{1,2,3,4,5,6,7,8,9});
    Matrix b(3,3,{1,2,3,2,4,5,3,5,1});



    /* fonction IsName */

    assert(isName("identite"));
    assert(!isName("5identite"));
    assert(!isName("545"));
    assert(isName("Identite12"));
    assert(!isName("~Identite+"));

    /* fonctions size,isEmpty,exist,addMatrix,find,erase */

    assert(lib.size()==0);
    assert(lib.isEmpty());

    lib.addMatrix("identite",identite);
    lib.addMatrix("matrice1",a);
    lib.addMatrix("matrice2",b);

    assert(!lib.isEmpty());
    assert(lib.exist("matrice1"));

    lib.erase("matrice2");

    assert(!lib.exist("matrice2"));
    assert(*lib.find("identite") == identite);
    assert(*lib.find("matrice1") == a);
    assert(lib.size()==2);

    /* fonctions isCalculableExpression */


    /* fonctions expressionCalcul */

    lib.addMatrix("matrice2",b);

    Matrix res;
    Matrix resultat1(3,3,{2,0,0,0,2,0,0,0,2});
    Matrix resultat2(3,3,{7,2,3,4,11,6,7,8,15});
    Matrix resultat3(3,3,{45360,55728,66096,102708,126198,149688,160056,196668,233280});
    Matrix resultat4(3,3,{33,-13,2,-13,20,-1,2,-1,12});
    Matrix resultat5(3,3,{615,-379,55,-379,235,-34,55,-34,6});
    Matrix resultat6(3,3,0);


    res = lib.expressionCalcul("identite+identite~");
    assert(res == resultat1);


    res = lib.expressionCalcul("identite+identite~*5+matrice1");
    assert(res == resultat2);


    res = lib.expressionCalcul("3*2*matrice1^4");
    assert(res == resultat3);


    res = lib.expressionCalcul("matrice2~+6*2");
    assert(res == resultat4);



    res = lib.expressionCalcul("1+(matrice2~)^2");
    assert(res == resultat5);

    res = lib.expressionCalcul("2*matrice2*matrice2*(matrice2*matrice2)~-2");
    assert(res == resultat6);


    cout << endl << endl << endl << "****** FIN DU TEST DE REGRESSION ******" << endl << endl ;
}



