#include <scoped_allocator>

int main() {
	char s1[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
	char s2[5];

	std::copy(s1, s1+2, s2);
	std::copy(s1 + 2, s1 + 2 + 5, s1);

	return 0;
}