#include <iostream>
void printBinary(int n)
{
	std::cout << "decimal: " << n << std::endl;
	std::cout << "binary : ";
	for (int i=31; i>=0; i--) 
	{
		int bit = ((n >> i) & 1);
		std::cout << bit;
	}
	std::cout << std::endl;
}
int main (int argc, char * const argv[]) {
    // insert code here...
	signed int a ;
	while(std::cin>>a){
		printBinary(a);
		printBinary(0xff);
		printBinary(a >> 8);
		printBinary(((a >> 8) & 0xff));
		printBinary(a >> 0);
		printBinary(((a >> 0) & 0xff));
		// print binary with leading zeros
		
		
	}
    return 0;
}
