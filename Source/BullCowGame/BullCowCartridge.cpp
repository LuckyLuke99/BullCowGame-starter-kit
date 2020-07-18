// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(HWList);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }

    else // checking the PlayerGuess
    {
        ProcessGuess(Input);
    }
    
}

void UBullCowCartridge::SetupGame() // Configuration of HiddenWord, Lives and the arrays for the HelpWord
{
    HiddenWord = Isograms [FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() - 1;
    bGameOver = false;
    TCHAR NoneWord ;
    RandomCharacter.Empty();
    HelpWord.Empty();
    for (int32 Index = 0; Index < HiddenWord.Len(); Index++) // Making Array for HelpWord
    {
        HelpWord.Push(NoneWord);
        RandomCharacter.Push(Index);
    }

    // Welcome the Player
    PrintLine(TEXT("Welcome to Bull Cows"));
    PrintLine(TEXT("Guess the %i leller word!\nYou have %i lives"), HiddenWord.Len(), Lives);
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("The game is over!!\nPress enter to play again"));// Prompt To Play Again, Press Enter To Play Again?
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) // Check the guess
{
    //Check if Win
    if (HiddenWord == Guess)
    {
        PrintLine(TEXT("You have Won!!"));
        EndGame();
        return;
    }
    
    //Check the isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeting letters, guess again"));
        return;
    }

    // Check if player have lives left
    if (Lives < 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left, you lost!!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);

        EndGame();
        return;
    }

    // Check Right Number Of Characters
    if (Guess.Len() != HiddenWord.Len()) 
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guess again!"));// Prompt To Guess Again
        PrintLine(TEXT("You have %i lifes left"), Lives);// Show Lives Left
        return;
    }

    // Remove Life
    PrintLine(TEXT("You lost a life!"));
    --Lives;

    // Show the player Bulls, Cows and RandomCharacter
    RandomCharacter.RemoveAt(HWRandom());
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
    PrintLine(TEXT("The hidden word is: "));
    for (TCHAR TempVar : HelpWord)
    {
        PrintLine(TEXT("%c"), TempVar);
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const // Check if the word is isogram
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const // Check if the word is valid
{
    TArray<FString> ValidWords;
    for (FString TempVar : WordList)
    {
        if (TempVar.Len() >= 4 && TempVar.Len() <= 8 && IsIsogram(TempVar))
        {
            ValidWords.Emplace(TempVar);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const // Count the Bull and Cows
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HWIndex = 0; HWIndex < HiddenWord.Len(); HWIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HWIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

int32 UBullCowCartridge::HWRandom()
{
    int32 RandomNumber = FMath::RandRange(0, RandomCharacter.Num() - 1);
    HelpWord[RandomCharacter[RandomNumber]] = HiddenWord[RandomCharacter[RandomNumber]];
    PrintLine(TEXT("%i"), HelpWord.Num());
    return RandomNumber;
}
