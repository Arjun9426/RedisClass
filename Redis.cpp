#include <bits/stdc++.h>
using namespace std;

class Data{
public:
    int value;
    bool isexpirable;
    time_t creationTime;
    int duration;
    Data(int value, bool isexpirable, int duration){
        this->value=value;
        this->isexpirable=isexpirable;
        this->creationTime=time(0);
        this->duration=duration;
    }
    void updateExpirability(bool isexpirable, int duration){
        this->isexpirable=isexpirable;
        this->creationTime=time(0);
        this->duration=duration;
    }
};

class Redis{
private :
    unordered_map<string,Data>* key_value_bag;

public :    
    Redis(){
        key_value_bag=new unordered_map<string,Data>;
    }

    // remove key if it is expirable and have exceeded it's duration
    void Modify(string key){
        if(key_value_bag->count(key)==1 && key_value_bag->at(key).isexpirable){
            int time_elapsed_so_far = difftime(time(0),key_value_bag->at(key).creationTime);
            if(time_elapsed_so_far > key_value_bag->at(key).duration){
                key_value_bag->erase(key);
            }
        }
    }
    // return value corresponding to the key
    int Get(string key){
        Modify(key);
        if(key_value_bag->count(key)==1){
            return key_value_bag->at(key).value; // value returned if key exists
        }
        else{
            cout<<"key doesn't exists!\n";
            return -1; // -1 returned if key doesn't exists
        }
    }
    // Set a key-Data pair
    void Set(string key, int value, bool isexpirable=false, int duration=-1){
        key_value_bag->erase(key); // old key-value pair with same key will be deleted if it is present already
        key_value_bag->insert(make_pair(key,Data(value,isexpirable,duration)));
        cout<<"settled successfully \n";
    }
    // make a key expirable with some duration
    void Expire(string key, int duration){
        Modify(key);
        if(key_value_bag->count(key)==0){
            cout<<"Key doesn't exists\n"; 
        }
        else{
            key_value_bag->at(key).updateExpirability(true,duration);
        }
    }
    // make a key persistent(non-expirable)
    void Persist(string key){
        Modify(key);
        if(key_value_bag->count(key)==0){
            cout<<"Key doesn't exists\n"; 
        }
        else{
            key_value_bag->at(key).updateExpirability(false,-1);
        }
    }

    // TTL stands for Time To Leave means after this time provided key will be deleted from the container
    int TTL(string key){
        Modify(key);
        if(key_value_bag->count(key)==0){
            cout<<"Key doesn't exists!\n";
            return -1;
        }
        else if(!key_value_bag->at(key).isexpirable){
            cout<<"Key is not expirable!\n";
            return -1;
        }
        else{
            int time_elapsed_so_far = difftime(time(0),key_value_bag->at(key).creationTime);
            return key_value_bag->at(key).duration - time_elapsed_so_far;
        }
    }

    //  A key with name oldKey is renamed with name newKey
    void Rename(string oldKey, string newKey){
        Modify(oldKey);
        Modify(newKey);
        if(key_value_bag->count(oldKey)==0){
            cout<<"Key doesn't exists!\n";
        }
        else{
            auto temp=key_value_bag->at(oldKey);
            key_value_bag->erase(oldKey);
            key_value_bag->erase(newKey);
            key_value_bag->insert(make_pair(newKey,temp));
        }
    }
    // just deletes a key
    void Del(string key){
        key_value_bag->erase(key);
    }
    // return true if a  key exists else returns false
    bool Exists(string key){
        Modify(key);
        return key_value_bag->count(key)==1;
    }
    // if no increament value is given increase value of key by 1 else increase by the provided amount
    void Incr(string key, int val=1){
        Modify(key);
        if(key_value_bag->count(key)==0){
            cout<<"Key doesn't exists!\n";
        }
        else{
            key_value_bag->at(key).value+=val;
        }
    }
    // if no decreament value is given decrease value of key by 1 else decrease by the provided amount
    void Decr(string key, int val=1){
        Modify(key);
        if(key_value_bag->count(key)==0){
            cout<<"Key doesn't exists!\n";
        }
        else{
            key_value_bag->at(key).value-=val;
        }
    }
    // deletes all the key from the container effectively reducing it's size to zero
    void FlushAll(){
        cout<<"All data will be lost! if still want to proceed please enter 1 else 0\n";
        int temp;
        cin>>temp;
        if(temp==1){
            key_value_bag->clear();
            cout<<"All keys deleted!\n";
        }
    }
    // return size of container i.e number of keys
    int size(){
        return (int)key_value_bag->size();
    }
    // print all key-value pairs from the container
    void printAll(){
        for(auto& Pair : *key_value_bag){
            cout<<Pair.first<<" "<<Pair.second.value<<'\n';
        }
    }
};
int main() {

    Redis obj;
    obj.Set("a",1);
    obj.Set("b",4);
    obj.Set("c",3);
    obj.Rename("a","c");
    obj.FlushAll();
    obj.printAll();
    return 0;
    while(true){
        string s;
        cin>>s;
        if(s=="end") break;
        if(s=="get"){
            cout<<obj.TTL("arjun");
        }
    }

return 0;

}
