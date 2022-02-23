#include<iostream>
#include<string>
#include<math.h>
#include<vector>

using namespace std;

class BigInteger
{
public:
    string a;

    BigInteger()
    {
        a.resize(0);
    }

    BigInteger(const BigInteger &b)
    {
        a = b.a;
    }

    BigInteger(string s)
    {
        a = s;
    }

    /** To get the value of BigInteger */
    string getValue()
    {
        while(*a.begin() == '0' )
            a.erase(a.begin());
        if(a.empty())
            a = "0";

        return a;
    }

    BigInteger add(BigInteger b)
    {
        BigInteger sum;
        bool AnsSign = true;

        if(a[0] != '-' && b.a[0] == '-')
        {
            b.a.erase(b.a.begin());
            sum = this->subtract(b);
            return sum;
        }

        if(a[0] == '-' && b.a[0] != '-')
        {
            a.erase(a.begin());
            sum = b.subtract(*this);
            return sum;
        }

        if(a[0] == '-' && b.a[0] == '-')
        {
            AnsSign = !AnsSign;
            a.erase(a.begin());
            b.a.erase(b.a.begin());
        }

        int len1 = a.length() ,len2 = b.a.length();
        int a1[len1] = {0}, a2[len2] = {0};

        int len = max(len1,len2)+1;
        int ans[len] = {0};

        convertStringToArray(a1,a);

        convertStringToArray(a2,b.a);

        /** Storing sum inside ans[] array */
        addArrays(ans,a1,a2,len1,len2);

        sum.a = convertArrayToString(ans,len,AnsSign);

        return sum;
    }

    BigInteger subtract(BigInteger b)
    {
        bool AnsSign = true;
        BigInteger sub;

        if(a[0] != '-' && b.a[0] == '-')
        {
            b.a.erase(b.a.begin());
            sub = this->add(b);
            return sub;
        }

        if(a[0] == '-' && b.a[0] != '-')
        {
            b.a.insert(b.a.begin(),'-');
            sub = this->add(b);
            return sub;
        }

        if(a[0] == '-' && b.a[0] == '-')
        {
            b.a.erase(b.a.begin());
            sub = this->add(b);
            return sub;
        }

        int len1 = a.length(), len2 = b.a.length();
        int a1[len1] = {0}, a2[len2] = {0};

        int MaxLen = max(a.length(),b.a.length());
        int ans[MaxLen] = {0};

        convertStringToArray(a1,a);

        convertStringToArray(a2,b.a);

        /** Storing subtraction inside ans[] array and getting ans sign in AnsSign */
        AnsSign = subtractArrays(ans,a1,a2,len1,len2);

        sub.a = convertArrayToString(ans,MaxLen,AnsSign);

        return sub;
    }

    BigInteger multiply(BigInteger b)
    {
        bool AnsSign = true;
        BigInteger mult;

        if(a[0] == '-')
        {
            AnsSign = !AnsSign;
            a.erase(a.begin());
        }
        if(b.a[0] == '-')
        {
            AnsSign = !AnsSign;
            b.a.erase(b.a.begin());
        }

        int len1 = a.length(), len2 = b.a.length();
        int a1[len1] = {0}, a2[len2] = {0};
        int ans[len1+len2] = {0};

        convertStringToArray(a1,a);

        convertStringToArray(a2,b.a);

        /** Storing multiplication inside ans[] */
        multiplyArray(ans,a1,a2,len1,len2);

        mult.a = convertArrayToString(ans,len1+len2,AnsSign);

        return mult;
    }

    BigInteger divide(BigInteger b)
    {
        bool AnsSign = true;
        BigInteger div;

        if(a[0] == '-') {
            a.erase(a.begin());
            AnsSign = !AnsSign;
        }
        if(b.a[0] == '-') {
            b.a.erase(b.a.begin());
            AnsSign = !AnsSign;
        }

        int len1 = a.length() ,len2 = b.a.length();

        if(len1 < len2)
        {
            div.a.resize(1);
            div.a = "0";
            return div;
        }

        if(len1 == len2)
        {
            if( a < b.a)
            {
                div.a.resize(1);
                div.a = "0";
                return div;
            }
            else if (a == b.a)
            {
                div.a.resize(1);
                if(AnsSign)
                    div.a = "1";
                else
                    div.a = "-1";
                return div;
            }
        }

        vector<int> dividend;                   /** Converting dividend string to vector*/
        for(int i=0;i<len1;i++)
            dividend.push_back( a[i] - '0' );

        vector<int>::iterator dividend_itr;

        vector<int> divisor;                    /** Converting divisor string to vector*/
        for(int i=0;i<len2;i++)
            divisor.push_back( b.a[i] - '0');

        vector<int>::iterator divisor_itr;

        vector<int> DivisorTable[9];
        vector<int>::iterator dtitr;

        /**  To create table of divisor inside TableVector **/
        for(int i=0;i<9;i++)
        {
            vector<int>::iterator ditr = divisor.end()-1 ;

            DivisorTable[i].push_back(0);       /** to make our vector non empty*/

            int n=0,carry=0;
            while( ditr != divisor.begin()-1)
            {
                n = *ditr * (i+1) + carry;
                DivisorTable[i].insert( DivisorTable[i].begin(),n%10 );
                carry = n/10;

                ditr--;
            }
            if( carry != 0 );
                DivisorTable[i].insert(DivisorTable[i].begin(),carry);

            DivisorTable[i].pop_back();         /** to remove the zero which was add earlier for non-empty*/

            vector<int>::iterator itr = DivisorTable[i].begin();
            if( *itr == 0 )
                DivisorTable[i].erase(itr);
        }

        vector<int> remainder;               /** to store the intermediate remainders */
        vector<int>::iterator ritr;

        vector<int> ans;                     /** ans vector to store the quotient */
        vector<int>::iterator aitr;

        dividend_itr = dividend.begin();
        int counter = 0, divi = 0;
        bool pop = false;

        while(dividend_itr != dividend.end())
        {
            remainder.push_back( *dividend_itr );
            counter++;

            if(divi != 0 && counter > 1)
                ans.push_back(0);

            if(pop)
            {
                ritr = remainder.begin();
                remainder.erase(ritr);
                pop = false;
            }

            if( (remainder.size() == divisor.size() && remainder >= divisor ) || remainder.size() > divisor.size() )
            {
                counter = 0;    divi = 1;
                int i;

                /** to check the genuine quotient */
                for( i=0;i<9;i++ )
                    if( (DivisorTable[i].size() == remainder.size() && DivisorTable[i] > remainder) || DivisorTable[i].size() > remainder.size() )
                        break;


                i = i == 9 ? 8 : i-1;
                ans.push_back( i+1 );

                ritr = remainder.end()-1;
                dtitr = DivisorTable[i].end()-1;

                /** to subtract two vectors */
                while( dtitr != DivisorTable[i].begin()-1 )
                {
                    if( *ritr < *dtitr )
                    {
                        vector<int>::iterator temp = ritr;
                        *ritr += 10;

                        while(*--temp == 0)
                            *temp = 9;

                        *temp -= 1;
                    }
                    *ritr -= *dtitr;

                    dtitr--;    ritr--;
                }

                /** to remove zeros at leftmost side  */
                vector<int>::iterator temp;
                temp = ritr = remainder.begin();
                while( *ritr == 0 )
                {
                    if( remainder.size() != 1)
                        remainder.erase(ritr);
                    else {
                        pop = true;
                        break;
                    }
                }
            }
            dividend_itr++;
        }

        /** converting ans from vector to our big integer variable */

        if(!AnsSign)                /** to put negative sign if ans is negative */
            div.a.push_back('-');

        aitr = ans.begin();
        while(aitr != ans.end())
        {
            div.a.push_back( *aitr + '0' );
            aitr++;
        }

        return div;
    }

private:

    void convertStringToArray(int arr[],string s)
    {
        int k=0;
        int len = s.length();
        for(int i = len-1 ; i>=0 ; i--)
            arr[k++] = s[i] - '0';
    }

    string convertArrayToString(int ans[], int ans_len, bool AnsSign)
    {
        string str;
        int i = ans_len - 1;
        while(ans[i] == 0)
            i--;

        if(i<0){
            str.push_back('0');
            return str;
        }

        if(!AnsSign)
            str.push_back('-');

        for( i=i ; i >= 0 ; i--)
            str.push_back(ans[i] + '0');

        return str;
    }

    void addArrays(int ans[],int a1[],int a2[],int len1, int len2)
    {
        int MaxLen = max(len1,len2);
        int MinLen = min(len1,len2);
        int carry = 0, i = 0;

        for(i = 0;i<MinLen;i++)
        {
            ans[i] = a1[i] + a2[i] + carry;
            carry = ans[i]/10;
            ans[i] %= 10;
        }

        if(len1 > len2)
            addRemainingArrays(ans,a1,MaxLen,i,carry);

        else
            addRemainingArrays(ans,a2,MaxLen,i,carry);

    }

    void addRemainingArrays(int ans[],int arr[],int MaxLen,int i,int carry)
    {
        for(i=i ; i < MaxLen ; i++)
        {
            ans[i] = arr[i] + carry;
            carry = ans[i]/10;
            ans[i] %= 10;
        }
        ans[i] = carry;
    }

    bool subtractArrays(int ans[], int a1[], int a2[],int len1, int len2)
    {
        bool AnsSign = true;
        int temp = len1;
        int MaxLen = max(len1,len2);

        if(len1 == len2)
        {
            while( temp-- )
            {
                if(a1[temp] == a2[temp])
                    continue;
                else if(a1[temp] > a2[temp])
                    break;

                else {
                    AnsSign = false;
                    break;
                }
            }

            if(AnsSign)
                subArrays(ans,a1,a2,len1,MaxLen);

            else
                subArrays(ans,a2,a1,len1,MaxLen);
        }

        else if(len1 > len2)
            subArrays(ans,a1,a2,len2,MaxLen);

        else {
            AnsSign = false;
            subArrays(ans,a2,a1,len1,MaxLen);
        }

        return AnsSign;
    }

    void subArrays(int ans[], int la[], int sa[],int len, int MaxLen )
    {
        int i;
        for(i=0;i<len;i++)
        {
            int t=i;
            if(la[t] < sa[t])
            {
                while(la[++t] == 0)
                    la[t] = 9;

                la[t]--;
                la[i] += 10;
            }

            ans[i] = la[i] - sa[i];
        }

        if(len != MaxLen)
        {
            for( ;i<MaxLen;i++)
                ans[i] = la[i];
        }
    }

    void multiplyArray(int ans[], int a1[], int a2[], int len1, int len2)
    {
        int carry = 0;

        for(int i=0;i<len2;i++)
        {
            if(a2[i] == 0)
                continue;
            carry = 0;
            for(int j=0;j<len1;j++)
            {
                ans[i+j] = ans[i+j] + a2[i]*a1[j] + carry;
                carry = ans[i+j]/10;
                ans[i+j] %= 10;
            }
            ans[i+len1] += carry;
        }
    }
};







