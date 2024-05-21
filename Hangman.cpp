/*
 * A program that allows the user to play hangman.
 * This program imports phrases from a file called:
 * "phrases.txt". Then sorts them into easy, medium,
 * hard based on if they are in the 1st, 2nd, or 3rd,
 * third of the file. Then it outputs the phrase as a line
 * of blanks, and the user can guess the phrase. Once the character
 * on the screen is fully drawn in the gallows, the player is dead.
 * If the player guesses the phrase without dying, they win.
 *
 * by Remley Hooker
 */

//libraries used in this project
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;

//struct Phrase with three components
struct Phrase 
{
  string text;
  unsigned int guessesRequired;
  bool isUsed;
};

/**
 * Displays the gallows. As the miss count increases, more of the person is
 * displayed hanging from the gallows.
 */
void drawGallows(int missCount);

//Runs the game. Input is a phrase which we then guess 
//components and determine cout statements to user. 
void runGame(Phrase phrase);

//Pulls a random index for the phrase based on the 
// level of difficulty.
int randomPhraseIndex(const int difficulty, 
  const Phrase phrases[], const int LENGTH);

// asks a user to input a level of difficulty.
int getDifficultyLevel();

//puts underscores for each character in the phrase
string phraseWithBlanks(const string text, const string guessedLetters);

//used in part I to display output for testing purposes.
void printPhrases(const Phrase phrases[], int length);

//sorts phrases by difficulty - smallest/easiest to hardest
void sortPhrases(Phrase phrases[], int length);

//used by sortPhrases to find the index of the smallest guessesRequired
int minlocation(const Phrase phrases[], int first, int last);

//used in sortphrases to chnage location of the smallest component
void swap(Phrase phrases[], int first, int second);

// used to find how many different letters are in the phrase
int uniqueLetterCount(const string text);

// gets phrases from a file called phrases.txt
int loadPhrasesFromFile(const string fileName, Phrase phrases[], int length);

int main()
{
  Phrase phrases[200];
  int numPhrases = loadPhrasesFromFile("phrases.txt", phrases, 200);
  printPhrases(phrases, numPhrases);
  sortPhrases(phrases, numPhrases);
  printPhrases(phrases, numPhrases);
  int seed = time(nullptr);
  int count; 
  srand(seed);  //seeding the random number generator

  count = numPhrases; //counting down to zero

  int level =  getDifficultyLevel();
  
  //leave game so that you don't have to divide by zero
  if(numPhrases == 0)
  {
    return 0; 
  }   
  int indexRandom = randomPhraseIndex(level, phrases, numPhrases);

  // allows player to play again
  char playAgain = 'y';
  do{
 
    Phrase phrase = phrases[indexRandom];
    runGame(phrase);
    phrase.isUsed = true;
    count--;
    if(count != 0)
    {
      cout << "Would you like to play again? (y/n): ";
      cin >> playAgain;
    }

    //while there are phrases in the file
  }while(count > 0 && playAgain == 'y');
  
  return 0;
}

void drawGallows(int missCount)
{
  // Output the top of the gallows
  cout
    << "   +----+     \n"
    << "   |    |     \n";

  // Output the stand and body
  switch (missCount)
  {
  case 0:
    cout
      << "   |       \n"
      << "   |       \n"
      << "   |       \n"
      << "   |       \n";
    break;
  case 1:
    cout
      << "   |    O  \n"
      << "   |    |  \n"
      << "   |       \n"
      << "   |       \n";
    break;
  case 2:
    cout
      << "   |    O  \n"
      << "   |   /|  \n"
      << "   |       \n"
      << "   |       \n";
    break;
  case 3:
    // The '\\' will translate as \, because it is a special char
    cout
      << "   |    O   \n"
      << "   |   /|\\ \n"
      << "   |        \n"
      << "   |        \n";
    break;
  case 4:
    cout
      << "   |    O   \n"
      << "   |   /|\\ \n"
      << "   |     \\ \n"
      << "   |        \n";
    break;
  default:
    cout
      << "   |    O    \n"
      << "   |   /|\\  \n"
      << "   |   / \\  \n"
      << "   |You're Dead\n";
  }

  // Output the base
  cout << "  =============\n" << endl;
}

int uniqueLetterCount(const string text)
{
  int length = text.length();
  
  //since we pass in a constant we have to make a new  one to lower case it
  string lowerText =  text; 
  string found = "";
  for (int index = 0; index < length; index++){
    lowerText[index] = tolower(lowerText[index]);
    if(found.find(lowerText[index]) == string::npos &&
      isalpha(lowerText[index]) != 0) 
      
      //its not already found 
      //and its a letter 
    {
      found = found + lowerText[index];
    }
  }
  unsigned int counter = found.length(); 
  return counter;
}

int loadPhrasesFromFile( const string fileName, Phrase phrases[], int length)
{
  // open the file
  ifstream file;
  file.open(fileName);
  int count = 0;

  //while it is not the end of the file, read in the lines
  while (!file.eof() && count < length)
  {
    getline(file, phrases[count].text);
    phrases[count].guessesRequired = uniqueLetterCount(phrases[count].text);
    phrases[count].isUsed = false;
    count++;
  }

  file.close();

  return count;
}

//selection sort pg 1191 and 1192
int minlocation(const Phrase phrases[], int first, int last)
{
  int loc;
  int minIndex;

  minIndex = first;

  for (loc = first + 1; loc <= last; loc++)
    if (phrases[loc].guessesRequired < phrases[minIndex].guessesRequired)
    minIndex = loc;

  return minIndex;

}

//Selection sort pg1192 1193
void swap( Phrase phrases[], int first, int second)
{
  // int loc, minIndex; 

  Phrase temp;
  temp = phrases[first];
  phrases[first] = phrases[second];
  phrases[second] = temp;
}

void sortPhrases(Phrase phrases[], int length)
{
  int loc, minIndex;

  for (loc = 0; loc < length; loc++){
    minIndex = minlocation(phrases, loc, length - 1);
    swap(phrases, loc, minIndex);
  }
  
}

void printPhrases(const Phrase phrases[], int length)
{
  // print the header
  cout << "  # Phrase                                           State" << endl;

  for (int index = 0; index < length; index++){
    string text = phrases[index].text;
    unsigned int guess = phrases[index].guessesRequired;
    bool isUsed = phrases[index].isUsed;
    int number;
    string state; 
    if (isUsed == 0)
    {
      state = "unused";
      number = 6;
    }
    else
    {
      state = "used";
      number = 4;
    }
    // formatting
    cout << fixed << showpoint;
    cout << " " << setw(2) << right << guess << " " << left << setw(49) << text 
      << left << setw(number) << state << endl;
    
  } 
  
}

string phraseWithBlanks(const string text, const string guessedLetters)
{
  int lengthText = text.length();
  int guessLength = guessedLetters.length();
  string toReturn = "";

  // for the length of the phrase
  for (int index = 0; index < lengthText; index++)
  {
    char letter = text[index];
    char lowerLetter = tolower(letter);

    int result = isalpha(lowerLetter);
  
    if (result == 0)
    {
      toReturn += letter;
      toReturn += ' '; //add spaces to output
    }
    else
    {
      bool isGuessed = false;
      for (int indexTwo = 0; indexTwo < guessLength; indexTwo++){
        char letterTwo = guessedLetters[indexTwo];
        char lowerLetterTwo = tolower(letterTwo);
        if (lowerLetter == lowerLetterTwo)
        {
          toReturn += letter;
          toReturn += ' '; //add spaces to output
          isGuessed = true;
        }
      }
  
      if(!isGuessed)
        toReturn += "_ ";//add spaces to output
    }
  }
  int almostlastSpace = toReturn.length();

  //get rid of the last space at the end of the output
  toReturn.erase(almostlastSpace - 1);
  return toReturn;
}

int getDifficultyLevel()
{
  int difficulty;
  bool done = false;
  cout << "Difficulty levels:" << "\n    1  Easy" << "\n    2  Medium" 
    << "\n    3  Hard";
  cout << "\nPick a difficulty level (1 to 3): \n";
  cin >> difficulty;
  while (!done)
  {
    if(difficulty == 3 || difficulty == 2 || difficulty == 1)
    {
      done = true;
    }
    else if(!cin)
    { 
      cin.clear();
      cin.ignore(100 , '\n');
      cout << "Please choose a difficulty level between 1 and 3: \n";

      cin >> difficulty;
    }
    else 
    {
      cout << "Please choose a difficulty level between 1 and 3: \n";
      cin >> difficulty;
    }
  }
  return difficulty - 1; //per the instructions
}

int randomPhraseIndex(const int difficulty, const Phrase phrases[],
  const int LENGTH)
{
  int easymax =  LENGTH / 3;

  //start the minimum at one more than easy max
  int mediummin = easymax; 
  
  //start mediummax at twice as long as the easy max
  int mediummax = 2 * easymax; 

  //minus  one because of the array boundaries
  int hardmax = LENGTH - mediummax;
  
  bool isUsed = true;
  int random;

  do
  {

    if (difficulty == 0)
    { //these are off by one per the instructions
      random = rand() % easymax;  // 0 to easy max works for indices
      isUsed = phrases[random].isUsed;
    }
    else if(difficulty == 1)
    {
      random = rand() % easymax + mediummin;
      isUsed = phrases[random].isUsed;
    }
    else
    {
      random = rand() % hardmax + mediummax;
      isUsed = phrases[random].isUsed;
    }
  }
  while(isUsed);
  return random;
}

void runGame(Phrase phrase)
{
  unsigned int guesses = 0;
  int missCount = 0;
  string lettersGuessed = "";
  string incorrectGuess = "";
  char guessChar;
  string phraseLower = "";
  for(string::size_type position = 0; position < phrase.text.length(); 
  position++)
  {
    if(isalpha(phrase.text[position]))
      phraseLower += tolower(phrase.text[position]);
    else
      phraseLower += phrase.text[position];
  }
  
  do {
    drawGallows(missCount);
    cout << phraseWithBlanks(phrase.text, lettersGuessed) << endl;
    cout << "Previous incorrect guesses: "; 
    for (string::size_type index = 0; index < incorrectGuess.length(); index++)
    {
      cout << incorrectGuess[index];
    }
    cout << "\nEnter guess: ";
    char guess;
    cin >> guess;
    for(int lines = 0; lines < 20; lines++)
      cout << endl;
    guessChar = tolower(guess);
    
    //cout << endl;
    bool found = false;
    
    //test for invalid entry
    if (!isalpha(guess))
    {
      cout << "\'" << guess << "\' is not a valid guess.";
      cout << "Please enter a letter.";
      found = true;
    }
    else{
      if(lettersGuessed.find(guessChar) != string::npos)
      {
        cout << "You have already guessed an \'" << guessChar << "\'." << endl;
        found = true;
      } 
      if(incorrectGuess.find(guessChar) != string::npos)
      {
        cout << "You have already guessed an \'" << guessChar << "\'." << endl;
        found = true;
      }
      if(phraseLower.find(guessChar) != string::npos && !found)
      {
        cout << "Good guess!" << endl;
        lettersGuessed += guessChar;
        guesses++;
        found = true;
      }
    }
    if(!found)
    {
      cout << "Sorry, bad guess." << endl;
      incorrectGuess += guessChar; //bot wants lower case
      missCount++;
    }
    
  }while(missCount < 5 && guesses < phrase.guessesRequired);

  if(missCount >= 5)
  {
    drawGallows(missCount);
    cout << phraseWithBlanks(phrase.text, lettersGuessed) << endl;
    string output;
    cout << "You're Dead! The phrase was:\n\"";
    for (string::size_type i3 = 0; i3 < phrase.text.length(); i3++)
    {
      output += phrase.text[i3];
      output += " ";
    }
    int almostlastSpace = output.length();
    
    //get rid of the last space at the end of the output so the bot is happy
    output.erase(almostlastSpace - 1);
    cout << output;
    cout << "\"" << endl;
  }
  else
  {
    drawGallows(missCount);
    cout << phraseWithBlanks(phrase.text, lettersGuessed) << endl;
    cout << "You Win!" << endl;
  }
}