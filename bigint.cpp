#include<bits/stdc++.h>
using namespace std;
#include "bigint.h"

bigint bigint::add_mag(const bigint& a, const bigint& b)
{
    try {if(a.v.empty() || b.v.empty()) throw invalid_argument("operation on uninitialised bigints");
    } catch(invalid_argument& e) { cerr<<e.what()<<endl; }
    bigint res;
    int carry=0;
    auto p = a.v.end()-1;
    auto q = b.v.end()-1;
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
        if( (p == a.v.begin() || p==zero.begin()) && \
                (q == b.v.begin() || q==zero.begin()) )
        {
            if(carry)
                res.v.insert(res.v.begin(),carry);
            break;
        }
        if(p == a.v.begin() || p==zero.begin())
            p = zero.begin();
        else
            p--;
        if(q == b.v.begin() || q==zero.begin() )
            q = zero.begin();
        else
            q--;
    }
    res.sign = true;
    return res;
}
bigint bigint::subtract_mag(const bigint& a, const bigint& b) //assumes: |a| > |b|
{
    try { if(a.v.empty() || b.v.empty()) throw invalid_argument("operation on uninitialised bigints");
          //if(compare_mag(a,b)==-1) throw invalid_argument("invalid subtraction");
    } catch(invalid_argument& e) { cerr<<e.what()<<endl; }
    bigint res;
    auto p = a.v.end()-1;
    auto q = b.v.end()-1;
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
        if(q==b.v.begin() || q==zero.begin())
            q=zero.begin();
        else
            q--;
        if(p==a.v.begin())
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
int bigint::compare_mag(const bigint& a, const bigint& b)
{
    try {if(a.v.empty() || b.v.empty()) throw invalid_argument("comparison on uninitialised bigints");
    } catch(invalid_argument& e) { cerr<<e.what()<<endl; }
    if(a.v.size() > b.v.size()) return 1;
    else if (a.v.size() < b.v.size()) return -1;
    for(auto i=a.v.begin(),j=b.v.begin();  ; i++,j++)
    {
        if(*i > *j) return 1;
        if(*i < *j) return -1;
        if(i==a.v.end())
            return 0; ///equality case
    }
}
bigint::bigint() { sign=true; }
bigint::bigint(char *s)
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
        try
        {
            if(!isdigit(*s))
                throw invalid_argument("non-numeric initialiser for bigint");
            v.push_back(*s-'0');
        }
        catch(invalid_argument& e)
        {
            cerr<<e.what()<<endl;
        }
    }
}
bigint::bigint(string s)
{
    if(s[0]=='-')
    {
        sign=false;
        for (int i=1; i<s.size(); i++)
        {
            if(!isdigit(i))
                throw invalid_argument("non-numeric initialiser for bigint");
            v.push_back(i-'0');
        }
    }
    else
    {
        sign=true;
        for (char i:s)
        {
            if(!isdigit(i))
                throw invalid_argument("non-numeric initialiser for bigint");
            v.push_back(i-'0');
        }
    }

}
bigint::bigint(int n)
{
    if(n>=0) sign=true;
    else { sign=false; n=-n; }
    if(n==0) v.insert(v.begin(),0);
    while(n)
    {
        int digit = n%10;
        v.insert(v.begin(),digit);
        n /= 10;
    }
}
bigint bigint::negate()
{
    if(sign == true) sign = false;
    else sign = true;
    return *this;
}
bigint bigint::abs()
{
    sign = true;
    return *this;
}
long long bigint::toInt()
{
    try { if(v.size()>18) throw range_error("too large for long long int");
    } catch (range_error& e) { cerr<<e.what()<<endl; }
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
bool bigint::operator > (const bigint& that)
{
    if(sign && !that.sign) return true;
    if(!sign && that.sign) return false;
    if(sign && that.sign)
    {
        if(compare_mag(*this,that)==1) return true;
        else return false;
    }
    if(compare_mag(*this,that)==1) return false;
    else return true;
}
bool bigint::operator < (const bigint& that)
{
    if(sign && !that.sign) return false;
    if(!sign && that.sign) return true;
    if(sign && that.sign)
    {
        if(compare_mag(*this,that)==-1) return true;
        else return false;
    }
    if(compare_mag(*this,that)==-1) return false;
    else return true;
}
bool bigint::operator >= (const bigint& that)
{
    if(sign && !that.sign) return true;
    if(!sign && that.sign) return false;
    if(sign && that.sign)
    {
        if(compare_mag(*this,that)==-1) return false;
        else return true;
    }
    if(compare_mag(*this,that)==-1) return true;
    else return false;
}
bool bigint::operator <= (const bigint& that)
{
    if(sign && !that.sign) return false;
    if(!sign && that.sign) return true;
    if(sign && that.sign)
    {
        if(compare_mag(*this,that)==1) return false;
        else return true;
    }
    if(compare_mag(*this,that)==1) return true;
    else return false;
}
bool bigint::operator == (const bigint& that)
{
    if(sign != that.sign)
        return false;
    if(compare_mag(*this,that)==0)
        return true;
    else
        return false;
}
bool bigint::operator != (const bigint& that)
{
    if(sign != that.sign)
        return true;
    if(compare_mag(*this,that)==0)
        return false;
    else
        return true;
}
//    ostream& bigint::operator << (ostream& outputStream)
//    {
//        for (int i : v)
//            outputStream<<i;
//        return outputStream;
//    }

bigint bigint::operator + (const bigint& that)
{
    if(sign && that.sign)
        return add_mag(*this,that);
    if(sign && !that.sign)
    {
        if(compare_mag(*this,that)>=0)
            return subtract_mag(*this,that);
        else
            return subtract_mag(that,*this).negate();
    }
    if(!sign && that.sign)
    {
        if(compare_mag(*this,that)>=0)
            return subtract_mag(*this,that).negate();
        else
            return subtract_mag(that,*this);
    }
    return add_mag(*this,that).negate();
}
bigint bigint::operator - (bigint const& that)
{
    if(sign && that.sign)
    {
        if (compare_mag(*this,that) >= 0)
            return subtract_mag(*this,that);
        else
            return subtract_mag(that,*this).negate();
    }
    if(!sign && that.sign)
    {
        return add_mag(*this,that).negate();
    }
    if(!sign && !that.sign)
    {
        if (compare_mag(*this,that) >= 0)
            return subtract_mag(*this,that).negate();
        else
            return subtract_mag(that,*this);
    }
    return add_mag(*this,that);
}
bigint bigint::operator -- ()   //prefix
{
    *this = *this-1;
    return *this;
}
bigint bigint::operator ++ ()  //prefix
{
    *this = *this+1;
    return *this;
}
bigint bigint::operator ++ (int)  //postfix
{
    bigint x = *this;
    *this = *this + 1;
    return x;
}
bigint bigint::operator -- (int)  //postfix
{
    bigint x = *this;
    *this = *this - 1;
    return x;
}
bigint bigint::operator * (bigint that)
{
    bigint result = 0;
    if(*this == (bigint)0 || that == (bigint)0)
        return result;
    vector <bigint> res(that.v.size());
    int carry=0;
    auto p = v.end()-1;
    auto q = that.v.end()-1;
    int k=0;
    while(1)
    {
        int val = *p * *q + carry;
        //cout<<"*p="<<*p<<", *q="<<*q<<", carry="<<carry<<". val="<<val<<endl;
        if(val <= 9)
        {
            res.at(k).v.insert(res.at(k).v.begin(),val);
            carry=0;
        }
        else
        {
            carry = val/10;
            res.at(k).v.insert(res.at(k).v.begin(),val%10);
        }
        //for(int i:res[k].v) cout<<i; cout<<endl;
        if(p != v.begin())
            p--;
        else
        {
            if(carry)
            {
                res.at(k).v.insert(res.at(k).v.begin(),carry);
                carry=0;
            }
            if(q != that.v.begin())
                q--;
            else
                break;
            p=v.end()-1;
            //for(int i:res[k].v) cout<<i; cout<<endl;
            k++;
            for (int i=0; i<k; i++)
                res.at(k).v.insert(res.at(k).v.begin(),0);
        }
    }
    for (int i=0; i<=k; i++)
    {
        //cout<<res[i]<<endl;
        result = add_mag(result,res[i]);
    }
    result.sign = (sign==that.sign);
    return result;
}

void bigint::operator += (bigint that) { *this = *this + that; }
void bigint::operator -= (bigint that) { *this = *this - that; }
void bigint::operator *= (bigint that) { *this = *this * that; }

bigint bigint::operator + (int that) { return *this + (bigint)that; }
bigint bigint::operator - (int that) { return *this - (bigint)that; }
bigint bigint::operator * (int that) { return *this * (bigint)that; }
void bigint::operator += (int that) { *this = *this + (bigint)that; }
void bigint::operator -= (int that) { *this = *this - (bigint)that; }
void bigint::operator *= (int that) { *this = *this * (bigint)that; }
bool bigint::operator > (int that) { return *this > (bigint)that; }
bool bigint::operator < (int that) { return *this < (bigint)that; }
bool bigint::operator >= (int that) { return *this >= (bigint)that; }
bool bigint::operator <= (int that) { return *this <= (bigint)that; }
bool bigint::operator == (int that) { return *this == (bigint)that; }
bool bigint::operator != (int that) { return *this != (bigint)that; }

static bigint bigint::factorial(bigint n)
{
    try { if(n.sign==false) throw invalid_argument("factorial on negative bigint");
    } catch(invalid_argument& e) { cerr<<e.what()<<endl; }
    if(n==(bigint)1 || n==(bigint)0)
        return (bigint)1;
    bigint res=1;
    for (bigint i=2; i<=n; i=i+1)
        res = res * i;
    return res;
}
bigint bigint::factorial()
{
    try { if(sign==false) throw invalid_argument("factorial on negative bigint");
    } catch(invalid_argument& e) { cerr<<e.what()<<endl; }
    if(*this==(bigint)1 || *this==(bigint)0)
        return (bigint)1;
    bigint res=1;
    for (bigint i=2; i<=*this; i=i+1)
        res = res * i;
    return res;
}
//    bigint operator / (int that)
//    {
//        bigint res;
//        auto p = v.begin();
//        int digits_divisor = to_string(that).size(), q;
//        while(1)
//        {
//            int num=0;
//            for(int i=0; i<digits_divisor; i++)
//            {
//                num *= 10;
//                num += *p;
//                p++;
//            }
//            q = num / that;
//            res.v.insert(res.v.end(),0);
//            p += digits_divisor;
//            if(p >= res.v.end())
//                break;
//        }
//
//    }
};
ostream& operator << (ostream& strm, const bigint& b)
{
    if(b.sign==false) strm<<'-';
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
string to_string (bigint x)
{
    stringstream s;
    s<<x;
    return s.str();
}
