#include <iostream>
#include <stack>
#include <string.h>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <cstring>

using namespace std;

class IO{
public:

    friend istream& operator>>(istream &in, IO & obj);
    friend ostream&operator<<(ostream &out, const IO & obj);

    virtual void Citire(istream &) = 0;
    virtual void Afisare(ostream &) const = 0;

    virtual ~IO();
};

istream &operator>>(istream &in, IO &obj){
        obj.Citire(in);
        return in;
}

ostream &operator<<(ostream &out, const IO &obj){
        obj.Afisare(out);
        return out;
}

IO::~IO(){}

template<typename T>
class Stack{
private:
    class Node{
    public:
        Node* prev;
        Node* next;
        T caracter;
    public:
        Node(T _char, Node * prev, Node *next){
            this->prev = prev;
            this->next = next;
            this->caracter = _char;
        }
        Node(){
            this->prev = NULL;
            this->next = NULL;
            this->caracter = NULL;
        }
    };

    Node * head;
    Node * tail;
    int _size;
public:
    Stack(){
        head = NULL;
        tail = NULL;
        _size = 0;
    }

    int getSize(){return _size;}
    bool isEmpty(){return _size == 0;}

    void Push(T data){
        if (isEmpty()){
            tail = head = new Node(data, NULL, NULL);
        }else{
            tail->next = new Node(data, tail, NULL);
            tail = tail->next;
        }
        _size++;
    }

    T PeekLast(){
        if(isEmpty() || tail == NULL){throw new runtime_error("Empty list");}
        return tail->caracter;
    }

    void Pop(){
        if (isEmpty()) {return;}

        tail = tail->prev;
        --_size;

        if (isEmpty()) {head = NULL; delete head;}
        else {tail->next = NULL; delete tail->next;}
    }
};

class Calculator: public IO{
public:
    char * operatii;
    int * numere;

    int size_numere;
    int size_operatii;
    char * expresie;

    static int contorOp;
    const int nrOperatii;

    float rezultat;

    //char *operatii_exec[] = "+-*/^#";
    //char *paranteze[] = "()[]";

    Calculator();
    Calculator(const char *);

    void Afisare(ostream & out) const;
    void Citire(istream & in);

    void extractInteger();

    float CALCULEAZA();
    bool isBalanced();

    float applyOp(float a, float b, char op);
    int precedence(char op);
};

int Calculator::precedence(char op){
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^' || op == '#')
        return 3;
    return 0;
}

float Calculator::applyOp(float a, float b, char op){
    switch(op){
        case '+': return a+b;
        break;
        case '-': return a-b;
        break;
        case '*': return a*b;
        break;
        case '/': return a/b;
        break;
        case '^': return pow(a, b);
        break;
        case '#': return pow(a, 1.0/b);//refaci si tu aici;
        break;
    }
}

void Calculator::Afisare(ostream & out) const{
    out << expresie << " = " << rezultat;
}

void Calculator::Citire(istream & in){
    char aux[150];
    cout << "Introduceti expresia: ";
    in.getline(aux, 150);

    if (expresie!=NULL)
        delete[] expresie;
    expresie = new char[strlen(aux)+1];
    strcpy(expresie, aux);
}
/*
void Calculator::extractInteger(){
    char aux[200];
    strcpy(aux, expresie);

    char * pch = strtok(aux, " )(][+-/^*#");
    int contor = 0;
    int vect[100];

    while(pch != NULL){
        stringstream geek(pch);
        int x = 0;
        geek >> x;
        pch = strtok(NULL, " )(][+-/^*#");
        vect[contor] = x;
        contor++;
    }
    size_numere = contor;
    if (numere!=NULL)
        delete[] numere;
    numere = new int[contor];
    for (int i = 0; i<contor; i++)
        numere[i] = vect[i];
} */

Calculator::Calculator():nrOperatii(contorOp){
    operatii = NULL;
    numere = NULL;
    expresie = NULL;
    size_numere = 0;
    size_operatii = 0;
}

Calculator::Calculator(const char* expresie):nrOperatii(contorOp++){
    operatii = NULL;
    numere = NULL;

    this->expresie = new char[strlen(expresie)+1];
    strcpy(this->expresie, expresie);

    size_numere = 0;
    size_operatii = 0;
}

int Calculator::contorOp = 1;

/*
bool Calculator::isBalanced(){
    Stack<char> check_bracket;

    char ch;
    string aux = string(expresie);
    string op = "+-/^*#";

    if (op.find(expresie[0]) != std::string::npos || op.find(expresie[strlen(expresie)-1]) != std::string::npos)
        return false;

    for (int i = 0; i < (int)aux.length(); i++){
        if (aux[i] == '(' || aux[i] == '['){
            check_bracket.Push(aux[i]);
            continue;
        }

        if (op.find(expresie[i]) != std::string::npos){
            operations.Push(expresie[i]);
            continue;
        }

        //if (check_bracket.isEmpty())
         //   return false;

        switch (aux[i]){
        case ')':
            ch = check_bracket.PeekLast();
            check_bracket.Pop();
            if (ch == '[')
                return false;
        break;
        case ']':
            ch = check_bracket.PeekLast();
            check_bracket.Pop();
            if (ch == '(')
                return false;
        break;
        }
    }

    size_operatii = operations.getSize();
    if (operatii != NULL)
        delete[] operatii;
    operatii = new char[operations.getSize()];
    for (int i = operations.getSize()-1 ; i >= 0; i--){
        //strcpy(operatii[i], operations.PeekLast());
        operatii[i] = operations.PeekLast();
        operations.Pop();
    }
    return (check_bracket.isEmpty());
} */

float Calculator::CALCULEAZA(){
        int i;
        //extractInteger();
        Stack<float> values;
        Stack<char> ops;

        string copy_of_expression = string(expresie);
        for (i = 0; i < (int)copy_of_expression.length(); i++){
            if (copy_of_expression[i] == ' ')
                continue;
            else if (copy_of_expression[i] == '[')
                ops.Push(copy_of_expression[i]);

            else if (copy_of_expression[i] == '(')
                ops.Push(copy_of_expression[i]);

            else if (isdigit(copy_of_expression[i])){
                float val = 0;
                string aux;
                while (i < (int)copy_of_expression.length() && (isdigit(copy_of_expression[i]) || copy_of_expression[i] == '.')){

                    aux = aux + copy_of_expression[i];
                    //val = (val*10)+(copy_of_expression[i]-'0');
                    i++;
                }
                char conversie[aux.length()+1];
                strcpy(conversie, aux.c_str());
                val = atof(conversie);
                values.Push(val);
                i--;
            }

            else if (copy_of_expression[i-1] == '(' && copy_of_expression[i] == '-'){
                float val = 0;
                string aux = "-";

                while (i+1 < (int)copy_of_expression.length() && (isdigit(copy_of_expression[i+1]) || copy_of_expression[i+1] == '.')){

                    aux = aux + copy_of_expression[i+1];
                    //val = (val*10)+(copy_of_expression[i]-'0');
                    i++;
                }
                char conversie[aux.length()+1];
                strcpy(conversie, aux.c_str());
                val = atof(conversie);
                values.Push(val);

            }

            else if (copy_of_expression[i] == ']'){
                while(!ops.isEmpty() && ops.PeekLast() != '['){
                    float val2 = values.PeekLast();
                    values.Pop();

                    float val1 = values.PeekLast();
                    values.Pop();

                    char op = ops.PeekLast();
                    ops.Pop();

                    values.Push(applyOp(val1, val2, op));
                }
                if(!ops.isEmpty())
                    ops.Pop();
            }

            else if (copy_of_expression[i] == ')'){
                while(!ops.isEmpty() && ops.PeekLast() != '('){
                    float val2 = values.PeekLast();
                    values.Pop();

                    float val1 = values.PeekLast();
                    values.Pop();

                    char op = ops.PeekLast();
                    ops.Pop();

                    values.Push(applyOp(val1, val2, op));
                }
                if(!ops.isEmpty())
                    ops.Pop();
            }

            else{
                while(!ops.isEmpty() && precedence(ops.PeekLast()) >= precedence(copy_of_expression[i])){
                    float val2 = values.PeekLast();
                    values.Pop();

                    float val1 = values.PeekLast();
                    values.Pop();

                    char op = ops.PeekLast();
                    ops.Pop();

                    values.Push(applyOp(val1, val2, op));
                }
                ops.Push(copy_of_expression[i]);
            }
        }
        while (!ops.isEmpty()){
            float val2 = values.PeekLast();
            values.Pop();

            float val1 = values.PeekLast();
            values.Pop();

            char op = ops.PeekLast();
            ops.Pop();

            values.Push(applyOp(val1, val2, op));
        }
        rezultat = values.PeekLast();
        return values.PeekLast();
}

int main()
{
    try{
        bool running = true;
        while(running){
            char comanda, ch;
            cout << "\nCalculator 1\n";
            cout << "Iesire x\n";
            cin >> comanda;
            ch = getchar();
            switch (comanda){
                case '1': {
                    Calculator obj;
                    cin >> obj;
                    obj.CALCULEAZA();
                    cout << obj;
                break;
                }
                case 'x':
                    running = false;
                break;
            }
        }
    }
    catch (exception &e){
        cout << e.what();
    }
    return 0;
}
