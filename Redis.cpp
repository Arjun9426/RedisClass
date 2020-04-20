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
    unordered_map<string,set<pair<int,string>>>* key_set_bag;
    unordered_map<string,unordered_map<string,int>>* scores;
public :    
    Redis(){
        key_value_bag = new unordered_map<string,Data>;
        key_set_bag = new unordered_map<string,set<pair<int,string>>>;
        scores = new unordered_map<string,unordered_map<string,int>>;
    }
    ~Redis(){
        delete key_value_bag;
        delete key_set_bag;
        delete scores;
    }
    /*adding values to the set corresponding to key, values are pairwise different(unique values) but they may have equal score,
    values are ordered in set in increasing order of their scoresand ties are broken by lexographical order of value(as value is string).  
    time complexity: O(1){for hashmap element access} + O(log(n)){for insertion and deletion operation
    from corresponding set where n is size of set} */

    void Zadd(string key, int score, string value){
        if(key_set_bag->count(key)==0){ // corresponding to this key no set exists, therefore we will make one 
            set<pair<int,string>> temp1;
            temp1.insert(make_pair(score,value));
            key_set_bag->insert(make_pair(key,temp1));

            unordered_map<string,int> temp2;
            temp2[value]=score;
            scores->insert(make_pair(key,temp2));
        }
        else{
            if(scores->at(key).count(value)==0){ // when value is not present in set
                scores->at(key)[value]=score;
                key_set_bag->at(key).insert(make_pair(score,value));
            }
            else{// value is already present in set, therefore we will update set and corresponding score to this value
                int oldscore=scores->at(key)[value];
                key_set_bag->at(key).erase(make_pair(oldscore,value)); // deleting old entry(which is in form of pair)
                key_set_bag->at(key).insert(make_pair(score,value)); // inserting new entry
                scores->at(key)[value]=score; // updating score corresponding to value
            }
        }
    }

    /*deleting value from the set corresponding to key, in this case we need not to send score
    since that can be extracted from scores hashmap 
    time complexity: O(1){for hashmap element access} + O(log(n)){for deletion operation
    from  set where n is size of set} */ 
    void Zdel(string key, string value){ 
        if(key_set_bag->count(key)==1){
            if(scores->at(key).count(value)==1){
                int score=scores->at(key)[value];
                key_set_bag->at(key).erase(make_pair(score,value)); // deleting  value's presence from set
                scores->at(key).erase(value); // deleting value's presence from scores
            }
        }
    }
    /* it returns rank(position) of a value in a set 
    time complexity : O(N) where N is size of set*/
    int Zrank(string key, string value){
        if(key_set_bag->count(key)==1 && scores->at(key).count(value)==1){
            int rank=0;
            for(auto & Pair : key_set_bag->at(key)){
                if(Pair.second == value){
                    break;
                }
                rank++;
            }
            return rank;
        }
        else{
            cout<<"this combination of key and value doesn't exists\n";
            return -1;
        }
    }

    /*print all elements in range from start to end-1 i.e [st,end), in this implementation we have considered positive indices
    time complexity : O(N) N is size of set*/
    void Zrange(string key, int start, int end){
        if(key_set_bag->count(key)==0){
            return ;
        }
        int rank=0;
        for(auto & Pair : key_set_bag->at(key)){
            if(rank >= start && rank < end){
                cout<<Pair.second<<"\n";
            }
            rank++;
        }
    }
    // returns the size of a set corresponding to a key
    int Zsize(string key){
        if(key_set_bag->count(key)==0) return 0;
        else return (int)key_set_bag->at(key).size();
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
    // deletes all the key from the container(key_value_bag) effectively reducing it's size to zero
    void FlushAll(){
        cout<<"All data will be lost! if still want to proceed please enter 1 else 0\n";
        int temp;
        cin>>temp;
        if(temp==1){
            key_value_bag->clear();
            cout<<"All keys deleted!\n";
        }
    }
    // return size of container i.e number of keys in key_value_bag
    int size(){
        return (int)key_value_bag->size();
    }
    // print all key-value pairs from the container key_value_bag
    void PrintAll(){
        for(auto& Pair : *key_value_bag){
            cout<<Pair.first<<" "<<Pair.second.value<<'\n';
        }
    }
};
int main() {


    // random code to check functions

    Redis obj;
    srand(time(0));

    /* 
    //code to check efficiency
    for(int i=0;i<1000000;i++){
        string key="";
        for(int j=0;j<10;j++){
            key+=rand()%26+'a';
        }
        string value="";
        for(int j=0;j<10;j++){
            key+=rand()%26+'a';
        }
        obj.Zadd(key,rand()%(int)(1e9+7),value);
    }
    //passes in ~ 2 secs
    */

    /*
    //unstructured code for checking some functionalities
    obj.Zadd("arjun",1,"coding");
    obj.Zadd("arjun",2,"cricket");
    obj.Zadd("arjun",4,"movies");
    obj.Zadd("arjun",3,"excercises");
    obj.Zrange("arjun",0,obj.Zsize("arjun"));
    
    cout<<endl;
    
    obj.Zadd("arjun",1,"excercises"); 
    obj.Zrange("arjun",0,obj.Zsize("arjun"));
    cout<<obj.Zrank("arjun","dhqdwhdqhduqdhw")<<'\n';
    cout<<obj.Zrank("dhqduddqwdwqd","coding")<<'\n';
    cout<<obj.Zrank("arjun","coding")<<'\n';
    
    obj.Zdel("arjun","doijqdioiwhqdqw");
    obj.Zdel("arjun","movies");
    
    obj.Zrange("arjun",0,obj.Zsize("arjun"));
    
    obj.Set("a",1);
    obj.Set("b",4);
    obj.Set("c",3);
    obj.Rename("a","c");
    obj.FlushAll();
    obj.PrintAll();
    */

    /*
    //code to check TTL function and time related parameters this should be checked using running in terminal
    while(true){
        string s;
        cin>>s;
        if(s=="end") break;
        if(s=="get"){
            cout<<obj.TTL("arjun");
        }
    }*/
return 0;

}

