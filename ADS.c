#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 100001

int count;
int wordLenght;
int maxWordLenght;
int firstRead;
char *wordToGuess;

//TYPES-----------------------------------------
//Generic Node:
typedef struct realNode
{
	struct realNode* next; //Used for list in hash Table
	struct realNode* nextWF; //Used to track words filtered
	char *word;
}Node;

Node *wordsFiltered;


typedef struct realNodeTP
{
	struct realNodeTP* left;
	struct realNodeTP* right;
	struct realNodeTP* p;
	char *word;
	unsigned short int color; //1 = black, 0 = red;
}NodeTP;

//RBT:
typedef struct realTreeRBT
{
	NodeTP *root;
	NodeTP *nill;
}TreeRBT;

//Type used to memorize for each possible character its characteristic (if it's present in this position, the list of characters not present in 
//this position, ecc...) 
typedef struct realSlot
{
	unsigned short int known;
	char c;
	unsigned short int charactersNA[64];
}Slot; 

//Type used to count the number of letters which I know are present in the word: when "limit" is 1 "num" contains the exact number of occurrences
//of that letter in the word
typedef struct realCounter
{
	int num;
	unsigned short int limit; //used as boolean
}Counter;

Counter counterCharacters[64];

//METHODS-----------------------------------------

void readWords(Node *words[], Slot *filterFEPosition, int readOutOFGame);
void compareWord(char* wordToGuess, char* readStr,Slot *filterFEPosition);
void setup(Slot *s, Counter *c);
int characterPosition(char c);
int filterWord(char* word, Slot *filterFEPosition);
void setupWF(Node* words[]);

int isPresent(Node* list, char* str);
int hash(char* str);

//RBT METHODS:
void deleteMoreElements(Node* words[], Slot *filterFEPosition);
void printInOrderTree(NodeTP *root);
void insertTreeRBT(TreeRBT *t, NodeTP *z);
void insertFixup(TreeRBT *t, NodeTP *z);
void rightRotate(TreeRBT *t, NodeTP *x);
void leftRotate(TreeRBT *t, NodeTP *x);

//MAIN-------------------------------------------------------------------------
int main()
{

	short int endProgram; //used when EOF is reached
	short int endGame; //used when a game ends
	char *readStr; //to read words;

	int numAttempts;
	int attemptsValue;


	//Variables to manage words:
	Node* words[DIM];
	TreeRBT *wordsToPrint;

	//SETUP
	endProgram = 0;
	endGame = 0;
	maxWordLenght = 0;
	count = 0;
	firstRead = 1;
	wordToGuess = NULL;

	//RBT:
	wordsToPrint = malloc(sizeof(TreeRBT)); 
	wordsFiltered = NULL;

	if(wordsToPrint)
	{
		wordsToPrint->nill = malloc(sizeof(NodeTP));
		if(wordsToPrint->nill)
		{
			wordsToPrint->nill->left = NULL;
			wordsToPrint->nill->right = NULL;
			wordsToPrint->nill->p = NULL;
			wordsToPrint->nill->word = NULL;
			wordsToPrint->nill->color = 1;
			wordsToPrint->root = wordsToPrint->nill;
		}
		else
		{
			endProgram = 1;
		}

	}
	else
	{
		endProgram=1;
	}

	//----------------------------------START----------------------------------
	//Read words' lenght:
	if(scanf("%d",&wordLenght)!=EOF)
	{
		if(wordLenght<17)
		{
			maxWordLenght = 17;
		}
		else
		{
			maxWordLenght = wordLenght;
		}
	}
	else
	{
		endProgram = 1;
	}

	//Setup variables used to filter words;
	Slot filterFEPosition[wordLenght];

	setup(filterFEPosition, counterCharacters); //Setup structeres used to filter words

	for(int i=0;i<DIM;i++)
	{
		words[i] = NULL;
	}

	//----------------------------------READ NOW ALL THE INITIAL WORDS---------------------------------------

	readWords(words, filterFEPosition,1);

	//----------------------------------ALL THE INITIAL WORDS HAVE BEEN READ----------------------------------
	//----------------------------------START A NEW GAME------------------------------------------------------

	wordToGuess = malloc(wordLenght + 1);
	//This "while" will end when the program ends
	while(endProgram == 0)
	{

		if(wordToGuess)
		{
			//specific word to guess which is contained in the list above
			if(scanf("%s", wordToGuess)!=EOF)
			{
				if(wordToGuess[0]=='+')
				{

				}
				//num of attempts
				if(scanf("%d", &numAttempts)!=EOF)
				{
					//------------------------------------------------------------------------------
					//THIS WHILE WILL MANAGE A SINGLE GAME
					attemptsValue = 0;
					while(endGame == 0)
					{
						readStr = malloc(maxWordLenght + 1);
						if(scanf("%s", readStr)!=EOF)
						{
							if(strcmp(readStr, "+inserisci_inizio"))
							{
								if(strcmp(readStr, "+stampa_filtrate"))
								{
									//the string is not "+inserisci_inizio" and it's not "+stampa_filtrate" so:
									//normal check now:
									//We compare readStr with wordToGuess:

									//We check if readStr is in the list:
									if(isPresent(words[hash(readStr)], readStr))
									{
										//We check if readStr is equal to wordToGuess:
										if(strcmp(wordToGuess, readStr))
										{
											//The words are different, so we have to check each character:
											compareWord(wordToGuess, readStr, filterFEPosition);
											count = 0;
											//Mehtod to delete words in filteredWords according to constrains just learned:
											deleteMoreElements(words, filterFEPosition);

											//We have to print here how many words now satisfy the constrains (counting wordToGuess!!!!) 
											printf("%d\n", count);

											//We increment attempts: "attempts++;"
											attemptsValue++;
											if(attemptsValue==numAttempts)
											{
												printf("ko\n");
												endGame = 1;
											}
										}
										else
										{
											printf("ok\n");
											endGame = 1;
										}
									}
									else
									{
										printf("not_exists\n");
									}
								}
								else
								{
									//built tree:
									Node* copy = wordsFiltered;
									NodeTP* toInsert;
									while(copy)
									{
										toInsert = malloc(sizeof(NodeTP));
										if(toInsert)
										{
											toInsert->word = copy->word;
											insertTreeRBT(wordsToPrint, toInsert);
										}
										copy = copy->nextWF;
									}

									//print tree:
									printInOrderTree(wordsToPrint->root);
									wordsToPrint->root = wordsToPrint->nill;
								}
							}
							else
							{
								readWords(words, filterFEPosition, 0);
							}

							//Anyway we will have to free memory:
							free(readStr);

						}
						else
						{
							endProgram = 1;
							endGame = 1;
						}
					}
					//------------------------------------------------------------------------------

					setup(filterFEPosition, counterCharacters); //Setup structeres used to filter words
					wordsFiltered = NULL;

					//NOW A NEW GAME CAN START AND/OR NEW WORDS CAN BE ADDED TO THE LIST:
					readStr = malloc(maxWordLenght + 1);
					if(scanf("%s", readStr)!=EOF)
					{
						if(strcmp(readStr, "+inserisci_inizio"))
						{
							//We have "+nuova_partita":
							endGame = 0;
							free(readStr);
							setupWF(words);
						}
						else
						{
							//insert new words
							readWords(words, filterFEPosition, 1);
							free(readStr);

							//Now we can read "+nuova_partita" or end the program
							readStr = malloc(maxWordLenght + 1);
							if(!endProgram)
							{
								if(scanf("%s", readStr)==EOF)
								{
									endProgram = 1;
								}
								else
								{
									endGame = 0;
									setupWF(words);
								}
							}
							free(readStr);
						}
					}
					else
					{
						endProgram = 1;
						endGame = 1;
					}
				}
				else
				{
					endProgram = 1;
				}
			}
			else
			{
				endProgram = 1;
			}
		}
		else
		{
			endProgram = 1;
		}
	}

	return 0;
}


//----------------------------------IMPLEMENTED METHODS---------------------------------------

void readWords(Node *words[], Slot *filterFEPosition, int readOutOFGame)
{
	int checkEndRead; //To check when I find "+nuova_partita" or "+inserisci_fine"
	char *maxReadStr; //to read words;
	int pos;

	checkEndRead = 1;
	while(checkEndRead)
	{
		maxReadStr = malloc(maxWordLenght + 1);
		if(maxReadStr)
		{
			if(scanf("%s", maxReadStr)!=EOF)
			{
				if(maxReadStr[0]=='+')
				{
					if(strcmp(maxReadStr, "+inserisci_inizio"))
					{
						checkEndRead = 0;
					}
					else
					{
						readWords(words, filterFEPosition, readOutOFGame);
					}

					//free(maxReadStr);
				}
				else
				{

					Node* temp = malloc(sizeof(Node));
					temp->word = maxReadStr;

					if(readOutOFGame)
					{
						pos = hash(maxReadStr);
						temp->next = words[pos];
						words[pos] = temp;

						if(firstRead)
						{
							temp->nextWF = wordsFiltered;
							wordsFiltered = temp;
						}
					}
					else
					{

						if(filterWord(maxReadStr, filterFEPosition))
						{
							temp->nextWF = wordsFiltered;
							wordsFiltered = temp;
						}

						pos = hash(maxReadStr);
						temp->next = words[pos];
						words[pos] = temp;
					}
				}
			}
			else
			{
				checkEndRead = 0;
			}
		}
		else
		{
			checkEndRead = 0;
		}
	}

	firstRead = 0;
}

//-------------------------------------------------------------------------------------------
void compareWord(char* wordToGuess, char* readStr,Slot *filterFEPosition)
{
	int i;
	int help;

	//used to count which and how many letters are not in the right place in the right place: we count che characters of wordToGuess
	short int characters[64] = {0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0};

	//used to memorize which and how many letters
	short int counter[64] = {0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0};


	char ris[wordLenght+1];
	//START:

	//spot '+':
	i=0;
	for(; i<wordLenght; i++)
	{
		if(wordToGuess[i]==readStr[i])
		{
			ris[i] = '+';
			//We have to remember that in this position we have this letter:
			filterFEPosition[i].known = 1;
			filterFEPosition[i].c = wordToGuess[i];
			//Deallocate memory in filterFEPosition[i].listLNA:
		}
		else
		{
			//count letters which are not in the right place
			characters[characterPosition(wordToGuess[i])]++;

			ris[i] = '!';
		}
	}

	//Check now the word:
	i=0;
	for (; i < wordLenght; i++)
	{
		help = characterPosition(readStr[i]);
		if(ris[i]!='+')
		{
			if(characters[help] > 0) //Check if the number of occurences of this letter in wordToGuess is sufficient
			{
				ris[i] = '|';
				characters[help]--;

				//It means that we can't find this letter in this position:
				filterFEPosition[i].charactersNA[help] = 1;

				//We now count it:
				counter[help]++;
			}
			else
			{
				ris[i]='/';
				counterCharacters[help].limit = 1;
				filterFEPosition[i].charactersNA[help] = 1;
			}
		}
		else
		{
			counter[help]++;
		}
	}

	i=0;
	for(; i<wordLenght; i++)
	{
		//We save now what we have learned:
		help = characterPosition(readStr[i]);
		if(counter[help] > counterCharacters[help].num)
		{
			counterCharacters[help].num = counter[help];
		}
	}

	ris[wordLenght] = '\0';

	printf("%s\n", ris);

}


//---------------------------------------------------------------

//between 0 - 63
int characterPosition(char c)
{
	int ris;
	if(c>='0' && c<='9')
	{
		ris = c - 48;
	}
	else
	{
		if(c>='A' && c<='Z')
		{
			ris = c - 55;
		}
		else
		{
			if(c>='a' && c<='z')
			{
				ris = c - 61;
			}
			else
			{
				if(c == '-')
				{
					ris = 62;
				}
				else
				{
					// character: '_'
					ris = 63;
				}
			}
		}
	}

	return ris;
}



//---------------------------------------------------------------
//This method is used to check if a word satisfys the constraints gotted till now
int filterWord(char* word, Slot *filterFEPosition)
{
	int valid = 1; //we consider the word valid
	int i;
	int pos;
	Counter helper;
	int counter[64] = {0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0};


	//valid = 1;


	//Check letters known:
	i=wordLenght-1;
	for(; valid && i>=0; i--)
	{
		pos = characterPosition(word[i]);
		if(filterFEPosition[i].known)
		{
			if(word[i] != filterFEPosition[i].c)
			{
				valid = 0;
			}
		}
		else
		{
			//if the letter is not known, we check if this letter cannot be in this position:
			if(filterFEPosition[i].charactersNA[pos]) //checkLetter(filterFEPosition[i].listLNA,word[i])
			{
				//This letter cannot be in this position:
				valid = 0;
			}

		}

		counter[pos]++;
	}

	for(i=0; valid && i<wordLenght;i++)
	{
		pos = characterPosition(word[i]);
		helper = counterCharacters[pos];
		if(helper.limit)
		{
			if(helper.num!=counter[pos])
			{
				valid = 0;
			}
		}
		else
		{
			if(counter[pos]<helper.num)
			{
				valid = 0;
			}
		}
	}



	for(i=0;valid && i<wordLenght;i++)
	{
		pos = characterPosition(wordToGuess[i]);
		helper = counterCharacters[pos];

		if(helper.limit)
		{
			if(helper.num!=counter[pos])
			{
				valid = 0;
			}
		}
		else
		{
			if(counter[pos]<helper.num)
			{
				valid = 0;
			}
		}
	}

	return valid;
}


void deleteMoreElements(Node* words[], Slot *filterFEPosition)
{

	Node* new;
	new = NULL;
	
	//Use list wordsFiltered to filter words:
	Node *copy,*temp;
	copy = wordsFiltered;

	while(copy)
	{
		if(filterWord(copy->word, filterFEPosition))
		{
			temp = copy ->nextWF;
			copy->nextWF = new;
			new = copy;
			copy = temp;

			count++;
		}
		else
		{
			copy = copy->nextWF;
		}
	}

	wordsFiltered = new;
}

void printInOrderTree(NodeTP *root)
{
	
	if(root && root->word)
	{
		printInOrderTree(root->left);
		printf("%s\n", root->word);
		printInOrderTree(root->right);

		free(root);
	}
}

void insertTreeRBT(TreeRBT *t, NodeTP *z)
{
	NodeTP *y, *x;

	y = t -> nill;
	x = t-> root;

	while(x!=t->nill)
	{
		y = x;
		if(strcmp(z->word, x->word)<0)
		{
			x = x -> left;
		}
		else
		{
			x = x -> right;
		}
	}

	z -> p = y;

	if(y == t->nill)
	{
		t -> root = z;
	}
	else
	{
		if(strcmp(z->word, y->word)<0)
		{
			y -> left = z;
		}
		else
		{
			y -> right = z;
		}
	}

	z -> left = t->nill;
	z -> right = t->nill;
	z->color = 0;

	insertFixup(t, z);

}


void insertFixup(TreeRBT *t, NodeTP *z)
{
	while(z->p->color==0)
	{
		if(z -> p==z->p->p->left)
		{
			NodeTP *y;
			y = z->p->p->right;

			if(y -> color == 0)
			{
				z->p->color = 1;
				y->color = 1;
				z->p->p->color = 0;
				z = z->p->p;
			}
			else
			{
				if(z==z->p->right)
				{
					z = z->p;
					leftRotate(t, z);
				}

				z->p->color = 1;
				z->p->p->color = 0;
				rightRotate(t, z->p->p);
			}
		}
		else
		{
			NodeTP *y;
			y = z->p->p->left;
			if(y->color == 0)
			{
				z->p->color = 1;
				y -> color = 1;
				z -> p->p ->color = 0;
				z = z -> p -> p;
			}
			else
			{
				if(z == z->p->left)
				{
					z = z->p;
					rightRotate(t, z);
				}

				z->p->color = 1;
				z->p->p->color = 0;
				leftRotate(t,z->p->p);
			}
		}
	}

	t->root->color = 1;
}

void leftRotate(TreeRBT *t, NodeTP *x)
{
	NodeTP *y;
	y = x -> right;
	x -> right = y -> left;
	if(y -> left != t ->nill)
	{
		y -> left -> p = x;
	}

	y -> p = x -> p;
	if(x -> p == t -> nill)
	{
		t -> root = y;
	}
	else
	{
		if(x == x->p->left)
		{
			x->p->left = y;
		}
		else
		{
			x -> p-> right = y;
		}
	}

	y -> left = x;
	x -> p = y;
}


void rightRotate(TreeRBT *t, NodeTP *x)
{
	NodeTP *y;

	y = x -> left;
	x-> left = y -> right;
	if(y -> right!=t->nill)
	{
		y -> right -> p = x;
	}

	y -> p = x -> p;

	if(x -> p == t -> nill)
	{
		t -> root = y;
	}
	else
	{
		if(x == x->p->right)
		{
			x->p->right = y;	
		}
		else
		{
			x->p->left = y;
		}
	}

	y -> right = x;
	x -> p = y;
}


int isPresent(Node* list, char* str)
{
	while(list)
	{
		if(strcmp(str, list->word)==0)
		{
			return 1;
		}

		list = list->next;
	}

	return 0;
}

int hash(char* str)
{
	int g = 3;
	long ris = 0;

	for(int i=0;i<wordLenght; i++)
	{
		ris = (ris * g) + str[i];
	}

	return (ris % DIM);
}

void setupWF(Node* words[])
{
	Node* temp;
	Node* new;
	new = NULL;

	for(int i=0; i<DIM; i++)
	{
		temp = words[i];

		while(temp)
		{
			temp->nextWF = new;
			new = temp;

			temp = temp->next;
		}
	}

	wordsFiltered = new;
}

//---------------------------------------------------------------
//Setup structures used to filter words
void setup(Slot *s, Counter *c)
{
	int i;
	i=0;

	for(; i<wordLenght; i++)
	{
		s[i].known = 0;
		for(int j=0; j<64; j++)
		{
			s[i].charactersNA[j]=0;
		}
	}

	i=0;
	for(; i<64; i++)
	{
		c[i].num = 0;
		c[i].limit = 0;
	}
}

