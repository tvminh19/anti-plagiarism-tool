#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct word{
    string str;
    int amount;
};

string handle(string& str){
    //lower case
    for (auto& e : str){
        if (isupper(e)){
            e = tolower(e);
        }
    }

    //remove symbol
    for (auto& e : str){
        if (e == '.' ||
            e == ',' || 
            e == '\'' || 
            e == '\"' ||
            e == '?' ||
            e == '!' ||
            e == '@' ||
            e == '*' ||
            e == '(' ||
            e == ')' ||
            e == '{' ||
            e == '}'){
                e = ' ';
            }
    }

    //remove duplicate space
    int pos = 0;
    while (pos < str.size() - 1){
        while (str[pos] == ' ' && str[pos + 1] == ' ' && pos < str.size() - 1){
            str.erase(pos, 1);
        }
        pos++;
    }

    //remove space two side
    while (str[0] == ' '){
        str.erase(0, 1);
    }

    pos = str.size() - 1;
    while (pos >= 0 && str[pos] == ' '){
        str.erase(pos, 1);
        pos--;
    }

    return str;
}

bool isInVt(string& x, vector<word>& vt){
    for (int i=0; i<vt.size(); ++i){
        if (x == vt[i].str){
            vt[i].amount++;
            return true;
        }
    }
    return false;
}

vector<word> strToWordVect(string str){
    vector<word> vt;

    while (str.size() != 0){
        string tmpStr = "";
        while (str.size() != 0 && str[0] != ' '){
            tmpStr += str[0];
            str.erase(0, 1);
        }
        if (str.size() != 0){
            str.erase(0, 1);
        }
        if (!isInVt(tmpStr, vt)){
            word tmpWord;
            tmpWord.amount = 1;
            tmpWord.str = tmpStr;
            vt.push_back(tmpWord);
        }
    }

    return vt;
}

vector<word> formatFile(const string file){
    vector<word> res;
    ifstream in;
    in.open(file);
    if (!in.is_open()){
        cout << "Unable to open file";
        return res;
    }
    else{
        //read content
        string content;
        content.assign(
            (istreambuf_iterator<char>(in)),
            (istreambuf_iterator<char>())
        );
        
        //format
        content = handle(content);

        //string to word vector
        res = strToWordVect(content);

    }
    in.close();
    return res;
}

int totalWord(vector<word>& res){
    int sum = 0;
    for (int i=0; i<res.size(); ++i){
        sum += res[i].amount;
    }
    return sum;
}

float compareString(int noOfFileNeedCheck, const string addRootFile){
    string fileName = "a" + to_string(noOfFileNeedCheck) + ".txt";
    vector<word> root = formatFile(addRootFile);
    vector<word> another = formatFile(fileName);

    // float percentPerWord = 100.0 / n * 1.0;
    float res = 0.0;

    int missingWord = 0;
    for (int i=0; i<another.size(); ++i){
        for (int j=0; j<root.size(); ++j){
            if (another[i].str == root[j].str){
                int k = another[i].amount - root[j].amount;
                if (k < 0){
                    res += another[i].amount / root[j].amount;
                }
                else if (k > 0){
                    int h = another[i].amount - 0.5 * k;
                    if (h > 0){
                        res += h / root[j].amount;
                    }
                }
                else{
                    res += 1;
                }
            }
            else if (j == root.size() - 1){
                missingWord++;
            }
        }
    }
    res -= 0.5 * missingWord;
    if (res < 0){
        res = 0.0;
    }
    return res * 100.0 / root.size();
}

int main(){
    cout << "Percentage: " << compareString(1, "root.txt") << "%" << endl;
    return 0;
}
