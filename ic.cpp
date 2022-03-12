#include <iostream>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

double letterFrequency[26] = { 8.12, 1.49, 2.71, 4.32, 12.02, 2.30, 2.03, 5.92, 7.31, 0.10, 0.69, 3.98, 2.61, 6.95,
    7.68, 1.82, 0.11, 6.02, 6.28, 9.10, 2.88, 1.11, 2.09, 0.17, 2.11, 0.07 };

string uppercase(string a) {
    for (int i = 0; i < a.size(); i++)
        if (a[i] >= 'a' && a[i] <= 'z')
            a[i] = a[i] - 'a' + 'A';
    return a;
}

string filter(string message) {
    for (int i = 0; i < message.length(); i++) {
        if (message[i] >= 'a' && message[i] <= 'z')
            message[i] = message[i] - 'a' + 'A';
        else if (message[i] < 'A' || message[i] > 'Z'){
            message.erase(i, 1);
            i--;
        }
    }
    return message;
}

// message and key are UPPERCASE
string encrypt(string message, string key) {
    string encryptedMessage = message;
    int messageLength = message.length();
    int keyLength = key.length();
    for (int i = 0; i < messageLength; i++) {
        encryptedMessage[i] = (message[i] - 'A' +  key[i % keyLength] - 'A') % 26 + 'A';
    }
    return encryptedMessage;
}

// message and key are UPPERCASE
string decrypt(string message, string key) {
    string decryptedMessage = message;
    int messageLength = message.length();
    int keyLength = key.length();
    for (int i = 0; i < messageLength; i++) {
        decryptedMessage[i] = (message[i] - key[i % keyLength] + 26) % 26 + 'a';
    }
    return decryptedMessage;
}

// s is UPPERCASE
double indexOfCoincidence(string s) {
    int fr[26];
    double ic = 0.0;
    int n = s.length();
    for (int i = 0; i < 26; i++)
        fr[i] = 0;
    for (int i = 0; i < n; i++) 
        fr[s[i] - 'A']++;
    for (int i = 0; i < 26; i++) 
        ic += ((double)fr[i] * ((double)fr[i] - 1.0));
    
    ic = ic / ((double)n * ((double)n - 1.0));

    return ic;
}

// obtains y_m,i : selects characters from the position k and then all the characters from that position plus m 
// s is UPPERCASE
string groupingMessage(int m, int k, string s) {
    int n = s.length();
    string result;
    int n1 = 0;
    int i = k;
    while (i < n) {
        result += s[i];
        i += m;
    }
    return result;
}

double avg(double a[], int n) {
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += a[i];
    sum = sum / (double)n;
    return sum;
}

// s is UPPERCASE, position represents the number of shifts of a letter in the alphabet
string shiftAlphabet(string s, int position) {
    int n = s.length();
    for (int i = 0; i < n; i++)
        s[i] = (s[i] - 'A' + position) % 26 + 'A' ;
    return s;
}

// s is UPPERCASE
double mutualIndexOfCoincidence(string s) {
    double mic = 0;
    int fr[26];
    int n = s.length();

    for (int i = 0; i < 26; i++)
        fr[i] = 0;

    for (int i = 0; i < n; i++)
        fr[s[i] - 'A']++;

    for (int i = 0; i < 26; i++)
        mic += ((letterFrequency[i] / 100) * (double)fr[i]);

    mic = mic / (double)(n);
    
    return mic;
}

// s is UPPERCASE
string findKey(string s, int keyLength) {
    string key;
    for (int j = 0; j < keyLength; j++) {
        double minDifference = 1.0;
        int indexKeyPart = 0;

        for (int i = 0; i < 26; i++) {
            double val = abs(mutualIndexOfCoincidence(groupingMessage(keyLength, j, shiftAlphabet(s, i))) - 0.065);
            if (val < minDifference) {
                minDifference = val;
                indexKeyPart = i;
            }
        }
        key += ( 26 - indexKeyPart ) % 26 + 'A';
    }
    return key;
}

// s is UPPERCASE
int keyLength(string s) {
    int m = 0;
    bool ok = 0;
    double ic[20];
    do {
        m++;
        ok = 1;
        for (int i = 0; i < m; i++) {
            ic[i] = indexOfCoincidence(groupingMessage(m, i, s));
        }
        if (abs(avg(ic, m) - 0.065) > 0.0035)
            ok = 0;
    } while (ok == 0);
    
    return m;
}

string generateRandomKey() {
    string key;
    srand(time(NULL));
    int n = 2 + rand() % 19;
    for (int i = 0; i < n; i++)
        key += rand() % 26 + 'A';
    return key;
}

int main()
{
    ifstream fin("input.txt");

    string s;

    getline(fin, s);
    fin.close();

    s = filter(s); // filtrare fisier initial

    string key;
    int option = 0;
    cout << "Alegeti o optiune:\n1.Introduceti o cheie\n2.Generati o cheie oarecare\n";
    cin >> option;

    if (option == 1) {
        cout << "Introduceti o cheie: ";
        cin >> key;
        key = uppercase(key);
    }
    else if (option == 2) {
        key = generateRandomKey();
        cout << "Cheia generata: " << key << endl;
    }
    else {
        cout << "optiune invalida; vom folosi cheia prestabilita \'ABC\'" << endl;
        key = "ABC";
    }

    string encryptedS = encrypt(s, key); // criptarea mesajului s
    cout << "\nMesajul criptat: \n" << encryptedS << endl << endl;

    int sKeyLength = keyLength(encryptedS); // gasirea lungimii cheii
    string sKey = findKey(encryptedS, sKeyLength);  // gasirea cheii
    cout << "Mesajul decriptat:\n" << decrypt(encryptedS, sKey) << endl << endl;
    cout << "Lungimea cheii: " << sKeyLength << endl;  
    cout << "Cheia: " << sKey << endl; 
}

