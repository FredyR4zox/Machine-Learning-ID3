# Machine-Learning-ID3
Fourth project for the subject Artificial Inteligence, which consisted in implementing the ID3 algorithm for generating decision trees and all the data structures needed to generate an efficient tree.
This implementation has several functions to optimize the performance of the algorithm, by discretizing continuous values and converting strings to numbers for a faster comparation of values.
Our implementation was in C++.


### Compilation
``` bash
$ g++ -O3 main.cpp -o main
```

### Usage
``` bash
$ ./main <examples_file.csv>
```


### Files
* main.cpp - Request input, call the ID3 algorithm and discretize continues values.
* Node.h - Implementation of the tree nodes.
* Interval.h - Data structure to represent an interval.
* weather.csv, restaurant.csv, iris.csv - Files with examples to generate the decision tree.


### Authors
* Ana Rita Santos Lopes da Costa - up201605706 - [ritasantos11](https://www.github.com/ritasantos11)
* Frederico Emanuel Almeida Lopes - up201604674 - [FredyR4zox](https://www.github.com/FredyR4zox)
* João Lucas Faria de Pires - up201606617 - [iSynthx](https://www.github.com/iSynthx)
