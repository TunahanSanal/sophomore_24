#include <iostream>
#include <string>	//String k�t�phanesi
#include <cctype>	//Karakter k�t�phanesi
#include <stack>	//Stackk k�t�phanesi

using namespace std;

int islemonceligi(char karakter) {
    if (karakter == '+') return 1;		//�arpma ve b�lme daima toplama ve ��karmadan �nceliklidir.
    else if (karakter == '*') return 2;	
    else if (karakter == '-') return 1;
    else return 2;
    
    return 0;
}

string Infix2Postfix(const string& s) {	//�nfixten postfixe d�n���m
	
    stack<char> elements;	//��lem elemanlar� i�in yeni y���n yap�s�
    string p_fix; 

    for (char c : s) 
	{	if (isdigit(c)) {	//E�er karakter say�ysa
            p_fix = p_fix + c;	//Direkt ��kt�ya ekle
            p_fix = p_fix + ' ';
            
        } else if (c == '(') {	//Karakter a��k parantez ise bu k�sm� y���na at�yor 
        
            elements.push(c);
        } else if (c == ')') {
        	
            while (!elements.empty() && elements.top() != '(') {	//Parantez yoksa ve y���n bo� de�ilse 	
                p_fix = p_fix + elements.top(); 	//Elemanlar toplan�r
                p_fix = p_fix + ' ';	//Araya bo�luklar koyulur
                elements.pop();	//Y���ndaki en �stteki eleman� ��kar�yor
                
            }
            elements.pop();	//A��k parantez i�aretini pop fonksiyonuyla y���ndan at
            
        } else {
            while (!elements.empty() && islemonceligi(elements.top()) >= islemonceligi(c)) {	//��lem �nceli�i g�z �n�ne al�narak y���n operasyonlar� yapan d�ng�
            	
                p_fix = p_fix + elements.top();
                p_fix = p_fix + ' ';
                elements.pop();
            }
            elements.push(c);
        }
    }

    while (!elements.empty()) {	//Y���n bo� duruma gelinceye kadar 
        p_fix = p_fix + elements.top();	//T�m elemanlar� postfix'e ekle
        p_fix = p_fix + ' ';
        elements.pop();
    }

    return p_fix;	//Postfix ifadesini d�nd�r
}

int EvaluatePostfixExpression(const string& s) {	//Postfix'i de�erlendirme fonksiyonu
	
    stack<int> deger;

    for (char c : s) {
        if (isdigit(c)) {
            deger.push(c - '0');	//Rakamlar� direkt olarak "deger" y���n�na ekle
        } else {
    
            int sag = deger.top(); deger.pop();	//��lem operat�reriyle kar��la�t���n zaman
            int sol = deger.top(); deger.pop();	//�ki adet de�er al

            switch (c) {	//Matematiksel 4 temel i�lemi se�ilen durumlara g�re uygula
            
                case '+': 
				          deger.push(sol + sag); 
				                          break;
                case '*': 
						  deger.push(sol * sag); 
				                          break;
                case '-': 
						  deger.push(sol - sag); 
				                          break;
                case '/': 
			              deger.push(sol / sag); 
				                          break;
            }
        }
    }

    return deger.top();	//Elde edilen y���n�n en �st k�sm� yani son eleman� sonucumuzu verir
}
