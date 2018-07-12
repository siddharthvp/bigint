#ifndef BIGINT_H
#define BIGINT_H

class bigint
{
    public:

        bigint();
        bigint(char *);
        bigint(string);
        bigint(int);

        bigint negate();    //changes the sign of the number and returns it
        long long toInt();  //converts to long long type if possible, else throws range_error

        bool operator > (const bigint&);
        bool operator < (const bigint&);
        bool operator >= (const bigint&);
        bool operator <= (const bigint&);
        bool operator == (const bigint&);
        bool operator != (const bigint&);

        bigint operator + (const bigint&);
        bigint operator - (const bigint&);
        bigint operator * (const bigint&);

        bigint operator ++ (); //prefix
        bigint operator -- ();   //prefix
        bigint operator ++ (int);  //postfix
        bigint operator -- (int); //postfix

        void operator += (bigint);
        void operator -= (bigint);
        void operator *= (bigint);

        friend ostream& operator << (ostream&, const bigint&);  //to enable output using cout<<
        friend int signum(const bigint&);   //returns 1 if number is positive, -1 if negative, 0 if its 0
        friend bigint abs(bigint);          //returns absolute value of number without changing the number

    private:
        vector <int> v; //vector to store the digits of the number
        bool sign;  //true for +, false for -
        vector <int> zero = {0};    //used in addition, subtraction operations
        bigint add_mag(const bigint&, const bigint&);
        bigint subtract_mag(const bigint&, const bigint&); //assumes: |a| > |b|
        int compare_mag(const bigint&, const bigint&);
};

istream& operator >> (istream&, bigint&);   //to facilitate reading a bigint variable using cin>>

bool operator < (int, const bigint&);
bool operator > (int, const bigint&);
bool operator <= (int, const bigint&);
bool operator >= (int, const bigint&);
bool operator == (int, const bigint&);
bool operator != (int, const bigint&);

bigint operator + (int n, const bigint&);
bigint operator - (int n, const bigint&);
bigint operator - (const bigint&);
bigint operator * (int n, const bigint&);

bigint factorial(bigint);
string to_string(bigint);

#endif // BIGINT_H
