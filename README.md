# RedisClass
A working implementation of redis with some basic functionalities like
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

(2) further improvements can be made in the implementation of Zrank and Zrange function , we could design a binary indexed tree, segment tree or some other data structure to reduce time complexity per operation and of course we could extend the functionalities of RedisClass in other utilities.

(3) I have majorly used HashMaps(unordered_map in c++) and Balanced Binary search trees(set in c++) to store the data, since HashMaps gives us a fast way to access,insert,delete and search operation with O(1) time complexity per query, i used sets to store value where order was neccessary to maintain, sets are also very fast and does most of the operations in O(logN) time.
nothing specific is used apart from these two data structure, these HashMaps and sets were used as stand along or in nested fashion, all data structures were availed using STL(Standard Template Library) of c++.

(4) I am not very sure and clear about multithreading application of it, but surely we can create multiple threads of process and run them independently for this also, we have just created a class like to any other standard class which can be instantiated and could be used by threads.
