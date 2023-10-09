#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_ROWS = 13;
const int MIN_ROWS = 4;
const int MAX_COLUMNS = 16;
const int MIN_COLUMNS = 4;
const int MAX_PLAYERS = 7;
const int MIN_PLAYERS = 3;

struct Board {
public:
  int rows;
  int columns;
  char **gameBoard;
};

struct Player {
public:
  string fullName;
  char playerChoice;
  char playerPiece;
  int playerLosses;
  int playerWins;
  int draws;
};

struct CaseInfo {
  bool isUppercase;
};

////////////////////////
// Function prototypes //
///////////////////////
bool isValidName(const string &str);
string rightAlign(const string &text, int width);
Board getBoardSize();
void assignPlayerPieces(Player players[], int numberOfPlayers);
void initializeGameBoard(Board &board);
void displayBoard(const Board &board);
bool isValidMove(const Board &board, int move);
void updateGameBoard(Board &board, int row, int col, char playerPiece);
void displayGameStatistics(const Player players[], int numberOfPlayers,
                           int totalGamesPlayed);
bool checkForDraw(const Board &board);
bool checkForWin(Board &board, int row, int col, char playerPiece);
void updateWinningConfiguration(Board &board, int row, int col,
                                char playerPiece);
void updateToUppercase(const Board &board, int startRow, int startCol,
                       int endRow, int endCol, char playerPiece);
char toUpperCase(char c);
int getUserMove(const string &playerName, char playerPiece, const Board &board);
void customSort(Player players[], int numberOfPlayers);
string trim(const string &str);

/////////
// MAIN //
////////
int main() {
  int startingPlayerIndex =
      2; // Initialize starting player index as 2 (third player)
  int numberOfPlayersRequested;
  int totalGamesPlayed = 0;
  string input;
  bool startingOff = true;
  bool inputValid = false; // Flag for input validation

  cout << "How many players would like to play? ";

  while (true) {
    getline(cin, input);
    input = trim(input);
    cout << "\n";
    if (cin.fail() || input.find_first_not_of("0123456789") != string::npos) {
      cin.clear();
      cout << "Incorrect Value! Please enter a valid value(Between 3 and 7): ";
    } else if (input.empty()) {
      cout << "Please enter a value for the number of players (Between 3 and 7): ";
    } else {
      numberOfPlayersRequested = stoi(input);

      if (numberOfPlayersRequested >= MIN_PLAYERS && numberOfPlayersRequested <= MAX_PLAYERS) {
        break;
      } else {
        cout << "Incorrect Value! Please enter a valid value(Between 3 and 7): ";
      }
    }
  }

  Player *person = new Player[numberOfPlayersRequested];

  for (int i = 0; i < numberOfPlayersRequested; i++) {
    cout << "Enter Player #" << i + 1 << ": ";

    do {
      getline(cin, person[i].fullName);
      person[i].fullName = trim(person[i].fullName);
      cout << "\n";
      if (!isValidName(person[i].fullName)) {
        cout << "Invalid name! Name must only contain letters (i.e a, b, c ...) Please try again: ";
      }
    } while (!isValidName(person[i].fullName));

    person[i].playerPiece = 'a' + i;

    if (i < numberOfPlayersRequested - 1) {
      cout << "Enter the next player's name:\n";
    }
  }

  assignPlayerPieces(person, numberOfPlayersRequested);

  customSort(person, numberOfPlayersRequested);

  Board boardSize;

  cout << "Choose your board size: \n\n";
  boardSize = getBoardSize();

  initializeGameBoard(boardSize);

  cout << "\nBoard size: " << boardSize.rows << " rows x " << boardSize.columns
       << " columns\n\n";

  bool playAgain = true;

  while (playAgain) {
    // Game loop
    int currentPlayerIndex = startingPlayerIndex;
    bool gameFinished = false;

    while (!gameFinished) {
      Player &currentPlayer = person[currentPlayerIndex];

      displayBoard(boardSize);

      cout << "\n";
      int move = getUserMove(currentPlayer.fullName, currentPlayer.playerPiece,
                             boardSize);

      if (isValidMove(boardSize, move)) {
        int row = (move - 1) / boardSize.columns;
        int col = (move - 1) % boardSize.columns;

        updateGameBoard(boardSize, row, col, currentPlayer.playerPiece);

        cout << "\n";

        // displayBoard(boardSize);

        if (checkForWin(boardSize, row, col, currentPlayer.playerPiece)) {
          currentPlayer.playerWins++;
          totalGamesPlayed++;

          currentPlayer.playerPiece = toupper(currentPlayer.playerPiece);
          displayBoard(boardSize);
          // Update losses for opponents
          for (int i = 0; i < numberOfPlayersRequested; i++) {
            if (i != currentPlayerIndex) {
              person[i].playerLosses++;
            }
          }
          cout << "\n";
          cout << currentPlayer.fullName << " wins!" << endl;
          gameFinished = true;
        } else if (checkForDraw(boardSize)) {
          currentPlayer.draws++;
          cout << "It's a draw!" << endl;
          gameFinished = true;
        }
        currentPlayer.playerPiece = tolower(currentPlayer.playerPiece);
      }

      if (!gameFinished) {
        currentPlayerIndex =
            (currentPlayerIndex + 1) % numberOfPlayersRequested;
      }
    }
    // Display game statistics for each player
    displayGameStatistics(person, numberOfPlayersRequested, totalGamesPlayed);

    // Ask if the players want to play again
    cout << "Do you want to play again? (y/n): ";
    cin.ignore();

    string playAgainInput;

    getline(cin, playAgainInput);
    playAgainInput = trim(playAgainInput);

    cout << "\n";

    while (playAgainInput != "y" && playAgainInput != "Y" &&
           playAgainInput != "n" && playAgainInput != "N") {
      cout << "Invalid input! Please enter 'y' to play again or 'n' to quit: ";
      getline(cin, playAgainInput);
      playAgainInput = trim(playAgainInput);
      cout << "\n";
    }

    if (playAgainInput != "y" && playAgainInput != "Y") {
      playAgain = false;
      cout << "\nThank you for playing My game!!!\n";
      break;
    }

    startingPlayerIndex = (startingPlayerIndex + 1) % numberOfPlayersRequested;
    playAgain = true;

    // Ask if the players want to change the board size
    cout << "Do you want to change the board size? (y/n): ";

    string changeSizeInput;
    bool changeBoardSize = false;

    getline(cin, changeSizeInput);
    changeSizeInput = trim(changeSizeInput);

    cout << "\n";

    while (changeSizeInput != "y" && changeSizeInput != "Y" &&
           changeSizeInput != "n" && changeSizeInput != "N") {
      cout << "Invalid input! Please enter 'y' to change the board size or 'n' "
              "to keep the same size: ";
      getline(cin, changeSizeInput);
      changeSizeInput = trim(changeSizeInput);
    }

    if (changeSizeInput == "y" || changeSizeInput == "Y") {
      // Get new board size
      boardSize = getBoardSize();
      initializeGameBoard(boardSize);
      changeBoardSize = true; // Set the flag to indicate board size change
    } else {
      // Reset the existing board only if the user chose not to change the size
      if (!changeBoardSize) {
        initializeGameBoard(boardSize);
      }
    }
  }
  return 0;
}

/////////////////////////////////////////////
// Update GameBoard With The Player's Piece //
////////////////////////////////////////////
void updateGameBoard(Board &board, int move, char playerPiece) 
{
  // Calculate the row and column from the move
  int row = (move - 1) / board.columns;
  int col = (move - 1) % board.columns;

  // Update the selected cell with the player's piece
  board.gameBoard[row][col] = playerPiece;
}

//////////////////////////////////////////////////
// Check's if the User Input For Names Are Valid //
/////////////////////////////////////////////////
bool isValidName(const string &str) 
{
  int spaceCount = 0;

  for (char c : str) 
  {
    if (c == ' ') 
    {
      spaceCount++;
    } 
    else if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) 
    {
      return false;
    }
  }

  return spaceCount >= 1;
}

///////////////////////////////
// Find The User's First Name //
//////////////////////////////
string getFirstName(const string &full_name) 
{
  size_t space_pos = full_name.find(' ');

  if (space_pos != string::npos) 
  {
    return full_name.substr(0, space_pos);
  }
  return full_name;
}

//////////////////////////////////////
// Get User Move For Piece Placement //
/////////////////////////////////////
int getUserMove(const string &playerName, char playerPiece, const Board &board) 
{
  // Extract the first name from the full name
  string firstName = getFirstName(playerName);

  // Remind the player of their piece
  cout << "It's " << firstName << "'s turn. Your piece is: " << playerPiece << endl;

  string move;
  cout << "\nEnter your move (e.g., A1, B2): ";
  cin >> move;
  cout << "\n";

  // Ensure the input is in the correct format
  for (char &c : move) 
  {
    c = toupper(c); // Convert each character to uppercase
  }

  if (move.length() < 2 || move.length() > 3 || !(move[0] >= 'A' && move[0] <= 'Z') || !(move[1] >= '0' && move[1] <= '9') || (move.length() == 3 && !(move[2] >= '0' && move[2] <= '9'))) 
  {
    cout << "\nInvalid move format! Please enter a move in the format A1, B2, etc.: \n";
    while (cin.get() != '\n')
      ; // Clear the input buffer

    return getUserMove(playerName, playerPiece, board);
  }

  char rowLabel = move[0];
  int row = rowLabel - 'A'; // Convert letter to row index (0-based)

  int col;
  if (move.length() == 2) 
  {
    col = move[1] - '1'; // Convert number to column index (0-based)
  } 
  else 
  {
    col = (move[1] - '0') * 10 + (move[2] - '0') - 1; // Handle three-digit input
  }

  // Check if the move is within the board boundaries
  if (row < 0 || row >= board.rows || col < 0 || col >= board.columns) {
    cout << "\nInvalid move! The selected cell is out of bounds. Choose another move: ";
    return getUserMove(playerName, playerPiece, board);
  }

  // Check if the selected cell is empty (contains ' ')
  if (board.gameBoard[row][col] != ' ') 
  {
    cout << "Invalid move! The selected cell is already occupied. Choose another move: ";
    return getUserMove(playerName, playerPiece, board);
  }
  if (move.empty()) 
  {
    cout << "\nInvalid move! Please choose a cell ie. (a1, a2 etc...) \n";
    return getUserMove(playerName, playerPiece, board);
  }

  // Convert row and col to a single number between 1 and board.rows *
  // board.columns
  int moveNumber = row * board.columns + col + 1;

  return moveNumber;
}

///////////////////////////////////
// Ask For User's Size Board Size //
//////////////////////////////////
Board getBoardSize() 
{
  Board board;
  string input; // Variable to store user input

  bool validInput = false;

  while (!validInput) 
  {
    do 
    {
      cout << "Enter number of rows (4-13): ";
      getline(cin, input);

      input = trim(input);
      // Check if the input is empty
      if (input.empty()) 
      {
        cout << "Invalid input! Please enter a valid number." << endl;
        continue; // Skip to the next iteration
      }
      // Check if the input contains only digits
      if (input.find_first_not_of("0123456789") == string::npos) 
      {
        cin.clear();
        board.rows = stoi(input); // Convert the input string to an integer
      }
    } while (input.empty() || board.rows < MIN_ROWS || board.rows > MAX_ROWS);

    do 
    {
      cout << "Enter number of columns (4-16): ";
      getline(cin, input);

      input = trim(input);
      // Check if the input is empty
      if (input.empty()) 
      {
        cout << "Invalid input! Please enter a valid number." << endl;
        continue; // Skip to the next iteration
      }
      // Check if the input contains only digits
      if (input.find_first_not_of("0123456789") == string::npos) 
      {
        board.columns = stoi(input); // Convert the input string to an integer
      }
    } while (input.empty() || board.columns < MIN_COLUMNS || board.columns > MAX_COLUMNS);

    // Check if the board size is valid (e.g., rows x columns <= 13 x 16)
    if (board.rows * board.columns <= 13 * 16) 
    {
      validInput = true; // Valid board size, exit the loop
    } 
    else 
    {
      cout << "Invalid board size! The product of rows and columns must be at most 13 x 16." << endl;
    }
  }

  return board;
}

///////////////////////////////////
// Assign Player's Pieces To Play //
//////////////////////////////////
void assignPlayerPieces(Player players[], int numberOfPlayers) 
{
  char piece = 'a';
  for (int i = 0; i < numberOfPlayers; i++) 
  {
    if (i == 5) 
    { // Check if it's the 6th player
      players[i].playerPiece = 'g';
    } 
    else if (i == 6) 
    { // Check if it's the 7th player
      players[i].playerPiece = 'h';
    } 
    else 
    {
      players[i].playerPiece = piece;
      piece++;
    }
  }
}

//////////////////////////////////////
// Inialize Empty Cells In The Board //
/////////////////////////////////////
void initializeGameBoard(Board &board) 
{
  board.gameBoard = new char *[board.rows];
  for (int i = 0; i < board.rows; i++) 
  {
    board.gameBoard[i] = new char[board.columns];
    for (int j = 0; j < board.columns; j++) 
    {
      board.gameBoard[i][j] = ' '; // Initialize all cells to ' '
    }
  }
}

/////////////////////////////////
// Display's Original GameBoard //
////////////////////////////////
void displayBoard(const Board &board) 
{
  // Display the top border with column numbers
  cout << "\n\n";
  cout << "   ";
  for (int col = 0; col < board.columns; col++) 
  {
    if (col < 10) 
    {
      cout << " " << col + 1 << "  ";
    } 
    else if (col >= 10) 
    {
      cout << col + 1 << "  ";
    }
  }
  cout << endl;

  // Display the board
  for (int row = 0; row < board.rows; row++) 
  {
    char rowLabel = 'A' + row;

    // Display the top border of each row
    cout << "   ";
    for (int col = 0; col < board.columns; col++) 
    {
      cout << "--- ";
    }
    cout << endl;

    // Display the cell content and labels
    cout << rowLabel << " |";
    for (int col = 0; col < board.columns; col++) 
    {
      cout << " " << board.gameBoard[row][col] << " |";
    }
    cout << " " << rowLabel << endl;
  }

  // Display the bottom border with row letters
  cout << "   ";
  for (int col = 0; col < board.columns; col++) 
  {
    cout << "--- ";
  }
  cout << endl;

  // Display the row numbers at the bottom
  cout << "   ";
  for (int col = 0; col < board.columns; col++) 
  {
    if (col < 10) 
    {
      cout << " " << col + 1 << " "
           << " ";
    } 
    else if (col >= 10) 
    {
      cout << col + 1 << "  ";
    }
  }
  cout << endl;
}

///////////////////////////
// Check's If Valid Move //
/////////////////////////
bool isValidMove(const Board &board, int move) {
  // Check if the move is within the board boundaries
  if (move < 1 || move > board.rows * board.columns) 
  {
    cout << "Invalid move! Please enter a valid move between 1 and " << board.rows * board.columns << ": ";
    return false;
  }

  // Calculate the row and column from the move
  int row = (move - 1) / board.columns;
  int col = (move - 1) % board.columns;

  // Check if the selected cell is empty (contains ' ')
  if (board.gameBoard[row][col] != ' ') 
  {
    cout << "Invalid move! The selected cell is already occupied. Choose another move: ";
    return false;
  }

  return true;
}

////////////////////////
// Update's Game Board //
///////////////////////
void updateGameBoard(Board &board, int row, int col, char playerPiece) {
  board.gameBoard[row][col] = playerPiece;
}

///////////////////////////////
// Display's board Statistics //
//////////////////////////////
void displayGameStatistics(const Player players[], int numberOfPlayers, int totalGamesPlayed) 
{
  cout << "\nTotal Games played: " << totalGamesPlayed << "\n\n";

  // Find the maximum length of player names for alignment
  int maxNameLength = 0;
  for (int i = 0; i < numberOfPlayers; i++) 
  {
    int nameLength = players[i].fullName.length();
    if (nameLength > maxNameLength) 
    {
      maxNameLength = nameLength;
    }
  }

  // Calculate the width of each cell to maintain consistent board dimensions
  int cellWidth = maxNameLength + 3; // Adjust for spacing

  // Display the top dotted line for title cells
  cout << rightAlign("", cellWidth) << " " << string(6, '-') << " " << string(6, '-') << " " << string(6, '-') << "\n";

  // Right-align the title cells
  for (int i = 0; i < maxNameLength; i++)
    {
      cout << " ";
    }
  cout  << string(4, ' ') << rightAlign("| WIN | LOSS | DRAW | \n", cellWidth);

  // Display the bottom dotted line for title cells
  cout << rightAlign("", cellWidth) << " " << string(6, '-') << " " << string(6, '-') << " " << string(6, '-') << "\n";

  // Display player statistics
  for (int i = 0; i < numberOfPlayers; i++) 
  {
    cout << rightAlign(players[i].fullName, cellWidth) << " |    " << players[i].playerWins << " |    " << players[i].playerLosses << " |    " << players[i].draws << " |\n";
    cout << rightAlign(" ", cellWidth) << string(6, '-') << "  " << string(6, '-') << " " << string(6, '-') << "\n";
  }
}

///////////////////
// Check for draw //
//////////////////
bool checkForDraw(const Board &board) 
{
  for (int row = 0; row < board.rows; row++) 
  {
    for (int col = 0; col < board.columns; col++) 
    {
      if (board.gameBoard[row][col] == ' ') 
      {
        return false; // There's an empty cell, the game is not a draw
      }
    }
  }
  return true; // All cells are filled, it's a draw
}

///////////////////////////////////////////////////////////////
// Function to trim leading and trailing spaces from a string //
//////////////////////////////////////////////////////////////
string trim(const string &str) 
{
  size_t first = str.find_first_not_of(' ');

  if (string::npos == first) 
  {
    return str;
  }

  size_t last = str.find_last_not_of(' ');

  return str.substr(first, (last - first + 1));
}

////////////////////////////////
// Check for winning Condition ///
///////////////////////////////
bool checkForWin(Board &board, int row, int col, char playerPiece) 
{
  // Check horizontally
  int count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row;
    int newCol = col + i;
    if (newCol >= 0 && newCol < board.columns &&
        board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;
      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        updateWinningConfiguration(board, row, col, playerPiece);
        return true;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  // Check vertically
  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col;
    if (newRow >= 0 && newRow < board.rows && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;
      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        updateWinningConfiguration(board, row, col, playerPiece);
        return true;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  // Check diagonally (both directions)
  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col + i;
    if (newRow >= 0 && newRow < board.rows && newCol >= 0 && newCol < board.columns && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;
      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        updateWinningConfiguration(board, row, col, playerPiece);
        return true;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col - i;
    if (newRow >= 0 && newRow < board.rows && newCol >= 0 &&
        newCol < board.columns &&
        board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;
      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        updateWinningConfiguration(board, row, col, playerPiece);
        return true;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  return false; // No winning condition found
}

////////////////////////////////////////
// Update the player piece to upperCase //
///////////////////////////////////////
void updateToUppercase(Board &board, int startRow, int startCol, int endRow, int endCol, char playerPiece) 
{
  for (int row = startRow; row <= endRow; row++) 
  {
    for (int col = startCol; col <= endCol; col++) 
    {
      if (board.gameBoard[row][col] == playerPiece) 
      {
        board.gameBoard[row][col] = toupper(playerPiece);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
// Function to convert a lowercase character to uppercase manually //
////////////////////////////////////////////////////////////////////
char toUpperCase(char c) 
{
  if (c >= 'a' && c <= 'z') 
  {
    return c - 'a' + 'A';
  }
  return c;
}

//////////////////////////////////////////////
// Checks for winning configuration based on //
/////////////////////////////////////////////
void updateWinningConfiguration(Board &board, int row, int col, char playerPiece) 
{
  // Define the winning configuration pattern (adjust as needed)
  char winPattern[] = "aaa"; // For example, three consecutive 'a's

  // Check horizontally
  int count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row;
    int newCol = col + i;

    if (newCol >= 0 && newCol < board.columns && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;

      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        for (int j = -2; j <= 2; j++) 
        {
          int updateRow = row;
          int updateCol = col + j;

          if (updateCol >= 0 && updateCol < board.columns && board.gameBoard[updateRow][updateCol] == playerPiece) 
          {
            board.gameBoard[updateRow][updateCol] = toupper(playerPiece);
          }
        }
        return;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  // Check vertically
  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col;

    if (newRow >= 0 && newRow < board.rows && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;

      if (count == 3) {
        // Update the winning configuration to uppercase
        for (int j = -2; j <= 2; j++) 
        {
          int updateRow = row + j;
          int updateCol = col;

          if (updateRow >= 0 && updateRow < board.rows && board.gameBoard[updateRow][updateCol] == playerPiece) 
          {
            board.gameBoard[updateRow][updateCol] = toupper(playerPiece);
          }
        }
        return;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  // Check diagonally (both directions)
  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col + i;

    if (newRow >= 0 && newRow < board.rows && newCol >= 0 && newCol < board.columns && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;

      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        for (int j = -2; j <= 2; j++) 
        {
          int updateRow = row + j;
          int updateCol = col + j;

          if (updateRow >= 0 && updateRow < board.rows && updateCol >= 0 && updateCol < board.columns && board.gameBoard[updateRow][updateCol] == playerPiece) 
          {
            board.gameBoard[updateRow][updateCol] = toupper(playerPiece);
          }
        }
        return;
      }
    } 
    else 
    {
      count = 0;
    }
  }

  count = 0;
  for (int i = -2; i <= 2; i++) 
  {
    int newRow = row + i;
    int newCol = col - i;

    if (newRow >= 0 && newRow < board.rows && newCol >= 0 && newCol < board.columns && board.gameBoard[newRow][newCol] == playerPiece) 
    {
      count++;

      if (count == 3) 
      {
        // Update the winning configuration to uppercase
        for (int j = -2; j <= 2; j++) 
        {
          int updateRow = row + j;
          int updateCol = col - j;

          if (updateRow >= 0 && updateRow < board.rows && updateCol >= 0 && updateCol < board.columns && board.gameBoard[updateRow][updateCol] == playerPiece) 
          {
            board.gameBoard[updateRow][updateCol] = toupper(playerPiece);
          }
        }
        return;
      }
    } 
    else 
    {
      count = 0;
    }
  }
}

////////////////////////////////////////////////////////////
// Custom sorting function to sort players by playerPiece //
///////////////////////////////////////////////////////////
void customSort(Player players[], int numberOfPlayers) 
{
  for (int i = 0; i < numberOfPlayers - 1; i++) 
  {
    for (int j = i + 1; j < numberOfPlayers; j++) 
    {
      if (players[i].playerPiece > players[j].playerPiece) 
      {
        // Swap players[i] and players[j]
        Player temp = players[i];
        players[i] = players[j];
        players[j] = temp;
      }
    }
  }
}

//////////////////////////
// Help to align strings ///
/////////////////////////
string rightAlign(const string &text, int width) 
{
  int spacesToAdd = width - text.length();
  if (spacesToAdd <= 0) 
  {
    // No need to align, text is already longer than or equal to the width
    return text;
  }

  // Create a string with the necessary number of spaces
  string padding(spacesToAdd, ' ');

  // Concatenate the spaces before the text
  return padding + text;
}
