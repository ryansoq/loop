set -x

if clang-format --version &> /dev/null
then
   clang-format -style=WebKit main.cc > temp && cat temp > main.cc && rm temp
fi

g++ -std=c++11 main.cc -g -O0 -o loop
