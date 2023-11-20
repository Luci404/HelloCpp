### Build
´´´
docker run --rm -it -v %cd%:/usr/src/project gcc:latest
cd /usr/src/project
g++ main.cpp -fcoroutines -std=c++20
´´´

### Resources
#### Basics
- https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
- https://www.incredibuild.com/blog/cpp-coroutines-lets-play-with-them
- https://itnext.io/c-20-coroutines-complete-guide-7c3fc08db89d (nice flow graph)

#### On Performance
- https://stackoverflow.com/questions/68533343/what-is-performance-penalty-for-converting-a-function-into-coroutine-in-c