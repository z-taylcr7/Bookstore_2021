//
// Created by Cristiano on 2021/12/18.
//

#ifndef BOOKSTORAGE_CBP_MEMORY_HPP
#define BOOKSTORAGE_CBP_MEMORY_HPP
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
    bool initialise(string FN = "") {
        if (FN != "") file_name = FN;
        bool x;
        file.open(file_name);//todo:delete out
        if(!file){
            x=true;
            file.open(file_name,std::ios::out);indexmax=5;
            file.write(reinterpret_cast<char*>(&indexmax),sizeof(int));
        }else{
            x=false;
            file.seekp(0,std::ios::beg);
            file.read(reinterpret_cast<char*>(&indexmax),sizeof(int));//indexmax赋值回到上一次的最后位置
        }
        file.close();
        return x;
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
    void updateData(T &t, T&n) {
        file.open(file_name);
        int d=indexmax;
        while(d>5){
            T tmp;
            read(tmp,d);
            if(tmp==t)
            {
                file.seekp(d);
                file.write(reinterpret_cast<char *>(&n), sizeofT);
            }
            d-=sizeofT;
        }

        file.close();
    }
    void read(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }
    int find(const T &t){
        file.open(file_name);
        int d=indexmax;
        while(d>5){
            T tmp;
            read(tmp,d);
            if(tmp==t)return d;
            d-=sizeofT;
        }
        return 0;
    }

    int getIndexMax() const {
        return indexmax;
    }

    void Delete(int index) {
        deleted.push_back(index);
    }
};

#endif //BOOKSTORAGE_CBP_MEMORY_HPP
