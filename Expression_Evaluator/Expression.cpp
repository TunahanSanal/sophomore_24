#include <iostream>
#include <string>	//String kütüphanesi
#include <cctype>	//Karakter kütüphanesi
#include <stack>	//Stackk kütüphanesi

using namespace std;

int islemonceligi(char karakter) {
    if (karakter == '+') return 1;		//Çarpma ve bölme daima toplama ve çýkarmadan önceliklidir.
    else if (karakter == '*') return 2;	
    else if (karakter == '-') return 1;
    else return 2;
    
    return 0;
}

string Infix2Postfix(const string& s) {	//Ýnfixten postfixe dönüþüm
	
    stack<char> elements;	//Ýþlem elemanlarý için yeni yýðýn yapýsý
    string p_fix; 

    for (char c : s) 
	{	if (isdigit(c)) {	//Eðer karakter sayýysa
            p_fix = p_fix + c;	//Direkt çýktýya ekle
            p_fix = p_fix + ' ';
            
        } else if (c == '(') {	//Karakter açýk parantez ise bu kýsmý yýðýna atýyor 
        
            elements.push(c);
        } else if (c == ')') {
        	
            while (!elements.empty() && elements.top() != '(') {	//Parantez yoksa ve yýðýn boþ deðilse 	
                p_fix = p_fix + elements.top(); 	//Elemanlar toplanýr
                p_fix = p_fix + ' ';	//Araya boþluklar koyulur
                elements.pop();	//Yýðýndaki en üstteki elemaný çýkarýyor
                
            }
            elements.pop();	//Açýk parantez iþaretini pop fonksiyonuyla yýðýndan at
            
        } else {
            while (!elements.empty() && islemonceligi(elements.top()) >= islemonceligi(c)) {	//Ýþlem önceliði göz önüne alýnarak yýðýn operasyonlarý yapan döngü
            	
                p_fix = p_fix + elements.top();
                p_fix = p_fix + ' ';
                elements.pop();
            }
            elements.push(c);
        }
    }

    while (!elements.empty()) {	//Yýðýn boþ duruma gelinceye kadar 
        p_fix = p_fix + elements.top();	//Tüm elemanlarý postfix'e ekle
        p_fix = p_fix + ' ';
        elements.pop();
    }

    return p_fix;	//Postfix ifadesini döndür
}

int EvaluatePostfixExpression(const string& s) {	//Postfix'i deðerlendirme fonksiyonu
	
    stack<int> deger;

    for (char c : s) {
        if (isdigit(c)) {
            deger.push(c - '0');	//Rakamlarý direkt olarak "deger" yýðýnýna ekle
        } else {
    
            int sag = deger.top(); deger.pop();	//Ýþlem operatöreriyle karþýlaþtýðýn zaman
            int sol = deger.top(); deger.pop();	//Ýki adet deðer al

            switch (c) {	//Matematiksel 4 temel iþlemi seçilen durumlara göre uygula
            
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

    return deger.top();	//Elde edilen yýðýnýn en üst kýsmý yani son elemaný sonucumuzu verir
}
