
#include <iostream>
#include <complex>
#include <cstring>
#include <cmath>
#include <random>
#include <utility>
#include <../../ext/Eigen/Dense>
#include "Matrix.h"
#include "Polynomial.h"
#include "Gauss.h"


using namespace std;

const double EPSILON = 0.00001;


const Matrix Matrix:: matrix_null = Matrix();
const complex<double> Matrix:: complex_null = complex<double>();

// ********* CONSTRUCTORS / DESTRUCTORS *********



Matrix:: Matrix () : tab ( vector<vector<complex<double>>> ())
{
    rows = 0;
    cols = 0;
}


Matrix:: Matrix (const unsigned int rows, const unsigned int cols, const complex<double> value) :
        tab (vector<vector<complex<double>>>(rows,vector<complex<double>> (cols, value)))
{
    this->rows = rows;
    this->cols = cols;
}


Matrix:: Matrix (const unsigned int rows, const unsigned int cols, const enum initMatrix& type) :
        tab (vector<vector<complex<double>>> (rows, vector<complex<double>> (cols, 0)))
{
    this->cols = cols;
    this->rows = rows;

    switch (type)
    {
        case Z: {
            unsigned int i, j;
            std::complex<double> z (0.0,0.0);
            for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                    tab[i][j] = z;
                }
            }
            break;
        }
        case R:
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-10000,10000);
            float random1 = dis(gen);
            float random2 = dis(gen);

            std::complex<double> z;
            for (auto& i : tab)
            {
                for (auto& j : i)
                {
                    random1 = dis(gen);
                    random2 = dis(gen);
                    z = std::complex<double>(random1, random2);
                    j = z;
                }
            }
            break;
        }
        case I:
        {
            if (cols != rows)
            {
                cerr << "Initialisatin d'une matrice identité impossible (rows != cols)" << endl;
                exit(EXIT_FAILURE);
            }

            for (unsigned int i = 0; i < rows; ++i)
            {
                tab[i][i] = std::complex<double>(1,0);
            }
            break;
        }
    }
}


Matrix:: Matrix (const unsigned int rows, const unsigned int cols, const VectorX & values)
{
    if (values.size() != rows * cols)
    {
        cerr << "Erreur : la dimension du vecteur passé en paramètre ne correspond pas aux dimensions de la matrice" << endl;
        exit (EXIT_FAILURE);
    }

    this->cols = cols;
    this->rows = rows;

    vector<complex<double>> temp;

    for ( unsigned int i = 0; i < rows; ++i )
    {
        temp.clear();

        for ( unsigned int j = 0; j < cols; ++j )
        {
            temp.push_back(values[i*cols+j]);
        }
        tab.push_back(temp);
    }
}


Matrix:: Matrix (const Matrix & m) : tab ( vector<vector<complex<double>>> (m.tab))
{
    cols = m.cols;
    rows = m.rows;
}






// ******** STATIC FUNCTION *********


Matrix Matrix:: ID (const unsigned int size)
{
    return Matrix(size, size, Matrix::I);
}





// ******* ACCESSORS / MUTATORS & BASIC FUNCTIONS ********


unsigned int Matrix:: getNbRows() const
{
    return rows;
}


unsigned int Matrix:: getNbCols() const
{
    return cols;
}


void Matrix:: setNbRows(unsigned int row)
{
    this->rows = row;
    tab = (vector<vector<complex<double>>>(rows,vector<complex<double>> (cols,0)));
}


void Matrix:: setNbCols(unsigned int col)
{
    this->cols = col;
    tab = (vector<vector<complex<double>>>(rows,vector<complex<double>> (cols,0)));
}


complex<double>& Matrix:: getVal (const unsigned int indice)
{
    if ( indice >= (rows * cols))
    {
        cerr << "L'indice " << indice <<" n'existe pas dans cette matrice" << endl;
        exit ( EXIT_FAILURE );
    }

    return tab[indice/rows][indice%rows];
}


complex<double> Matrix:: getVal (const unsigned int indice) const
{
    if ( indice >= (rows * cols))
    {
        cerr << "L'indice " << indice <<" n'existe pas dans cette matrice" << endl;
        return complex_null;
    }

    return tab[indice/rows][indice%rows];
}


vector<complex<double>>&  Matrix:: operator [] (const unsigned int indice)
{
    if ( indice >= rows)
    {
        cerr << "L'indice " << indice <<" n'existe pas dans cette matrice" << endl;
        exit ( EXIT_FAILURE );
    }
    return tab[indice];
}


const std::vector<complex<double>>& Matrix:: operator [] (const unsigned int indice) const
{
    if ( indice >= rows)
    {
        cerr << "L'indice " << indice <<" n'existe pas dans cette matrice" << endl;
        exit ( EXIT_FAILURE );
    }
    return tab[indice];
}


ostream& operator << (ostream& flux, const Matrix & m)
{
    for (auto i : m.tab)
    {
        for (auto j : i)
        {
            flux << j << "  ";
        }
        flux << endl;
    }
    return flux;
}


Matrix Matrix:: operator << (const string& values)
{
    unsigned int i,j;
    string c;
    vector<string> table;
    table = explode(values);

    if (table.size() != rows*cols)
    {
        cerr <<"Le nombre des valeurs rentrées ne correspond pas à la taille de la matrice" << endl;
        return matrix_null;
    }

    else
    {  for (i = 0; i < this->rows; i++)
        {
            for ( j = 0; j < this->cols; j++)
            {
                c = table[i*cols+j];
                tab[i][j] = atof(c.c_str());
            }
        }
     }
    return *this;
}


istream& operator >> (istream& flux, Matrix & m)
{
    unsigned int i,j,rows,cols;
    double value;
    flux >> rows >> cols;    //Entering rows and columns numbers
    m.setNbRows(rows);
    m.setNbCols(cols);

    for ( i = 0; i < rows; ++i )
    {
        for ( j = 0; j < cols; ++j )
        {
            flux >> value;
            m.tab[i][j] = value;      // Entering des coefficients
        }
    }

    return flux;
}


void Matrix:: setMatrixKB ()
{
    cout << "Saisir nblignes : ";
    cin >> rows;
    cout << "Saisir nbColonnes : ";
    cin >> cols;

    tab.resize(rows, vector<complex<double>>(cols));

    int re;
    int im;

    for(unsigned int i = 1; i <= rows; ++i)
    {
        for(unsigned int j = 1; j <= cols; ++j)
        {
            re = 0;
            im = 0;
            cout << endl << "Saisir la partie réelle du coeff " << "(" << i << " , " << j << ")" << ": ";
            cin >> re;
            cout << endl << "Saisir la partie imaginaire du coeff " << "(" << i << " , " << j << ")" << ": ";
            cin >> im;
            tab[i-1][j-1] = complex<double>(re,im);
        }
    }
}


void Matrix:: setMatrixRA ()
{
    cout << "Saisir nblignes : ";
    cin >> rows;
    cout << "Saisir nbColonnes : ";
    cin >> cols;

    tab = Matrix(rows, cols, Matrix::R).tab;
}


const Matrix Matrix:: operator + (const Matrix & m) const
{
    if ( (rows!=m.rows) || (cols!=m.cols) )
    {
        cerr << "Addition impossible!" << endl;
        return matrix_null;
    }

    unsigned int i,j;
    Matrix copy(*this);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            copy[i][j] = copy[i][j] + m[i][j];
        }
    }
    return copy;
}


const Matrix Matrix:: operator - (const Matrix & m) const
{
    if ((rows!=m.rows) || (cols!=m.cols))
    {
        cerr << "Soustraction impossible!" << endl;
        return matrix_null;
    }

    unsigned int i,j;
    Matrix copy(*this);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            copy[i][j] = copy[i][j] - m[i][j];
        }
    }
    return copy;
}


const Matrix Matrix:: operator * (const Matrix & m) const
{
    if (cols != m.rows)
    {
        cerr << "Multiplication impossible!" << endl <<
                "A * B -> Le nombre de ligne de A doit  nombre de colonne de B!" << endl;
        return matrix_null;
    }

    unsigned int i,j,k;
    complex<double> sum;
    Matrix res(rows, m.cols, Matrix::Z);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < m.cols; j++)
        {
            sum = 0;
            for (k = 0; k < cols; k++)
            {
                sum += tab[i][k] * (m[k][j]);
            }
            res[i][j] = sum;
        }
    }
    return res;
}


const Matrix Matrix:: operator * (const std::complex<double> & lambda) const
{
    unsigned int i,j;
    Matrix res(*this);

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            res[i][j] *= lambda;
        }
    }
    return res;
}


const Matrix operator * (const double & lambda, const Matrix & m)    // friend operator
{
    return m * lambda;
}


const Matrix Matrix:: operator / (const Matrix & m) const
{
    if (!m.isSQMatrix())
    {
        cerr << "Division est impossible, la matrice diviseure n'est pas une matrice carrée !" << endl;
        return matrix_null;
    }
    if (m.determinant() == 0.0)
    {
        cerr << "Division impossible, la matrice diviseure n'est pas inversible !" << endl;
        return matrix_null;
    }
    Matrix result ((*this) * m.inverse());
    return result ;
}


const Matrix Matrix:: operator ^ (const int & p) const
{

    if ( !isSQMatrix() )
    {
        cerr << "Erreur, la matrice n'est pas carrée ! " << endl;
        return matrix_null;
    }

    if ( p == 0 )
    {
        return Matrix (rows, I);
    }

    if ( p == -1 )
    {
        return inverse();
    }

    Matrix temp (*this);
    Matrix temp2 (*this);

    if ( p < -1 )
    {
        temp = temp.inverse();
        temp2 = temp2.inverse();
    }

    for (int i = 1; i < abs(p); ++i )
    {
        temp = (temp * temp2);
    }
    return temp;
}


bool Matrix:: operator == (const Matrix & m ) const
{
    unsigned int i,j ;

    if ( (m.cols == cols) && (m.rows == rows) )
    {
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < cols; j++)
            {
                if ( (abs(tab[i][j].real() - m.tab[i][j].real()) > EPSILON)
                    || (abs(tab[i][j].imag() - m.tab[i][j].imag()) > EPSILON)) //!\\ MODIFICATION A DISCUTER
                {
                    return false;
                }
            }
        }
        return true;
    }

    return false;
}


bool Matrix::operator != (const Matrix & m) const
{
    return !((*this) == m);
}


bool Matrix:: isSQMatrix() const
{
    return rows == cols;
}


complex<double> Matrix:: traceMatrix() const
{
    if ( !isSQMatrix() )
    {
        cerr << "Calcul de la trace impossible, la matrice n'est pas carrée" << endl;
        return complex_null;
    }

    complex<double> s(0.0,0.0);
    for ( unsigned int i = 0; i < rows; i++)
    {
        s += tab[i][i];
    }
    return s;
}


complex<double> Matrix:: determinant() const
{
    if (!isSQMatrix())
    {
        cerr << "Calcul du déterminant impossible, la matrice n'est pas carrée" << endl;
        return complex_null;
    }

    Eigen:: MatrixXcd m = class2Eigen();
    return m.determinant();
}


Matrix Matrix::coMatrix() const
{
    if ( !isSQMatrix() )
    {
        cerr << "Calcul de la comatrice impossible, la matrice n'est pas carrée" << endl;
        return matrix_null;
    }

    unsigned int i, j;
    Matrix com(rows,cols);
    Matrix sub(rows - 1, cols - 1);

    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
        {
            sub = subMatrix(i,j);
            com[i][j] = pow(-1,i + j) * sub.determinant();
        }
    }
    return com;
}


Matrix Matrix:: transposeMatrix() const
{
    unsigned int i, j;
    Matrix copy(cols,rows);

    for (i = 0; i < copy.rows; i++)
    {
        for (j = 0; j < copy.cols; j++)
        {
            copy[i][j] = tab[j][i];
        }
    }
    return copy;
}


Matrix Matrix:: inverse() const
{
    if (!isSQMatrix())
    {
        cerr << "Calcul de l'inverse impossible, la matrice n'est pas carrée" << endl;
        return matrix_null;
    }

    if(determinant() == 0.0)
    {
        cerr << "Calcul de l'inverse impossible, la matrice n'est pas inversible" << endl;
        return matrix_null;
    }

    Eigen::MatrixXcd m = class2Eigen(), inverse;

    return eigen2Class(m.inverse());
}


unsigned int Matrix:: rank()const
{
    unsigned int i, j, r, c, rg = 0;
    bool non_zero = false;
    r = getNbRows();
    c = getNbCols();
    Matrix reduction(*this);
    reduction = reduction.gaussReduction();

    Matrix id (r,I);
    if (id == *this)
      return r;

    for (i = 0; i < r; i++)
    {
        non_zero = false;
        for (j = 0; j < c; j++)
        {
            if (reduction[i][j]!=0.0)
            {
                non_zero = true;    // rank is the number of non zero lines after gaussReduction
            }
        }
        if (non_zero)
        {
            rg++;
        }
    }

    return rg;
}





// ********   PRIVATE FUNCTIONS   ***********

complex<double> Matrix::checkCast(const complex<double> & c) const
{
    int l;
    double re, im;

    l = -150;
    re = c.real();
    while (l < 151)
    {
        if ( abs(c.real() - l) < EPSILON )
            {
                re = l;
                break;
            }
        l++;
    }

    l = -150;
    im = c.imag();
    while (l < 151)
    {
        if ( abs(c.imag() - l) < EPSILON )
            {
                im = l;
                break;
            }
        l++;
    }


    return (complex<double>(re,im));

}


VectorX Matrix:: checkCast(const VectorX & v) const
{
    VectorX res;
    for (unsigned int i = 0; i < v.size(); i++)
    {
        res.push_back(checkCast(v[i]));
    }
    return res;
}



Matrix Matrix:: checkCast() const
{
    unsigned int r = getNbRows();
    Matrix result(*this);

    for (unsigned int i = 0; i < r; i++)
    {
        result[i] = checkCast(tab[i]);
    }

    return result;
}


Matrix Matrix :: subMatrix(const unsigned int & a, const unsigned int & b) const
{
    unsigned int i, j;
    unsigned int ii = 0, jj = 0;
    unsigned int r = rows;
    unsigned int c = cols;

    Matrix sub(r-1,c-1);

    for(i = 0; i < r; i++)
    {
        jj = 0;
        if(i != a)
        {
            for (j = 0; j < c; j++)
            {
                if (j != b)
                {
                    sub[ii][jj] = tab[i][j];
                    jj++;
                }
            }
            ii++;
        }
    }
    return sub;
}


vector<string> Matrix:: explode (const string & expression) const
{
    unsigned int i;
    unsigned  long l = expression.length();
    vector<string> tab;
    string c, temp;
    temp="";
    for (i = 0; i < l; i++)
    {
        c = expression[i];

        if (c == ",")
        {
            if (temp.length()!=0) tab.push_back(temp);
            temp="";
        }
        else if (!c.empty())
        {
            temp += c;
        }

    }
    if (temp != "")
    tab.push_back(temp);

    return tab;
}


VectorX Matrix:: normaliseVectorX (const VectorX & v) const
{
    VectorX res;
    unsigned int i;
    double norme = 0;

    for (i = 0; i < rows; i++)
    {
        norme += ( (v[i].real() * v[i].real()) + (v[i].imag() * v[i].imag()) );
    }

    for (i = 0; i < rows; i++)
    {
        res.push_back(v[i] / sqrt(norme));
    }

    return res;
}


Eigen::MatrixXcd Matrix:: class2Eigen () const
{
    unsigned int i, j, r, c;
    r = getNbRows();
    c = getNbCols();
    Eigen:: MatrixXcd m(r,c);

    for(i = 0 ; i < r ; i++)
    {
        for(j = 0 ; j < c ; j++)
        {
            m(i,j) = tab[i][j] ;
        }
    }

    return m;
}


const Matrix Matrix:: eigen2Class(const Eigen::MatrixXcd & m) const
{
    unsigned int i, j, r, c ;
    r = static_cast<unsigned int>(m.rows());
    c = static_cast<unsigned int>(m.cols());
    Matrix a(r,c);

    for(i = 0 ; i < r ; i++)
    {
        for(j = 0 ; j < c ; j++)
        {
            a.tab[i][j] = m(i,j) ;
        }
    }

    return a;
}







// *********   ADVANCED MATRIX STUDY FUNCTIONS   *********


const Matrix Matrix:: gaussReduction()const
{
    Gauss g;
    Matrix res(*this);

    int col;
    int r = static_cast<int>(res.getNbRows()) ;
    int c = static_cast<int>(res.getNbCols()) ;


    int nonzero_row_id, next_row_id = 0;
    vector<Gauss> pivot_gauss;

    for (col = 0; col < c; col++)
    {
        nonzero_row_id = g.isNonZeroColumn(res, col, r, next_row_id);
        if (nonzero_row_id >= 0)
        {
            if (nonzero_row_id != next_row_id)
            {
                g.rowExchange(res.tab.begin() + next_row_id, res.tab.begin() + nonzero_row_id);
                nonzero_row_id = next_row_id;
            }
            pivot_gauss.push_back(Gauss(nonzero_row_id, col));
            for (int row = next_row_id; row < r; row++)
            {
                if (res[static_cast<unsigned int>(row)][static_cast<unsigned int>(col)] == 0.0)
                    continue;
                if (row == nonzero_row_id)
                    continue;
                g.rowReplace(res.tab.begin() + row,
                            res.tab.begin() + nonzero_row_id,
                            - res.tab[static_cast<unsigned int>(row)]
                                     [static_cast<unsigned int>(col)]
                            / res.tab[static_cast<unsigned int>(nonzero_row_id)]
                                     [static_cast<unsigned int>(col)]);
            }
            next_row_id++;
        }
        else
        {
            continue;
        }

    }

    for (vector<Gauss>::iterator pos = pivot_gauss.end() - 1;
         pos >= pivot_gauss.begin(); pos--)
    {
        if (pos->getVal(res).real() != 1.0)
        {
            complex<double> inverse (1./res.tab[static_cast<unsigned int>(pos->getRow())]
                        [static_cast<unsigned int>(pos->getCol())]);

            res.tab[pos->getRow()] = res.normaliseVectorX(res.tab[pos->getRow()]);
        }
        for (int row = 0; row < r; row++)
        {
            if (res.tab[static_cast<unsigned int>(row)]
                        [static_cast<unsigned int>(pos->getCol())].real() != 0.0
                    && res.tab[static_cast<unsigned int>(row)]
                                [static_cast<unsigned int>(pos->getCol())].imag() != 0.0
                    && row != pos->getRow())
            {
                g.rowReplace(res.tab.begin() + row, res.tab.begin() + pos->getRow(),
                            -res.tab[static_cast<unsigned int>(row)][static_cast<unsigned int>(pos->getCol())]
                        / res.tab[static_cast<unsigned int>(pos->getRow())][static_cast<unsigned int>(pos->getCol())]);
            }
        }

    }

    return res;
}



const pair<unsigned int, unsigned int> Matrix:: dimensionsStudy()const
{
    unsigned int dim_E = getNbCols();   // number of cols = dimension of start set of linear application
    unsigned int dim_im = rank();
    unsigned int dim_ker = dim_E - dim_im;   // using rank theorem

    return make_pair(dim_im,dim_ker);
}


const VectorX Matrix:: eigenValues() const
{
    unsigned long int i, n;
    vector<complex<double>> result;
    Eigen::MatrixXcd a = class2Eigen();
    Eigen::ComplexEigenSolver<Eigen::MatrixXcd> m(a,false);
    n = static_cast<unsigned long int>(m.eigenvalues().size());

    for (i = 0; i < n; i++)
    {
        result.push_back(m.eigenvalues()(i));
    }

    return result;
}


const Polynomial Matrix:: characteristicPolynomial()const
{
    unsigned int i,r;
    r = getNbRows();
    Polynomial result(r);
    Polynomial temp(1);

    vector<complex<double>> eigen_values;
    eigen_values = eigenValues();

    result.tab[0] = eigen_values[0];
    result.tab[1] = -1;

    for(i = 2; i < r + 1; i++)
    {
        result.tab[i] = 0;
    }

    for(i = 1; i < r; i++)
    {
        temp.tab[0] = eigen_values[i];
        temp.tab[1] = -1;
        result = result * temp;
    }

    return result;
}



const vector<Polynomial> Matrix:: splitCharacteristicPolynomial()const
{
    unsigned int i,r;
    r = getNbRows();
    Polynomial result(r);
    Polynomial temp(1);
    std::vector<Polynomial> v;
    vector<complex<double>> eigen_values;
    eigen_values = eigenValues();

    for(i = 0; i < r; i++)
    {
        temp.tab[0] = eigen_values[i];
        temp.tab[1] = -1;
        v.push_back(temp);
    }

    return v;
}


const Matrix Matrix:: eigenVectors()const
{
    unsigned int i, j;
    Matrix res(getNbRows(),getNbCols());
    vector<VectorX> tab;
    Eigen:: MatrixXcd m = class2Eigen();
    Eigen:: ComplexEigenSolver<Eigen::MatrixXcd> ces(m);

    for (i = 0; i < getNbRows(); i++)
    {
        for (j = 0; j < getNbCols(); j++)
        {
            res[i][j] = ces.eigenvectors()(i,j);
        }
    }

    return res;

}


const vector<pair<complex<double>,VectorX>> Matrix:: allEigen()const
{
    unsigned int i;
    unsigned long int n;
    Matrix e_vector;
    VectorX e_value;
    vector<pair<complex<double>,VectorX>> result;

    e_value = eigenValues();
    e_vector = eigenVectors();

    n = e_value.size();

    for (i = 0; i < n; i++)
    {
        result.push_back(make_pair(e_value[i],e_vector[i])) ;

    }
    return result;
}


bool isComplex(VectorX v)
{
  unsigned int i;
  for (i = 0; i < v.size(); i++)
  {
    if (abs(v[i].imag()) > EPSILON)
      return true;
  }
  return false;
}


bool Matrix::isDiagonalisableR() const
{
  if (!isDiagonalisableC())
      return false;

  Matrix d (diagonalise());
  for (unsigned int i = 0; i < rows; i++)
  {
      if (isComplex(d[i]))
          return false;
  }
  return true;
}


bool Matrix:: isDiagonalisableC()const
{
    if (!isSQMatrix())
        return false;

    Matrix copy(*this);
    Matrix p1(*this);
    Matrix p2(*this);

    allMatrix(p1,copy,p2);
    if (p2 == matrix_null)
    {
        return false;
    }

    unsigned int i, j, r, c;
    r = copy.getNbRows();
    c = copy.getNbCols();
    for(i = 0; i < r; i++)
    {
        for(j = 0; j < c; j++)
        {
            if (i != j && copy[i][j] != 0.0)
                return false;
        }
    }

    return true;
}


const Matrix Matrix:: diagonalise()const
{
    Matrix res(getNbRows(),getNbCols());
    VectorX v = eigenValues();
    Eigen:: MatrixXcd a,b;

    for (unsigned int i = 0; i < getNbRows(); i++)
    {
        res[i][i] = v[i];
    }

    return res;

}


const Matrix Matrix::transferMatrix()const
{
    if (!isSQMatrix())
        return matrix_null;

    return eigenVectors();

}


void Matrix:: allMatrix (Matrix & transferC2B, Matrix & diagonal,
                         Matrix & transferB2C) const
{
   transferC2B = transferMatrix();
   diagonal = diagonalise();
   if (transferC2B.determinant() == 0.0)
   {
       transferB2C = matrix_null;
   }
   else
   {
       transferB2C = (transferC2B.inverse());
   }

}


Matrix Matrix::matrixCol(const unsigned int & j) const {
    unsigned int i;
    Matrix vect(rows,1);

    for (i = 0; i < rows; i++) {
        vect[i][0] = tab[i][j];
    } 

    return vect;
}


Matrix Matrix::matrixRow(const unsigned int & i) const {
    unsigned int j;
    Matrix vect(1,cols);

    for (j = 0; j < cols; j++) {
        vect[i][0] = tab[i][j];
    } 

    return vect;
}


Matrix Matrix::normaliseMatrix() const {
    unsigned int i, j;
    Matrix res(*this);
    std::complex<double> norme;

    for (j = 0; j < cols; j++) {
        norme = std::complex<double> (0.0,0.0);
        for (i = 0; i < rows; i++) {
            norme += (res[i][j] * res[i][j]);
        }
        norme = sqrt(norme);
        if (norme != std::complex<double>(0.0,0.0)) {
            for (i = 0; i < rows; i++) {
                res[i][j] /= norme;
            }
        }
        
    }
    return res;
}


std::pair<Matrix,Matrix> Matrix::LUDecomposition() const {
    
    unsigned int n = rows;

    if (!isSQMatrix()) {
        std::cerr << "La décomposition LU ne s'effectue pour que pour des matrices carrées"
                    << std::endl;
        return std::pair<Matrix,Matrix>(matrix_null,matrix_null);
    }
    if (determinant() == 0.0) {
        std::cerr << "La décomposition LU ne s'effectue que pour des matrices inversibles"
                    << std::endl;
        return std::pair<Matrix,Matrix>(matrix_null,matrix_null);
    }

    Matrix l(n,n,Matrix::I);
    Matrix u(n,n,Matrix::Z);
    Matrix t(*this);

    unsigned int i, j, k;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n ; j++) {
            l[j][i] = t[j][i] / t[i][i];
        }
        for (j = i; j < n; j++) {
            u[i][j] = t[i][j];
        }
        for (j = i + 1; j < n; j++) {
            for (k = i + 1; k < n; k++) {
                t[j][k] = t[j][k] - l[j][i] * u[i][k];
            }
        }
    }
    u[n - 1][n - 1] = t[n - 1][n - 1];
    
    return std::pair<Matrix,Matrix>(l,u);
}


Matrix Matrix::solveAx_LU(const Matrix & b) const {

    if (!isSQMatrix() 
        || determinant() == std::complex<double>(0.0,0.0)) {
        std::cout << "Compliqué..." << std::endl;
        return matrix_null;
    }

    unsigned int i, j, n = rows;
    Matrix l = LUDecomposition().first;
    Matrix u = LUDecomposition().second;
    Matrix x(n,1,Matrix::Z);
    Matrix y(n,1,Matrix::Z);

    y[0][0] = b[0][0];

    for (i = 1; i < n; i++) {
        y[i][0] = b[i][0];
        for (j = 0; j < i; j++) {
            y[i][0] -= (l[i][j] * y[j][0]);
        }
    }

    x[n - 1][0] = y[n - 1][0] / u[n - 1][n - 1];

    for(i = n - 2; i < n; i--) {
        x[i][0] = y[i][0];
        for(j = i + 1; j < n; j++) {
            x[i][0] -= (u[i][j] * x[j][0]);
        }
        if (u[i][i] == 0.0)
            x[i][0] = 0.0;
        else {
            x[i][0] /= u[i][i];
        }
    }

    return x;

}


Matrix Matrix::gramSchmidt() const {

    Matrix a(*this);
    Matrix q(rows,cols,Matrix::Z);
    
    Matrix temp_matrix (1,rows,Matrix::Z);
    unsigned int i, j, k;
    std::complex<double> coef, temp_coef;
    VectorX temp_vect;

    for(i = 0; i < rows; i++) {
        q[i][0] = a[i][0];
    }

    q = q.normaliseMatrix(); 

    for (i = 1; i < cols; i++) {
        coef = std::complex<double>(0.0, 0.0);
        temp_matrix = matrixCol(i);
        for (j = 0; j < i; j++) {
            coef = (q.matrixCol(j).transposeMatrix() * a.matrixCol(i))[0][0];
            coef /= (q.matrixCol(j).transposeMatrix() * q.matrixCol(j))[0][0];
            temp_matrix = temp_matrix - (q.matrixCol(j) * coef);
        }
        temp_matrix = temp_matrix.normaliseMatrix();

        for(k = 0; k < rows; k++) {
            q[k][i] = temp_matrix[k][0];
        }
    }

    return q;
}


std::pair<Matrix,Matrix> Matrix::QR_GramSchmidt() const {

    Matrix a(*this);
    Matrix q(rows,cols,Matrix::Z);
    Matrix r(cols,cols,Matrix::Z);
    
    Matrix temp_matrix (1,rows,Matrix::Z);
    unsigned int i, j, k;
    std::complex<double> coef, temp_coef;
    VectorX temp_vect;

    for(i = 0; i < rows; i++) {
        q[i][0] = a[i][0];
    }
    q = q.normaliseMatrix(); 
    r[0][0] = (a.matrixCol(0).transposeMatrix() * q.matrixCol(0))[0][0];

    for (i = 1; i < cols; i++) {
        coef = std::complex<double>(0.0, 0.0);
        temp_matrix = matrixCol(i);
        for (j = 0; j < i; j++) {
            coef = (q.matrixCol(j).transposeMatrix() * a.matrixCol(i))[0][0];
            r[j][i] = coef;
            coef /= (q.matrixCol(j).transposeMatrix() * q.matrixCol(j))[0][0];
            temp_matrix = temp_matrix - (q.matrixCol(j) * coef);
        }
        temp_matrix = temp_matrix.normaliseMatrix();

        for(k = 0; k < rows; k++) {
            q[k][i] = temp_matrix[k][0];
        }

        r[i][i] = (a.matrixCol(i).transposeMatrix() * q.matrixCol(i))[0][0];
    }

    return (std::pair<Matrix,Matrix> (q,r));
}


std::pair<Matrix,Matrix> Matrix::QR_Householder() const {

    unsigned int n = rows;
    unsigned int m = cols;
    Matrix h(m,m,Matrix::I);
    Matrix a(*this);
    VectorX v(n);
    unsigned int i, j, k;
    std::complex<double> alpha (0.0,0.0);
    std::complex<double> beta (0.0,0.0);
    std::complex<double> c (0.0,0.0);

    for (k = 0; k < n - 1; k++) { 
        alpha = 0.0;
        beta = 0.0;
        c = 0.0;

        for (i = k; i < m; i++) {
            alpha = alpha + (a[i][k] * a[i][k]);
        }
        
        alpha = sqrt(alpha);

        beta = alpha * (alpha - a[k][k]);
        v[k] = a[k][k] - alpha;
        for (i = k + 1; i < m; i++) {    // vecteur v
            v[i] = a[i][k];
        }
        
    
        for (j = k; j < n; j++) {      // A^k+1
            for (i = k; i < m; i++) {
                c += (v[i] * a[i][j] / beta);
            }
            for (i = k; i < m; i++) {
                a[i][j] -= (c * v[i]);
            }
        }
        
        for (j = 0; j < m; j++) {    // H = Hk ... H1
            for (i = k; i < m; i++) {
                c = (v[i] * h[i][j] / beta);
            }
            for (i = k; i < m; i++) {
                h[i][j] -= (c * v[i]);
            }
        }
    }

    return std::pair<Matrix,Matrix> (h.transposeMatrix(),a);

}
