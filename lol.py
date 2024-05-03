import random 
import time
import math
import curses 
import subprocess
from PIL import Image

# Check if out of bounds - this is used nowhere lol
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

def adjust_positions(curWord, positions, width, yAxis):
    for i in range(len(curWord)):
        if positions[i] + len(curWord[i]) > width:
            positions[i] = width - len(curWord[i])
            yAxis[i] = 0

# Check if the new position overlaps with any existing positions
def check_overlap(new_position, existing_positions, word_length):
    """
    Check if the new position overlaps with any existing positions, ensuring there's at least
    one space between words.
    
    :param new_position: The proposed x-coordinate for the new word.
    :param existing_positions: List of tuples containing the positions and lengths of existing words.
    :param word_length: The length of the new word.
    :return: True if there is an overlap, False otherwise.
    """
    for pos, length in existing_positions:
        # Expand the range by 1 on both sides to ensure a space between words
        if (pos - 1) <= new_position < (pos + length + 1) or (pos - 1) < (new_position + word_length + 1) <= (pos + length + 1):
            return True
    return False

# Main fucntion
def main(): 
    stdscr = curses.initscr()
    stdscr.clear()
    curses.curs_set(0)
    
    # Open wordlist.txt and read the words into a list
    with open('wordlist.txt', 'r') as f:
        words = f.readlines()
    
    words = [word.strip('\n') for word in words]
    curWord = random.sample(words, 1)
    yAxis = [0 for i in range(10)]
    
    # Get the start time
    start_time = time.time()
    
    # Check if the user wants to change the wordlist
    stdscr.addstr(0,0, "Do you want to change the wordlist? (y/n): ")
    stdscr.refresh()
    change_wordlist = stdscr.getstr().decode('utf-8')
    
    # If the user wants to change the wordlist, open the list in CotEditor
    if change_wordlist == 'y':
        curses.endwin()  # Temporarily end curses to use the terminal normally
        subprocess.run(['open', '-a', 'CotEditor', 'wordlist.txt'])
        stdscr = curses.initscr()  # Re-initialize curses after CotEditor is closed
    elif change_wordlist == 'n':
        pass
    
    stdscr.clear()
    # Get the difficulty level from the user
    stdscr.addstr(0,0, "Choose a difficulty level (1-10): ")
    difficulty = int(stdscr.getstr().decode('utf-8'))
    stdscr.clear()
    height, width = stdscr.getmaxyx()
    
    # Main game loop
    while True:
        positions = [random.randint(0, width - len(curWord[i])) for i in range(len(curWord))]  # Adjusted line
        
        # Adjust the loop where positions are assigned
        existing_positions = [(positions[i], len(curWord[i])) for i in range(len(curWord))]

        # Check if the new position of the word overlaps with the position of any existing words
        for i in range(len(curWord)):
            new_position = random.randint(0, width - len(curWord[i]))
            # Keep generating a new position until it doesn't overlap
            while check_overlap(new_position, existing_positions, len(curWord[i])):
                new_position = random.randint(0, width - len(curWord[i]))
            positions[i] = new_position
            existing_positions[i] = (new_position, len(curWord[i]))  # Update the existing positions list
    
        # Move a random word one position down
        randIndices = random.sample(range(len(curWord)), len(curWord))
        if len(curWord) >= difficulty:
            randIndices = random.sample(range(len(curWord)), difficulty)
        # make it so that it won't reach the state where the program fails, because the user will be fucked before then
        if time.time() -start_time > 30:
            difficulty = 20
        # Increase the difficulty after a certain length of time by increasing the distance the words move down each iteration
        for randIndex in randIndices:
            if time.time() - start_time > 5:
                yAxis[randIndex] += 2
            elif time.time() - start_time > 10:
                yAxis[randIndex] += 4
                yAxis[randIndex] += 4
            elif time.time() - start_time > 15:
                yAxis[randIndex] += 6
                yAxis[randIndex] += 6
            elif time.time() - start_time > 20:
                yAxis[randIndex] += 8
                yAxis[randIndex] += 8
            elif time.time() - start_time > 25:
                yAxis[randIndex] += 10
                yAxis[randIndex] += 10
            elif time.time() - start_time > 30:
                yAxis[randIndex] += 12
                yAxis[randIndex] += 12

        # Check if any word has reached the bottom of the screen
        height, width = stdscr.getmaxyx()
        if any(y >= height for y in yAxis):
            break

        # Print the words on the screen
        for i, word in enumerate(curWord):
            stdscr.addstr(yAxis[i], positions[i], word)
        
        stdscr.refresh()
        
        # Get user input as a string
        stdscr.addstr(height-1, 0, "Enter a word: ")
        userInput = stdscr.getstr().decode('utf-8')
        
        # Vim keybinds lol
        if userInput == ':q':
            break

        # Check if the user input matches any of the words on the screen
        if userInput in curWord:
            index = curWord.index(userInput)
            stdscr.addstr(yAxis[index], positions[index], ' ' * len(userInput))  # Clear the word
            curWord.pop(index)
            yAxis.pop(index)
            positions.pop(index)
            
            
            # Only add a new random word if there are less than 10 words


            
            # If more than 5 seconds have passed and there are less than 25 words, add another new word
            # So the maximum number of words on the screen is 25
            if time.time() - start_time > 5 and len(curWord) < 10:
                additionalWord = random.choice(words)
                curWord.append(additionalWord)
                yAxis.append(0)  # Additional new word also starts at the top
                positions.append(20 * (len(curWord) - 1))  # Position for the additional new word
         
            # if the user input matches a word, add a new word       
            newWord = random.choice(words)
            curWord.append(newWord)
            yAxis.append(0)
            positions.append(random.randint(0,width-len(newWord)))

        stdscr.refresh()
        stdscr.getch()  # Wait for user input before continuing

        stdscr.clear()
    
    # Clear the screen after the game ends
    stdscr.clear()
    # Print the time taken, and print game over
    stdscr.addstr(1, 0, "Time taken: {:.2f} seconds".format(time.time() - start_time))    
    stdscr.addstr(0, 0, "Game Over!")
    stdscr.refresh()
    # Sleep for 5 seconds before closing the window, ending the program for good :)
    time.sleep(5)
    curses.endwin() 
    
# Run the main function
main()
        
        