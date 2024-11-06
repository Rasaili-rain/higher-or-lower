# Instagram Followers Higher/Lower Game

A command-line game where players guess which Instagram personality has more followers. Test your knowledge of social media influencers and try to achieve the highest score!

## Features

- Interactive command-line interface with ASCII art
- Multiple game modes: Play, Add Questions, View Questions
- Persistent storage of questions and high scores
- Cross-platform compatibility (Windows and Linux)
- Dynamic question pool with support for adding new personalities

## Build Requirements

- C compiler (GCC recommended)
- Standard C libraries
- Windows or Linux operating system

## Building the Game

### Linux
```bash
gcc -o higher_lower main.c
```

### Windows
```bash
gcc -o higher_lower.exe main.c
```

## File Structure

- `data.txt` - Stores personality data (name and follower count)
- `high_score.txt` - Stores the all-time high score
- Main source file contains all game logic and utilities

## Gameplay

1. **Main Menu Options:**
   - Play Game (1)
   - Add Questions (2)
   - View Questions (3)
   - Exit (4)

2. **During Gameplay:**
   - Two personalities are randomly selected
   - Player must guess who has more followers
   - Score increases with each correct answer
   - Game ends on first wrong answer
   - Press 9 during a game to quit

## Adding New Questions

1. Select option 2 from the main menu
2. Enter the number of questions you want to add
3. For each question, provide:
   - Personality name
   - Number of followers (in millions)

## Data Storage Format

- `data.txt`: Each line contains a name and follower count
  ```
  Personality_Name Followers_In_Millions
  ```
- Names with spaces are stored with underscores
- Follower counts are stored as integers

## Technical Notes

- Maximum name length: 30 characters
- Maximum number of personalities: 100
- Demonstration mode available (set `demonstration = true` to show follower counts during gameplay)

## Error Handling

- File operation errors are handled gracefully
- Input validation for menu choices
- Duplicate question prevention during gameplay
- Empty question pool detection

## Contributing

To add more personalities to the default question pool:
1. Open `data.txt`
2. Add new entries in the format: `Name_Of_Person Follower_Count`
3. Ensure follower counts are in millions

## Tips

- Keep track of popular Instagram personalities and their follower counts
- The game becomes more challenging as more questions are added
- Regular updates to follower counts keep the game current
