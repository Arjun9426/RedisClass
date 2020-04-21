# RedisClass
A working implementation of redis with some functionalities like
(1) Get
(2) Set
(3) Expire
(4) Zadd
(5) Zrank
(6) Zrange
plus some more helpful functionalities like:
Persists, TTl, Rename, Zdel, Incr, Decr, FlushAll, size, PrintAll, Exists, Modify,Zsize

Report:
(1) I chose c++ because it is one of the most fastest language which support object oriented programming and multithreading.

(2) Further improvements can be made in the implementation of Zrank and Zrange function , we could design a binary indexed tree, segment tree or some other data structure to reduce time complexity per operation and of course we could extend the functionalities of RedisClass to achieve other utilities.

(3) I have majorly used HashMaps(unordered_map in c++) and Balanced Binary search trees(set in c++) to store the data, since HashMaps gives us a fast way to access,insert,delete and search operation with O(1) time complexity per query, i used sets to store value where order was neccessary to maintain, sets are also very fast and does most of the operations in O(logN) time.
nothing specific is used apart from these two data structure, these HashMaps and sets were used as stand along or in nested fashion, all data structures were availed using STL(Standard Template Library) of c++. 
EFFICIENCY OF CLASS: This c++ implementation is quite efficient for eg: we can perform around one million operations with key_value_bag and one million with key_set_bag(performing mostly Zadd and Zdelete) in just 2 seconds.

(4) Multithreading makes process more efficient using parallel computation of different part of complete process for eg: in a mobile application we generally do not use main thread for all processes as some IO processes are processed on different threads for that we need to maintain concurrency and should resolve any concurrency/Critical-Section issues if any, in this implementation of RedisClass i have not used any concurrency resolution method as i don't have experience of those techniques and standards, this is a simple class which can be instantiated and could be used as Redis for performing some utilities.
