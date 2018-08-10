#ifndef BIGINT_H
#define BIGINT_H

class bigint
{
    public:

        bigint();
        bigint(char *);
        bigint(string);
        bigint(int);
        bigint(long);
        bigint(long long);
        bigint(unsigned int);
        bigint(unsigned long);
        bigint(unsigned long long);

        bigint& negate();    //changes the sign of the number and returns it
        long long toInteger() const;  //converts to long long type if possible, else throws range_error
        unsigned long long toUnsigned() const;  //ignores the sign and converts if possible, else throws range_error
        string binary() const;    //returns binary form of magnitude of number with a minus sign if number is negative
        int num_digits() const;
        int operator[] (int);   //return digit in given position from left (zero-indexed)

        friend bool operator > (const bigint&, const bigint&);
        friend bool operator < (const bigint&, const bigint&);
        friend bool operator >= (const bigint&, const bigint&);
        friend bool operator <= (const bigint&, const bigint&);
        friend bool operator == (const bigint&, const bigint&);
        friend bool operator != (const bigint&, const bigint&);

        friend bigint operator + (const bigint&, const bigint&);
        friend bigint operator - (const bigint&, const bigint&);
        friend bigint operator * (const bigint&, const bigint&);
        friend bigint operator / (const bigint&, long long);
        friend long long operator % (const bigint&, long long);
        friend bigint operator / (const bigint& a, const bigint& b);   //converts b to long long
        friend long long operator % (const bigint& a, const bigint& b);

        bigint& operator ++ ();     //pre-increment
        bigint& operator -- ();     //pre-decrement
        const bigint operator ++ (int);   //post-increment
        const bigint operator -- (int);   //post-decrement

        bigint& operator += (const bigint&);
        bigint& operator -= (const bigint&);
        bigint& operator *= (const bigint&);
        bigint& operator /= (int);
        bigint& operator %= (int);
        bigint& operator |= (const bigint&);
        bigint& operator &= (const bigint&);
        bigint& operator ^= (const bigint&);
        bigint& operator <<= (int);
        bigint& operator >>= (int);

        friend bigint operator | (const bigint&, const bigint&);
        friend bigint operator & (const bigint&, const bigint&);
        friend bigint operator ^ (const bigint&, const bigint&);
        friend bigint operator << (const bigint&, int);
        friend bigint operator >> (const bigint&, int);
        friend bigint operator ~ (const bigint&);

        friend ostream& operator << (ostream&, const bigint&);  //to enable output using cout<<
        friend int signum(const bigint&);   //returns 1 if number is positive, -1 if negative, 0 if its 0
        friend bigint abs(const bigint&);          //returns absolute value of number without changing the number
        friend string to_string(const bigint&);
        friend bigint factorial(const bigint&);

        bigint flipbit(int);
        bigint setbit(int);
        bigint clearbit(int);
        bool testbit(int);

    private:

        vector <int> v; //vector to store the digits of the number
        bool sign;  //true for +, false for -
        vector <int> zero = {0};    //used in addition, subtraction operations
        bigint add_mag(const bigint&) const;
        bigint subtract_mag(const bigint&) const; //assumes: |*this| > |that|
        int compare_mag(const bigint&) const;
        void twosComplement();     //Converts binary bigint to its 2s complement
        bigint decimal();          //Returns decimal form of binary bigint

};

istream& operator >> (istream&, bigint&);   //to facilitate reading a bigint variable using cin>>

bigint operator - (const bigint&);


#endif // BIGINT_H
