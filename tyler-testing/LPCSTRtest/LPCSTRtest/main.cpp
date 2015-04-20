#include <Windows.h>
#include <string>

LPCSTR lpTest;
std::string sTest;

void main() {
	lpTest = "nopers";
	sTest = "nopers";
	if(sTest == lpTest) {
		printf("it works\n");
	}
}