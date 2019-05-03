
#include <iostream>
#include <vector>
#include <math.h>
#include "VectorX.h"
#include "Polynomial.h"

using namespace std;


const double EPSILON=0.0001;
const Polynomial Polynomial:: polynomial_null = Polynomial();
const Polynomial Polynomial:: polynomial_noEigen = Polynomial();



Polynomial :: Polynomial() : tab (vector<double>(1,0))
{
    this->degree = 0;
}


Polynomial :: Polynomial(const unsigned int & d) : tab(vector<double>(d+1,1))
{
    this->degree = d;
}


Polynomial :: Polynomial(const unsigned int & d, const VectorX & values)
{
    if ( values.size() != d + 1 )
    {
        cerr << "Erreur : Pas le nombre de coefficients" << endl;
        return;
    }

    this->degree = d;

    for ( unsigned int i=0; i<d+1; i++ )
    {
        tab.push_back(values[i]);
    }

}


Polynomial::Polynomial(const Polynomial &p) : tab (vector<double>(p.tab))
{
    degree = p.degree;
}


void Polynomial:: check()
{
    for(unsigned int i=0; i<=degree; i++)
    {
        if(abs(tab[i])<EPSILON)
        {
            tab[i]=0;
        }

    }
}


ostream& operator << (ostream& flux, const Polynomial & p)
{
    unsigned int i;
    bool notfirst=false;

    for (i=0; i<=p.degree; i++)
    {
        if(p.tab[i]>0 && notfirst)
            flux << " + " ;
        else if(p.tab[i]<0 && notfirst)
            flux << " " ;

        if (p.tab[i]!=0.0)
        {
            notfirst = true;
            if (p.tab[i]==1.0)
            {
                if (i==0)
                    flux << 1 ;
                else if (i==1)
                    flux << "X";
                    else
                        flux << "X^" << i ;
            }
            else if (-p.tab[i]==1.0)
            {
                if (i==0)
                    flux << -1 ;
                else if (i==1)
                    flux << "- X" ;
                    else
                        flux << "- X^" << i ;
            }
                else
                {
                    if (i==0)
                        flux << p.tab[i] ;
                    else if (i==1)
                        flux << p.tab[i] << "X" ;
                        else
                            flux << p.tab[i] << "X^" << i ;
                }

        }
    }
    cout << endl;
    return flux;
}


Polynomial& Polynomial:: operator = (const Polynomial & p)
{
    this->degree=p.degree;
    tab=vector<double>(this->degree+1,0);
    unsigned int i;
    for (i=0; i<=degree; i++)
    {
        tab[i]=p.tab[i];
    }

    return (*this);
}


bool Polynomial:: operator == (const Polynomial & p) const
{
    if(degree!=p.degree)
        return false;
    unsigned int i;
    for(i = 0; i < degree; i++)
        if (tab[i] - p.tab[i] != 0.0)
            return false;

    return true;
}


const Polynomial Polynomial:: operator + (const Polynomial & p)const
{
    unsigned int i;
    if (degree>p.degree)
    {
        Polynomial resultat(*this);

        for (i=0; i<=p.degree;i++)
        {
            resultat.tab[i]+=p.tab[i];
        }

        return resultat;

    }
    else
    {
        Polynomial resultat(p);

        for (i=0; i<=degree;i++)
        {
            resultat.tab[i]+=tab[i];
        }

        return resultat;
    }

}


const Polynomial Polynomial:: operator - (const Polynomial & p)const
{
    unsigned int i, mindegree;

    Polynomial resultat(*this);

    if(degree>p.degree)
        mindegree=p.degree;
    else
        mindegree=degree;

    for(i=0; i<=mindegree; i++)
    {
        resultat.tab[i]-=p.tab[i];
    }

    if(mindegree==degree)
    {
        for(i=mindegree+1; i<=p.degree; i++)
        {
            resultat.tab.push_back(-p.tab[i]);
            resultat.degree++;
        }
    }

    return resultat;
}


const Polynomial Polynomial:: operator * (const Polynomial & p)const
{
    unsigned int i,j,k,d=degree+p.degree;
    Polynomial result(d);

    for(i=0; i<=d; i++)
        result.tab[i]=0;

    for(i=0; i<=d; i++)
    {
        for(j=0; j<=degree; j++)
        {
            for(k=0; k<=p.degree; k++)
            {
                if(j+k==i)
                {
                    result.tab[i]+=tab[j]*p.tab[k];
                }
            }
        }
    }
    result.check();
    return result;
}


const Polynomial Polynomial:: operator * (const double & scale)const
{
    unsigned int i;
    Polynomial p(*this);
    for (i=0; i<=degree; i++)
    {
        p.tab[i]*=scale;
    }
    p.check();
    return p;
}


const Polynomial Polynomial:: division(const Polynomial & divisor, Polynomial & reste)
{
    Polynomial quotient(degree - divisor.degree);
    Polynomial copy(*this);

    unsigned int i = 0, j, tampon;
    unsigned long k;

    while (copy.degree - i >= divisor.degree)
    {
        quotient.tab[copy.degree - divisor.degree - i] = 
        											copy.tab[degree - i] 
        											/ divisor.tab[divisor.degree];

        for (j = i; j <= divisor.degree; j++)
        {
            copy.tab[copy.degree - j] = copy.tab[copy.degree - j]
            							- divisor.tab[divisor.degree - (j - i)]
            							* quotient.tab[copy.degree - divisor.degree - i];

        }
        i++;
    }

    copy=quotient*divisor;
    reste = (*this) - copy;

    tampon=0;
    k=reste.degree;

    while (reste.tab[k] == 0.0)
    {
        tampon++;
        k--;
    }
    reste.degree=reste.degree-tampon;


    if (reste.degree>divisor.degree)
    {
        Polynomial quotient2 (reste.degree-divisor.degree);
        quotient2=reste.division(divisor,reste);
        quotient=quotient+quotient2;
    }

    return quotient;
}


void Polynomial:: equation2degre (unsigned int  & nbsolution, 
									double & x1, double & x2)
{
    if (degree != 2)
    {
        cout << "Equation de degré différent de 2" << endl;
        exit(EXIT_FAILURE);
    }

    double a,b,c;
    Polynomial copie(*this);
    copie.check();
    a=copie.tab[2];
    b=copie.tab[1];
    c=copie.tab[0];

    double delta=b*b-4*a*c;

    if(delta < 0)
    {
        nbsolution=0;
    }
        else if (delta == 0.0)
        {
            nbsolution=1;
            x1=-b/(2*a);
        }
        else
            {
                nbsolution=2;
                double r=sqrt(delta);
                x1=(-b-r)/(2*a);
                x2=(-b+r)/(2*a);
            }

}


