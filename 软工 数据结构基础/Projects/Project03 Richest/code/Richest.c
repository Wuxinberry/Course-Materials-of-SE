#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define maxn 100001
#define maxm 101
#define maxc 9

struct Richer {
	char name[maxc];
	int  age;
	int  netWorth;
}richers[maxn], tmpSort[maxm];
int N, K;

void Richest( int num, int ageMin, int ageMax, int caseNum );
int  Compare( struct Richer richer, int i );
void QuickSort( struct Richer* array, int low, int high );


/*
 * Part 1: Main Function
 * Read in Input -> Sort -> Answer Queries
 */
int main() {
	// Read in N, K
	int i;
	scanf("%d %d", &N, &K );
	// Read in Information of Richers
	for ( i = 0; i < N; i++ ) 
		scanf("%s %d %d", richers[i].name, &richers[i].age, &richers[i].netWorth);

	// Sort Richers
	QuickSort( richers, 0, N-1 );

	// Read in queries and Output
	for ( i = 0; i < K; i++ ) {
        // Read in specific Query
		int num, ageMin, ageMax;
		scanf("%d %d %d", &num, &ageMin, &ageMax );
        // Filtrate Richest
		Richest( num, ageMin, ageMax, i+1 );
	}

	return 0;
}


/*
 * Part 2: Quick Sort 
 * Sort richer by Quick Sort Algorithm
 */
void QuickSort( struct Richer* array, int low, int high ) {
    // Detect if sort finished
	if ( low >= high ) 
		return ;
	int i = low, j = high;
	struct Richer key = array[low]; // set as pivot
	while ( i < j ) {
        // search for the 1st number larger than key from right side
		while ( i < j && (Compare(key, j)))
			j--;
		if ( i < j ) 
			array[i++] = array[j];
        // search for the 1st number larger than key from left side
		while ( i < j && !Compare(key, i) ) 
			i++;
		if ( i < j ) 
			array[j--] = array[i];
	}
	array[i] = key; // place key at proper place
	QuickSort(array, low, i-1); // Sort numbers by pivot's left side
	QuickSort(array, i+1, high);// Sort numbers by pivot's right side
} 

int Compare( struct Richer richer, int i ) {
    // 1st, compare by net worth(wealthier)
	if ( richer.netWorth != richers[i].netWorth ) 
		return richer.netWorth > richers[i].netWorth;
    // 2nd, compare by age(younger)
	if ( richer.age != richers[i].age ) 
		return richer.age < richers[i].age;
    // 3rd, compare by name(alphabetic)
	if ( strcmp( richers[i].name, richer.name) > 0 ) 
		return 1;
	return 0;
}


/*
 * Part 3: Filtrate Richest
 * Select richer within suitable age -> Output
 */
void Richest( int num, int ageMin, int ageMax, int caseNum ) {
	int i, j = 0;
	// visit richers and select suitable ones
	for ( i = 0; i < N && j < num; i++ ) 
		if ( richers[i].age >= ageMin && richers[i].age <= ageMax ) {
			tmpSort[j] = richers[i];
			j++;
		}
	// Output
	printf("Case #%d:\n", caseNum);
    // if no one in the range of age
	if ( !j ) {
		printf("None");
		return ;	
	}
    // else, print information line by line
	for ( i = 0; i < num && i < j; i++ ) {
		printf("%s %d %d\n", tmpSort[i].name, tmpSort[i].age, tmpSort[i].netWorth);
	}

	return ;
}
