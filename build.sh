clang -std=c++11 -c *.cpp
ld -o snake -macosx_version_min 10.14.0 -allow_stack_execute -no_pie -lcurses -lstdc++ -lc++ -lSystem *.o
#clang -Wl,-allow_stack_execute -Wl,-no_pie main.cpp -o main -lcurses
