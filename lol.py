import random 
import time
import math
import curses 
import subprocess

def oob(y, x):
    if y < 0:
        y = 0
    elif y >= 200:
        y = 200
    elif x < 0:
        x = 0
    elif x >= 200:
        x = 200
    return y, x

def main(): 
    stdscr = curses.initscr()
    stdscr.clear()
    curses.curs_set(0)
    
    with open('wordlist.txt', 'r') as f:
        words = f.readlines()
    
    words = [word.strip('\n') for word in words]
    curWord = random.sample(words, 1)
    yAxis = [0 for i in range(10)]
    
    start_time = time.time()
    
    stdscr.addstr(0,0, "Do you want to change the wordlist? (y/n): ")
    stdscr.refresh()
    change_wordlist = stdscr.getstr().decode('utf-8')
    
    if change_wordlist == 'y':
        curses.endwin()  # Temporarily end curses to use the terminal normally
        subprocess.run(['open', '-a', 'CotEditor', 'wordlist.txt'])
        stdscr = curses.initscr()  # Re-initialize curses after CotEditor is closed
    elif change_wordlist == 'n':
        pass
    
    if change_wordlist == 'y':
        subprocess.run(['nano', 'wordlist.txt'])
    elif change_wordlist == 'n':
        pass
    
    while True:
        positions = [20*i for i in range(len(curWord))]  # Reset positions

        # Move a random word one position down
        randIndex = random.randint(0, len(curWord) - 1)
        if time.time() - start_time > 5:
            yAxis[randIndex] += 1
        elif time.time() - start_time > 10:
            yAxis[randIndex] += 2
        elif time.time() - start_time > 15:
            yAxis[randIndex] += 3
        elif time.time() - start_time > 20:
            yAxis[randIndex] += 4
        elif time.time() - start_time > 25:
            yAxis[randIndex] += 5

        # Check if any word has reached the bottom of the screen
        height, width = stdscr.getmaxyx()
        if any(y >= height for y in yAxis):
            break

        for i, word in enumerate(curWord):
            stdscr.addstr(yAxis[i], positions[i], word)
        
        stdscr.refresh()
        
        # Get user input as a string
        stdscr.addstr(10, 0, "Enter a word: ")
        userInput = stdscr.getstr().decode('utf-8')

        if userInput in curWord:
            index = curWord.index(userInput)
            stdscr.addstr(yAxis[index], positions[index], ' ' * len(userInput))  # Clear the word
            curWord.pop(index)
            yAxis.pop(index)
            positions.pop(index)
            
            # Only add a new random word if there are less than 10 words
            if len(curWord) < 10:
                newWord = random.choice(words)
                curWord.append(newWord)
                yAxis.append(0)  # New word starts at the top
                positions.append(20 * (len(curWord) - 1))  # Position for the new word
            
            # If more than 5 seconds have passed and there are less than 10 words, add another new word
            if time.time() - start_time > 5 and len(curWord) < 10:
                additionalWord = random.choice(words)
                curWord.append(additionalWord)
                yAxis.append(0)  # Additional new word also starts at the top
                positions.append(20 * (len(curWord) - 1))  # Position for the additional new word

        stdscr.refresh()
        stdscr.getch()  # Wait for user input before continuing

        stdscr.clear()
        
    
main()
        
        