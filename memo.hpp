#ifndef BOOKSTORM_MEMO_HPP
#define BOOKSTORM_MEMO_HPP
#include <fstream>
#include <vector>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;




template<class T>
class Memo{
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int indexmax=5;
    std::vector<int>deleted;
public:
    Memo()=default;
    ~Memo(){
        file.open(file_name);
        file.write(reinterpret_cast<char*>(&indexmax),sizeof(int));
        file.close();
    }
    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name);//todo:delete out
        if(!file){
            file.open(file_name,std::ios::out);indexmax=5;
            file.write(reinterpret_cast<char*>(&indexmax),sizeof(int));
        }else{
            file.seekp(0,std::ios::beg);
            file.read(reinterpret_cast<char*>(&indexmax),sizeof(int));//indexmax赋值回到上一次的最后位置
        }
        file.close();
    }
    int write(T &t) {//write down ,return index
        file.open(file_name);
        if(!deleted.empty()){
            int dex=deleted.back();
            file.seekp(dex,std::ios::beg);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
            deleted.pop_back();
            return dex;
        }
        indexmax+=sizeofT;
        file.seekp(indexmax,std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return indexmax;
    }
    void update(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    void read(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    void Delete(int index) {
        deleted.push_back(index);
    }
};

#endif //BOOKSTORM_MEMO_HPP
