#include "std_lib_facilities.h"

const char number='n';
bool iszero=0;
class Token{
public:
    char kind;        // what kind of token
    double value;     // for numbers: a value
    Token(char ch)    // make a Token from a char
        :kind(ch), value(0) { }
    Token(char ch, double val)     // make a Token from a char and a double
        :kind(ch), value(val) { }
};


class Token_stream {
public:
    Token_stream();   // make a Token_stream that reads from cin
    Token get();      // get a Token
    void putback(Token t);    // put a Token back
private:
    bool full;        // is there a Token in the buffer?
    bool full2;
    Token buffer;     // here is where we keep a Token put back using putback()
    Token buffer2;
};

Token_stream::Token_stream()
    :full(false), full2(false),buffer(0),buffer2(0){}

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
    if (full) //if buffer is full,buffer2 must be full
    {
        cout<<"putback() into a full buffer";
        exit(1);
    }
    else if(full2)//buffer2 is full while buffer is empty,fill in buffer
    {
        buffer = t;       // copy t to buffer
        full = true;      // buffer is now full
    }
    else if(!full&&!full2)//buffer and buffer2 are all empty
    {
        buffer2 = t;
        full2 = true;
    }
}

Token Token_stream::get()
{
    if (full) {       // do we already have a Token ready?
        // remove Token from buffer
        full = false;
        return buffer;
    }
    else if(full2)
    {
        full2=false;
        return buffer2;
    }

    char ch;
    cin >> ch;

    switch (ch) {
    case ';':    // for "print"
    case '(': case ')': case '+': case '-': case '*': case '/':
        return Token(ch);        // let each character represent itself
    case '.':
    case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);         // put digit back into the input stream
        double val;
        cin >> val;              // read a floating-point number
        return Token('n', val);   // let 'n' represent "a number"
    }
    default:
        return Token_stream::get();
    }
}

Token_stream ts;        // provides get() and putback()
double expression();
double term();
double primary();
void getnum()//when get a operator and its next char is also a operator, then transform the next character into a number 
{
    Token temp=ts.get();
    if(temp.kind=='+'||temp.kind=='-')
    {
        
        cin.putback(temp.kind);
        double t;
        cin>>t;
        temp.kind=number;
        temp.value=t;
        ts.putback(temp);
    }
    else
    {
        ts.putback(temp);
    }
}


// Start here!
double expression()
{
    // please implement here!
    //
    double left=term();
    while(true)
    {
        Token t=ts.get();
        switch(t.kind)
        {
            case '+': left+=term();break;
            case '-': left-=term();break;
            default: 
                ts.putback(t);return left;
        }
    }


}

double term()
{
    // please implement here!
    //
    double left=primary();
    while(true)
    {
        Token t = ts.get();
        switch(t.kind)
        {
            case '*':
                left *= primary();
                break;
            case '/':
                {   
                    double d = primary();
                    if(d==0){
                        cout<<"divide by zero";
                        system("pause");
                        exit(1);
                    }
                    left/=d;
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
        
    }

}


double primary()
{
    // please implement here!
    //
    Token t = ts.get();
    switch(t.kind)
    {
        case '(':
            {   
                double d=expression();
                t=ts.get();
                if(t.kind!=')') {cout<<"')'expected";exit(1);}
                return d;
            }
        case number:
            return t.value;
        case ';':
            ts.putback(t);
            break;
        case '+':
            return primary();
        case '-':
            return (-1)*primary();
        default:
            cout<<"primary expected";
            exit(1);

    }

}


// main function
int main()
{while(true)
    {double val = 0;
    while (cin) {
        Token t = ts.get();
        if (t.kind == ';')
        {
            cout << fixed;
            cout << setprecision(3) << val << '\n';
            break;
        }
        else
            ts.putback(t);
        val = expression();
    }
    
    }
}
