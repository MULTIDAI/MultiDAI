# KDTree

KD-Tree and multiple adaptive indexing methods.

- points are given as STL vectors (and inserted in their own STL vector) so supports n-dimensional points for any n
- supports range queries

The following primary make targets are available:
    make run\_experiments: Compiles and runs the experiments specified in experiments.cpp
    make run\_test: Compiles and runs the testsuite, tests are specified in Test.cpp
 

Use test framework: https://github.com/Snaipe/Criterion
    Install:
        $ sudo add-apt-repository ppa:snaipewastaken/ppa
        $ sudo apt-get update
        $ sudo apt-get install criterion-dev



## License and copyright

The static KDTree is from J. Frederico Carvalho.

© J. Frederico Carvalho
Licensed under the [BSD3 License](LICENSE)
