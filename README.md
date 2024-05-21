# Hangman Game

This program allows the user to play Hangman. It imports phrases from a file called `phrases.txt`, categorizes them into easy, medium, and hard based on their position in the file, and lets the user guess the phrase. The game displays the phrase as a series of blanks and updates the display as the user guesses letters. If the user completes the phrase before the character is fully drawn on the gallows, they win. Otherwise, they lose.

## Features
- Reads phrases from a file and sorts them into difficulty levels.
- Displays the Hangman game interface with blanks and updates with each guess.
- Tracks incorrect guesses and draws the gallows incrementally.
- Allows the user to play multiple rounds.

## Requirements
- C++ compiler
- `phrases.txt` file with phrases for the game

## How to Play
1. Compile the program:
    ```sh
    g++ -o hangman hangman.cpp
    ```
2. Run the compiled program:
    ```sh
    ./hangman
    ```
3. Follow the on-screen instructions to play the game.

## Code Explanation

### Import Libraries
```cpp
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;

```
- iomanip: For input/output manipulation.
- fstream: For file input/output.
- iostream: For standard input/output.
- cctype: For character handling functions.
- ctime: For time-related functions.

### Structure Definition

```cpp
struct Phrase 
{
  string text;
  unsigned int guessesRequired;
  bool isUsed;
};

```

- Phrase: Structure to store a phrase, the number of unique letters it contains, and whether it has been used.

## Function Declarations

### Main Function
```cpp
int main()
{
  // Initialization and setup code
  Phrase phrases[200];
  int numPhrases = loadPhrasesFromFile("phrases.txt", phrases, 200);
  printPhrases(phrases, numPhrases);
  sortPhrases(phrases, numPhrases);
  printPhrases(phrases, numPhrases);

  int seed = time(nullptr);
  srand(seed);

  int count = numPhrases;
  int level = getDifficultyLevel();

  if(numPhrases == 0) {
    return 0; 
  }   

  int indexRandom = randomPhraseIndex(level, phrases, numPhrases);

  char playAgain = 'y';
  do {
    Phrase phrase = phrases[indexRandom];
    runGame(phrase);
    phrase.isUsed = true;
    count--;
    if(count != 0) {
      cout << "Would you like to play again? (y/n): ";
      cin >> playAgain;
    }
  } while(count > 0 && playAgain == 'y');

  return 0;
}

```
- Main function initializes the game, loads phrases, sorts them, and handles the game loop.

### Function Definitions
- drawGallows(): Displays the gallows based on the miss count.
- runGame(): Manages the gameplay for a single phrase.
- randomPhraseIndex(): Selects a random phrase based on difficulty.
- getDifficultyLevel(): Prompts the user to select a difficulty level.
- phraseWithBlanks(): Generates a string of blanks and guessed letters.
- printPhrases(): Displays the list of phrases.
- sortPhrases(): Sorts phrases by difficulty.
- loadPhrasesFromFile(): Loads phrases from the phrases.txt file.
- Other helper functions for sorting and counting unique letters.

## File Structure
- hangman.cpp: Main program file.
- phrases.txt: File containing phrases for the game.

## Contributing
Feel free to submit issues or pull requests if you have suggestions for improving this project.

## Liscence
This project is licensed under the MIT License. See the LICENSE file for more details.
