#include <iostream>
#include <algorithm>
#include <cmath> 

using namespace std;

class Polynomial; 

class Term {
    friend class Polynomial;
private:
    float coef; 
    int exp;    
};

class Polynomial {
private:
    Term* termArray; 
    int capacity;    
    int terms;       

    void NewTerm(const float theCoeff, const int theExp);

public:
    Polynomial(int cap = 10); 
    ~Polynomial(); 

    Polynomial Add(const Polynomial& poly) const; 
    Polynomial Mult(const Polynomial& poly) const; 
    float Eval(float f) const; 
};

Polynomial::Polynomial(int cap) {
    capacity = cap;
    terms = 0;
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}


void Polynomial::NewTerm(const float theCoeff, const int theExp) {
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        std::copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms++].exp = theExp;
}


Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;
    int thisPos = 0, polyPos = 0;
    while ((thisPos < terms) && (polyPos < poly.terms)) {
        if (termArray[thisPos].exp == poly.termArray[polyPos].exp) {
            float newCoef = termArray[thisPos].coef + poly.termArray[polyPos].coef;
            if (newCoef) result.NewTerm(newCoef, termArray[thisPos].exp);
            thisPos++;
            polyPos++;
        }
        else if (termArray[thisPos].exp < poly.termArray[polyPos].exp) {
            result.NewTerm(poly.termArray[polyPos].coef, poly.termArray[polyPos].exp);
            polyPos++;
        }
        else {
            result.NewTerm(termArray[thisPos].coef, termArray[thisPos].exp);
            thisPos++;
        }
    }
    for (; thisPos < terms; thisPos++)
        result.NewTerm(termArray[thisPos].coef, termArray[thisPos].exp);
    for (; polyPos < poly.terms; polyPos++)
        result.NewTerm(poly.termArray[polyPos].coef, poly.termArray[polyPos].exp);
    return result;
}


Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < poly.terms; j++) {
            float newCoef = termArray[i].coef * poly.termArray[j].coef;
            int newExp = termArray[i].exp + poly.termArray[j].exp;
            result.NewTerm(newCoef, newExp);
        }
    }
    return result;
}

float Polynomial::Eval(float f) const {
    float result = 0.0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * std::pow(f, termArray[i].exp);
    }
    return result;
}


int main() {
    Polynomial p1;
    p1.NewTerm(2, 6);
    p1.NewTerm(1, 4);
    p1.NewTerm(4, 2);
    p1.NewTerm(-3, 1);
    p1.NewTerm(-2, 0);

    Polynomial p2;
    p2.NewTerm(3, 5);
    p2.NewTerm(-1, 3);
    p2.NewTerm(2, 1);
    p2.NewTerm(1, 0);

    Polynomial p3 = p1.Add(p2);
    Polynomial p4 = p1.Mult(p2);

    std::cout << "p1 evaluated at 2: " << p1.Eval(2) << std::endl;
    std::cout << "p3 (p1 + p2) evaluated at 2: " << p3.Eval(2) << std::endl;
    std::cout << "p4 (p1 * p2) evaluated at 2: " << p4.Eval(2) << std::endl;

    return 0;
}

