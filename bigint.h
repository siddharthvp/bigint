#ifndef BIGINT_H
#define BIGINT_H

class bigint
{
    public:
        bigint();
        bigint(char *s);
        bigint(string s);
        bigint(int n);
        bigint negate();
        bigint abs();
        long long toInt();
        bool operator > (const bigint& that);
        bool operator < (const bigint& that);
        bool operator >= (const bigint& that);
        bool operator <= (const bigint& that);
        bool operator == (const bigint& that);
        bool operator != (const bigint& that);
        friend ostream& operator << (ostream&, const bigint&);
        bigint operator + (const bigint& that);
        bigint operator - (bigint const& that);
        bigint operator -- ();   //prefix
        bigint operator ++ (); //prefix
        bigint operator ++ (int);  //postfix
        bigint operator -- (int); //postfix
        bigint operator * (bigint that);
        void operator += (bigint that);
        void operator -= (bigint that);
        void operator *= (bigint that);
        bigint operator + (int that);
        bigint operator - (int that);
        bigint operator * (int that);
        void operator += (int that);
        void operator -= (int that);
        void operator *= (int that);
        bool operator > (int that);
        bool operator < (int that);
        bool operator >= (int that);
        bool operator <= (int that);
        bool operator == (int that);
        bool operator != (int that);

        static bigint factorial(bigint n);
        bigint factorial();

    private:
        vector <int> v;
        bool sign;  //true for +, false for -
        vector <int> zero = {0};
        bigint add_mag(const bigint& a, const bigint& b);
        bigint subtract_mag(const bigint& a, const bigint& b); //assumes: |a| > |b|
        int compare_mag(const bigint& a, const bigint& b);
};

#endif // BIGINT_H
