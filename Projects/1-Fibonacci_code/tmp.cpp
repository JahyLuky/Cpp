#include<iostream>
#include<climits>
int main(){
int a = 0, b = 1;
std::cout << a << ", " << b << ", ";
while (a <= INT_MAX - b) {
    int temp = b;
    b += a;
    a = temp;
    std::cout << b << ", ";
}
return 0;
}
