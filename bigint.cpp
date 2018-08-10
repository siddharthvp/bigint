#include<iostream>
#include<vector>
#include<climits>
using namespace std;

#include "bigint.h"

bigint bigint::add_mag(const bigint& that) const
{
    if(v.empty() || that.v.empty())
        throw invalid_argument("operation on uninitialised bigints");
    bigint res;
    int carry=0;
    auto p = v.end()-1;
    auto q = that.v.end()-1;
    while(1)
    {
        int sum_digits = *p + *q + carry;
        if(sum_digits <= 9)
        {
            res.v.insert(res.v.begin(),sum_digits);
            carry=0;
        }
        else
        {
            carry = sum_digits/10;
            res.v.insert(res.v.begin(),sum_digits%10);
        }
        if( (p == v.begin() || p==zero.begin()) && \
                (q == that.v.begin() || q==zero.begin()) )
        {
            if(carry)
                res.v.insert(res.v.begin(),carry);
            break;
        }
        if(p == v.begin() || p==zero.begin())
            p = zero.begin();
        else
            p--;
        if(q == that.v.begin() || q==zero.begin() )
            q = zero.begin();
        else
            q--;
    }
    res.sign = true;
    return res;
}

bigint bigint::subtract_mag(const bigint& that) const //assumes: |*this| > |that|
{
    if(v.empty() || that.v.empty())
        throw invalid_argument("operation on uninitialised bigints");
    bigint res;
    auto p = v.end()-1;
    auto q = that.v.end()-1;
    int carry=0;
    while(1)
    {
        int sub_digits = carry + *p - *q;
        if(sub_digits >= 0)
        {
            res.v.insert(res.v.begin(),sub_digits);
            carry=0;
        }
        else
        {
            res.v.insert(res.v.begin(),sub_digits+10);
            carry = -1;
        }
        if(q==that.v.begin() || q==zero.begin())
            q=zero.begin();
        else
            q--;
        if(p==v.begin())
            break;
        else
            p--;
    }
    res.sign=true;
    while(res.v[0]==0)
    {
        res.v.erase(res.v.begin());
        if(res.v.empty())
        {
            res.v.insert(res.v.begin(),0);
            break;
        }
    }
    return res;
}

int bigint::compare_mag(const bigint& that) const
{
    if(v.empty() || that.v.empty())
        throw invalid_argument("comparison on uninitialised bigints");
    if(v.size() > that.v.size()) return 1;
    else if (v.size() < that.v.size()) return -1;
    auto i=v.begin(); auto j=that.v.begin();
    for(;  ; i++,j++)
    {
        if(*i > *j) return 1;
        if(*i < *j) return -1;
        if(i==v.end())
            return 0; //equality case
    }
}

bigint::bigint() : sign(true) {}
bigint::bigint(const char *s)
{
    if(s[0]=='-')
    {
        sign=false;
        s++;
    }
    else
        sign=true;
    for ( ; *s != '\0'; s++)
    {
        if(!isdigit(*s))
            throw invalid_argument("non-numeric initialiser for bigint");
        v.push_back(*s-'0');
    }
}
bigint::bigint(string s)
{
    if(s[0]=='-')
    {
        sign=false;
        for (int i=1; i<s.size(); i++)
        {
            if(!isdigit(s[i]))
                throw invalid_argument(("non-numeric initialiser " + s +" for bigint").c_str());
            v.push_back(s[i]-'0');
        }
    }
    else
    {
        sign=true;
        for (char i:s)
        {
            if(!isdigit(i))
                throw invalid_argument(("non-numeric initialiser " + s +" for bigint").c_str());
            v.push_back(i-'0');
        }
    }

}
bigint::bigint(long long n)
{
    if(n>=0)
        sign=true;
    else
        { sign=false; n=-n; }
    if(n==0) v.insert(v.begin(),0);
    while(n)
    {
        int digit = n%10;
        v.insert(v.begin(),digit);
        n /= 10;
    }
}
bigint::bigint(unsigned long long n)
{
    sign=true;
    if(n==0) v.insert(v.begin(),0);
    while(n)
    {
        int digit = n%10;
        v.insert(v.begin(),digit);
        n /= 10;
    }
}
bigint::bigint(long n) : bigint((long long)n) {}
bigint::bigint(int n) : bigint((long long)n) {}
bigint::bigint(unsigned int n) : bigint((unsigned long long)n) {}
bigint::bigint(unsigned long n) : bigint((unsigned long long)n) {}

bigint& bigint::negate()
{
    if(sign == true) sign = false;
    else sign = true;
    return *this;
}
long long bigint::toInteger() const
{
    if(compare_mag(LLONG_MAX)==1)
        throw range_error((to_string(*this) + " out of bounds of long long int"));
    long long n=0;
    for (int i : v)
    {
        n *= 10;
        n += i;
    }
    if(sign==false)
        n=-n;
    return n;
}
unsigned long long bigint::toUnsigned() const
{
    if(compare_mag(ULLONG_MAX)==1)
        throw range_error((to_string(*this) + " out of bounds of unsigned long long int"));
    unsigned long long n=0;
    for (int i : v)
    {
        n *= 10;
        n += i;
    }
    return n;
}


bool operator > (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(!b.sign) return true;
        else return a.compare_mag(b)==1;
    }
    else
    {
        if(b.sign) return false;
        return (a.compare_mag(b)!=1);
    }
}
bool operator < (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(b.sign) return a.compare_mag(b)==-1;
        else return false;
    }
    else
    {
        if(b.sign) return true;
        else return a.compare_mag(b)==1;
    }
}
bool operator >= (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(b.sign) return (a.compare_mag(b) != -1);
        else return false;
    }
    else
    {
        if(b.sign) return false;
        else return a.compare_mag(b)!=1;
    }
}
bool operator <= (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(b.sign) return a.compare_mag(b) != 1;
        else return false;
    }
    else
    {
        if(b.sign) return true;
        else return a.compare_mag(b) != -1;
    }
}
bool operator == (const bigint& a, const bigint& b)
{
    if(a.sign != b.sign)
        return false;
    return (a.compare_mag(b)==0);
}
bool operator != (const bigint& a, const bigint& b)
{
    if(a.sign != b.sign)
        return true;
    return (a.compare_mag(b)!=0);
}


bigint operator + (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(b.sign)
            return a.add_mag(b);
        else
        {
            if(a.compare_mag(b)>=0)
                return a.subtract_mag(b);
            else
                return b.subtract_mag(a).negate();
        }
    }
    else
    {
        if(b.sign)
        {
            if(a.compare_mag(b)>=0)
                return a.subtract_mag(b).negate();
            else
                return b.subtract_mag(a);
        }
        else
            return a.add_mag(b).negate();
    }
}

bigint operator - (const bigint& a, const bigint& b)
{
    if(a.sign)
    {
        if(b.sign)
        {
            if (a.compare_mag(b) >= 0)
                return a.subtract_mag(b);
            else
                return b.subtract_mag(a).negate();
        }
        else
            return a.add_mag(b);
    }
    else
    {
        if(b.sign)
            return a.add_mag(b).negate();
        else
        {
            if (a.compare_mag(b) >= 0)
                return a.subtract_mag(b).negate();
            else
                return b.subtract_mag(a);
        }
    }
}

bigint& bigint::operator ++ ()  //pre-increment
{
    return *this = *this+(bigint)1;
}
bigint& bigint::operator -- ()   //pre-decrement
{
    return *this = *this-(bigint)1;
}
const bigint bigint::operator ++ (int)  //post-increment
{
    bigint x = *this;
    *this = *this + (bigint)1;
    return x;
}
const bigint bigint::operator -- (int)  //post-decrement
{
    bigint x = *this;
    *this = *this - (bigint)1;
    return x;
}




//constexpr unsigned long long MAX = sqrt(ULLONG_MAX);
//          //KARATSUBA MULTILICATION -- SLOW
//bigint operator * (const bigint& A, const bigint& B)
//{
//    //cout<<A<<'*'<<B<<endl;
//    if(A.v.empty() || B.v.empty())
//        throw invalid_argument("multiplication on uninitialised bigints");
//    bigint result;
//    if(A.compare_mag(MAX)==-1 && B.compare_mag(MAX)==-1)
//        result = A.toUnsigned() * B.toUnsigned();
//    else
//    {
//        int i, na=A.v.size(), nb=B.v.size();
//
//        bigint a,b,c,d;
//        if(na==1)
//            a.v.push_back(0);
//        for (i=0; i<na/2; i++)
//            a.v.push_back(A.v[i]);
//        for (; i<na; i++)
//            b.v.push_back(A.v[i]);
//        if(nb==1)
//            c.v.push_back(0);
//        for (i=0; i<nb/2; i++)
//            c.v.push_back(B.v[i]);
//        for (; i<nb; i++)
//            d.v.push_back(B.v[i]);
//        bigint ac = a*c, bd = b*d, ad = a*d, bc = b*c;
//
//        for (i=0; i<(na-na/2+nb-nb/2); i++) ac.v.push_back(0);
//        for (i=0; i<(na-na/2); i++) ad.v.push_back(0);
//        for (i=0; i<(nb-nb/2); i++) bc.v.push_back(0);
//        //cout<<setw(30)<<ac<<endl<<setw(30)<<ad<<endl<<setw(30)<<bc<<endl<<setw(30)<<bd<<endl;
//        result = ac.add_mag(ad).add_mag(bc).add_mag(bd);
//    }
//
//    result.sign = (A.sign==B.sign);
//    return result;
//}



bigint operator * (bigint const& a, bigint const& b)
{
    if(a.v.empty() || b.v.empty())
        throw invalid_argument("multiplication on uninitialised bigints");
    bigint result = 0;
    if(a.compare_mag(0)==0 || b.compare_mag(0)==0)
        return result;
    vector <bigint> res(b.v.size());
    int carry=0;
    auto p = a.v.end()-1;
    auto q = b.v.end()-1;
    int k=0;
    while(1)
    {
        int val = *p * *q + carry;
        //cout<<"*p="<<*p<<", *q="<<*q<<", carry="<<carry<<". val="<<val<<endl;
        if(val <= 9)
        {
            res[k].v.insert(res[k].v.begin(),val);
            carry=0;
        }
        else
        {
            carry = val/10;
            res[k].v.insert(res[k].v.begin(),val%10);
        }
        //for(int i:res[k].v) cout<<i; cout<<endl;
        if(p != a.v.begin())
            p--;
        else
        {
            if(carry)
            {
                res[k].v.insert(res[k].v.begin(),carry);
                carry=0;
            }
            if(q != b.v.begin())
                q--;
            else
                break;
            p=a.v.end()-1;
            //for(int i:res[k].v) cout<<i; cout<<endl;
            k++;
            for (int i=0; i<k; i++)
                res[k].v.insert(res[k].v.begin(),0);
        }
    }
    for (int i=0; i<=k; i++)
    {
        //cout<<res[i]<<endl;
        result = result.add_mag(res[i]);
    }
    result.sign = (a.sign==b.sign);
    return result;
}






inline bigint& bigint::operator += (const bigint& that) { return *this = *this + that; }
inline bigint& bigint::operator -= (const bigint& that) { return *this = *this - that; }
inline bigint& bigint::operator *= (const bigint& that) { return *this = *this * that; }
inline bigint& bigint::operator /= (int that) { return *this = *this / that; }
inline bigint& bigint::operator %= (int that) { return *this = *this % that; }
inline bigint& bigint::operator |= (const bigint& that) { return *this = *this | that; }
inline bigint& bigint::operator &= (const bigint& that) { return *this = *this & that; }
inline bigint& bigint::operator ^= (const bigint& that) { return *this = *this ^ that; }
inline bigint& bigint::operator <<= (int that) { return *this = *this << that; }
inline bigint& bigint::operator >>= (int that) { return *this = *this >> that; }

inline bigint operator - (bigint x) { return x.negate(); }

ostream& operator << (ostream& strm, const bigint& b)
{
    if(b.sign==false)
        strm<<'-';
    for (int i : b.v)
        strm<<i;
    return strm;
}
istream& operator >> (istream& strm, bigint &b)
{
    string s;
    strm>>s;
    b = s;
    return strm;
}

int bigint::operator[] (int n)
{
    if(n>=0)
    {
        if(n >= v.size())
            throw out_of_range(("index " + to_string(n) + " out of range").c_str());
        return v[n];
    }
    else
    {
        if(n < -(int)v.size())
            throw out_of_range(("index " + to_string(n) + " out of range").c_str());
        return v[v.size()+n];
    }
}

inline int bigint::num_digits() const
{
    return v.size();
}

string to_string(const bigint& x)
{
    string s;
    if(x.sign==false)
        s.insert(s.begin(),'-');
    for (int i : x.v)
        s.insert(s.end(), i+'0');
    return s;
}

int signum(const bigint& n)
{
    if(n.sign == false)
        return -1;
    if(n.v.front()==0)
        return 0;
    return 1;
}

inline bigint abs(const bigint& n)
{
    return (n.sign==false ? -n : n);
}

bigint factorial(const bigint& n)
{
    if(n.sign == false)
        throw invalid_argument("factorial on negative bigint");
    if(n.v.size()==1 && n.v[0]<=1)  //if n is 0 or 1
        return 1;
    bigint res=2, i=3;
    while(i<=n)
    {
        res = res*i;
        cout<<i<<"! = "<<res<<endl;
        i=i+1;
    }
    return res;
}
bigint factorial(long long n)
{
    if(n < 0)
        throw invalid_argument("factorial on negative number");
    if(n<=1)  //if n is 0 or 1
        return 1;
    bigint res=2, i=3, x=n;
    while(i<=x)
    {
        res = res*i;
        cout<<i<<"! = "<<res<<endl;
        i=i+1;
    }
    return res;
}

bigint operator / (const bigint& a, long long b)
{
    if(a.v.empty())
        throw invalid_argument("division on uninitialised bigint");
    if(b == 0)
        throw invalid_argument("division by 0");
    if(a.compare_mag(b)==-1)
        return (bigint)0;
    bigint res;
    auto p = a.v.begin();
    long long q, num=0;
    bool b_sign = (b>=0 ? true : false);
    if(b_sign==false) b = -b;
    while(num < b)
    {
        num *= 10;
        num += *p;
        p++;
    }
    while(1)
    {
        q = num / b;
        res.v.push_back(q);
        if(p==a.v.end())
            break;
        num = (num % b) * 10 + *p;
        p++;
    }
    res.sign = (a.sign == b_sign);
    return res;
}
long long operator % (const bigint& a, long long b)
{
    if(a.v.empty())
        throw invalid_argument("modulo on uninitialised bigint");
    if(b == 0)
        throw invalid_argument("division by 0");
    if(a.compare_mag(b)==-1)
        return a.toInteger();
    auto p = a.v.begin();
    long long res, num=0;
    while(num < b)
    {
        num *= 10;
        num += *p;
        p++;
    }
    while(1)
    {
        res = num % b;
        if(p==a.v.end())
            break;
        num = res * 10 + *p;
        p++;
    }
    return res;
}
bigint operator / (const bigint& a, const bigint& b) { return a/b.toInteger(); }
long long operator % (const bigint& a, const bigint& b) { return a%b.toInteger(); }

string bigint::binary() const
{
    string str;
    bigint x = abs(*this);
    while(x>0)
    {
        str.insert(str.begin(),x%2+'0');
        x=x/2;
    }
    if(sign==false)
        str.insert(str.begin(),'-');
    return str;
}

void bigint::twosComplement()
{
    for (int i=0; i<v.size(); i++)
        v[i] = (v[i]==0 ? 1 : 0);
    auto p = v.end()-1;
    int flag=0;
    while(*p==1)
    {
        *p=0;
        if(p==v.begin())
        {
            flag=1;
            break;
        }
        else
            p--;
    }
    if(flag)
        v.insert(v.begin(),1);
    else
        *p=1;
}
bigint bigint::decimal() const
{
    bigint res=0; long long place_value=1;
    for (auto r=v.rbegin(); r!=v.rend(); ++r)
    {
        long long x = (*r) * place_value;
        res = res.add_mag(x);
        place_value *= 2;
    }
    return res;
}

bigint operator | (const bigint& a, const bigint& b)
{
    if(a.v.empty() || b.v.empty())
        throw invalid_argument("bitwise OR on uninitialised bigints");
    // Converting numbers to binary form
    bigint op1 = a.binary();
    if(a.sign==false)
        op1.twosComplement();
    bigint op2 = b.binary();
    if(b.sign==false)
        op2.twosComplement();
//    cout<<"op1: "<<op1<<'\n';
//    cout<<"op2: "<<op2<<'\n';

    auto p = op1.v.end()-1;
    auto q = op2.v.end()-1;

    // Computing bitwise OR
    bigint bin;
    bool breaked=false;
    while(1)
    {
        int bit = *p | *q;
        bin.v.insert(bin.v.begin(),bit);

        if(p!=op1.v.begin() && q!=op2.v.begin())
            p--, q--;
        else if(p==op1.v.begin())
        {
            if(q==op2.v.begin())
                break;
            int starp = a.sign==true ? 0 : 1;
            while(1)
            {
                q--;
                bit = starp | *q;
                bin.v.insert(bin.v.begin(),bit);
                if(q==op2.v.begin())
                { breaked=true; break; }
            }
        }
        else ///if(q==op2.v.begin())
        {
            int starq = b.sign==true ? 0 : 1;
            while(1)
            {
                p--;
                bit = *p | starq;
                bin.v.insert(bin.v.begin(),bit);
                if(p==op1.v.begin())
                { breaked=true; break; }
            }
        }
        if(breaked)
            break;
    }
    //cout<<"bin: "<<bin<<endl;
    // Taking 2's complement if output is to be negative
    bool flag=true;
    if(!a.sign || !b.sign)
    {
        bin.twosComplement();
        flag=false;
    }
    //cout<<"bin: "<<bin<<endl;
    bigint res = bin.decimal();
    //cout<<"res: "<<res<<endl;
    if(flag==false)
        res.sign=false;

    return res;
}

bigint operator & (const bigint& a, const bigint& b)
{
    if(a.v.empty() || b.v.empty())
        throw invalid_argument("bitwise AND on uninitialised bigints");
    // Converting numbers to binary form
    bigint op1 = a.binary();
    if(a.sign==false)
        op1.twosComplement();
    bigint op2 = b.binary();
    if(b.sign==false)
        op2.twosComplement();
//    cout<<"op1: "<<op1<<'\n';
//    cout<<"op2: "<<op2<<'\n';

    auto p = op1.v.end()-1;
    auto q = op2.v.end()-1;

    // Computing bitwise AND
    bigint bin;
    bool breaked=false;
    while(1)
    {
        int bit = *p & *q;
        bin.v.insert(bin.v.begin(),bit);

        if(p!=op1.v.begin() && q!=op2.v.begin())
            p--, q--;
        else if(p==op1.v.begin())
        {
            if(q==op2.v.begin())
                break;
            int starp = a.sign==true ? 0 : 1;
            while(1)
            {
                q--;
                bit = starp & *q;
                bin.v.insert(bin.v.begin(),bit);
                if(q==op2.v.begin())
                { breaked=true; break; }
            }
        }
        else ///if(q==op2.v.begin())
        {
            int starq = b.sign==true ? 0 : 1;
            while(1)
            {
                p--;
                bit = *p & starq;
                bin.v.insert(bin.v.begin(),bit);
                if(p==op1.v.begin())
                { breaked=true; break; }
            }
        }
        if(breaked)
            break;
    }
    //cout<<"bin: "<<bin<<'\n';

    // Taking 2's complement if output is to be negative
    bool sign_flag=true;
    if(!a.sign && !b.sign)
    {
        bin.twosComplement();
        sign_flag=false;
    }
    //cout<<"bin: "<<bin<<'\n';

    // Computing decimal value of binary
    bigint res = bin.decimal();

    res.sign = sign_flag;
    //cout<<"res: "<<res<<'\n';

    return res;
}

bigint operator ^ (const bigint& a, const bigint& b)
{
    if(a.v.empty() || b.v.empty())
        throw invalid_argument("bitwise XOR on uninitialised bigints");
    // Converting numbers to binary form
    bigint op1 = a.binary();
    if(a.sign==false)
        op1.twosComplement();
    bigint op2 = b.binary();
    if(b.sign==false)
        op2.twosComplement();

    auto p = op1.v.end()-1;
    auto q = op2.v.end()-1;

    // Computing bitwise AND
    bigint bin;
    bool breaked=false;
    while(1)
    {
        int bit = *p ^ *q;
        bin.v.insert(bin.v.begin(),bit);

        if(p!=op1.v.begin() && q!=op2.v.begin())
            p--, q--;
        else if(p==op1.v.begin())
        {
            if(q==op2.v.begin())
                break;
            int starp = a.sign==true ? 0 : 1;
            while(1)
            {
                q--;
                bit = starp ^ *q;
                bin.v.insert(bin.v.begin(),bit);
                if(q==op2.v.begin())
                { breaked=true; break; }
            }
        }
        else ///if(q==op2.v.begin())
        {
            int starq = b.sign==true ? 0 : 1;
            while(1)
            {
                p--;
                bit = *p ^ starq;
                bin.v.insert(bin.v.begin(),bit);
                if(p==op1.v.begin())
                { breaked=true; break; }
            }
        }
        if(breaked)
            break;
    }

    // Taking 2's complement if output is to be negative
    bool sign_flag=true;
    if(a.sign != b.sign)
    {
        bin.twosComplement();
        sign_flag=false;
    }

    // Computing decimal value of binary
    bigint res = bin.decimal();

    res.sign = sign_flag;

    return res;
}

bigint operator ~ (const bigint& B)
{
    if(B.v.empty())
        throw invalid_argument("bitwise NOT on uninitialised bigint");
    bigint bin = B.binary();
    bigint res;

    if(B.sign==true)
    {
        // Flip the bits
        for (int i=0; i<bin.v.size(); i++)
            bin.v[i] = (bin.v[i]==0 ? 1 : 0);

        bin.twosComplement();
        res = bin.decimal();
        res.sign=false;
    }

    else
    {
        bin.twosComplement();

        //Flip the bits
        for (int i=0; i<bin.v.size(); i++)
            bin.v[i] = (bin.v[i]==0 ? 1 : 0);

        res = bin.decimal();
    }

    return res;
}

bigint operator << (const bigint& a, int n)
{
    if(a.v.empty())
        throw invalid_argument("operator << on uninitialised bigint");
    if(n<0)
        throw invalid_argument("negative left-shift value");
    bigint bin = a.binary();
    while(n--)
        bin.v.push_back(0);
    //cout<<"bin: "<<bin<<'\n';

    bigint res = bin.decimal();
    res.sign = a.sign;
    //cout<<"res: "<<res<<'\n';

    return res;
}

bigint operator >> (const bigint& a, int n)
{
    if(a.v.empty())
        throw invalid_argument("operator >> on uninitialised bigint");
    if(n<0)
        throw invalid_argument("negative right-shift value");
    bigint bin = a.binary();
    if(a.sign==false)
        bin.twosComplement();
    //cout<<"bin: "<<bin<<'\n';

    bigint resbin;
    int bit = (a.sign==true ? 0 : 1);
    for(int i=0; i<n; i++)
        resbin.v.push_back(bit);

    int k = bin.v.size()-n;
    for (int i=0; i<k; i++)
        resbin.v.push_back(bin.v[i]);
    //cout<<"resbin: "<<resbin<<'\n';

    if(a.sign==false)
        resbin.twosComplement();
    //cout<<"resbin: "<<resbin<<'\n';

    bigint res = resbin.decimal();

    res.sign = a.sign;
    //cout<<"res: "<<res<<'\n';
    return res;
}

bigint bigint::flipbit(int n)
{
    if(n>=binary().size() || n<0)
        throw out_of_range(("index " + to_string(n) + " out of range").c_str());
    bigint bin = binary();
    bin.v[n] = (bin.v[n]==0 ? 1 : 0);
    return bin.decimal();
}
bigint bigint::setbit(int n)
{
    if(n>=binary().size() || n<0)
        throw out_of_range(("index " + to_string(n) + " out of range").c_str());
    bigint bin = binary();
    bin.v[n] = 1;
    return bin.decimal();
}
bigint bigint::clearbit(int n)
{
    if(n>=binary().size() || n<0)
        throw out_of_range(("index " + to_string(n) + " out of range").c_str());
    bigint bin = binary();
    bin.v[n] = 0;
    return bin.decimal();
}
bool bigint::testbit(int n)
{
    if(n>=binary().size() || n<0)
        throw out_of_range(("index " + to_string(n) + " out of range").c_str());
    bigint bin = binary();
    return (bin.v[n]==1);
}
