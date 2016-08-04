/* 8x8 Matrix Display for Arduino
 * Pong with Preceding Frames Animation
 * Simeng Yang
 */

//Global variables
int rightButton;
int leftButton;

int left = 1;
int right = -1;
int rdir;                           //random direction for ball
int deltaCoordinate = 0;                           //Change in ball 'co-ordinate'
int ballPosition;                           //Determines x-position of ball

//Reference variables for array indexes
int leftCoordinate = 2;
int rightCoordinate = 2;
int ballCoordinate = 3;

int leftCounter = 0;                           //Determinor for change in left paddle direction
int rightCounter = 0;                           //Determinor for change in right paddle direction
unsigned long firstLeftMillis = 0;
unsigned long lastLeftMillis = 0;
unsigned long firstRightMillis = 0;
unsigned long lastRightMillis = 0;

//button PINs
const int LEFT_BUTTON = 13;
const int RIGHT_BUTTON = 12;

//Toggle whether to show the name frames or not
bool showIt = true;

//Sets frame duration; will be different for initial frames of animation and pong game
int framesDuration;

//Rows and columns of matrix
int column[] = {2, 7, 19, 5, 10, 18, 11, 16};                            // Anode pins in order they represent as column
int row[] = {6, 14, 15, 3, 17, 4, 8, 9};                              // Cathode pins in order they represent as row

int leftPad[] = {0, 0, 128, 128, 128, 0, 0, 0};
int rightPad[] = {0, 0, 1, 1, 1, 0, 0, 0};
int ball[] = {0, 0, 0, 16, 0, 0, 0, 0};

int firstName[][9][8] = {                          //First name: SIMENG
  { //S
    {255, 255, 192, 255, 255, 3, 255, 255},
    {254, 254, 128, 254, 254, 6, 254, 254},
    {253, 253, 0, 252, 252, 12, 253, 253},
    {251, 251, 0, 248, 248, 24, 251, 251},
    {247, 247, 0, 240, 240, 48, 247, 247},
    {239, 239, 1, 225, 225, 97, 239, 239},
    {223, 223, 3, 195, 195, 195, 223, 223},
    {191, 191, 6, 134, 134, 134, 191, 191},
    {127, 127, 12, 12, 12, 12, 127, 127}
  },
  { //I
    {255, 255, 24, 24, 24, 24, 255, 255},
    {255, 24, 24, 24, 24, 255, 255, 0},
    {24, 24, 24, 24, 255, 255, 0, 195},
    {24, 24, 24, 255, 255, 0, 195, 231},
    {24, 24, 255, 255, 0, 195, 231, 255},
    {24, 255, 255, 0, 195, 231, 255, 255},
    {255, 255, 0, 195, 231, 255, 255, 255},
    {255, 0, 195, 231, 255, 255, 255, 219},
    {0, 195, 231, 255, 255, 255, 219, 195}
  },
  { //M
    {195, 231, 255, 255, 255, 219, 195, 195},
    {206, 254, 254, 254, 182, 134, 134, 0},
    {252, 252, 252, 108, 12, 12, 0, 1},
    {248, 248, 216, 24, 24, 0, 3, 3},
    {240, 176, 48, 48, 0, 7, 7, 6},
    {96, 96, 96, 0, 15, 15, 12, 15},
    {192, 192, 0, 31, 31, 24, 31, 31},
    {128, 0, 63, 63, 48, 63, 63, 48},
    {0, 127, 127, 96, 127, 127, 96, 127}
  },
  { //E
    {255, 255, 192, 255, 255, 192, 255, 255},
    {127, 96, 127, 127, 96, 127, 127, 0},
    {48, 63, 63, 48, 63, 63, 0, 128},
    {31, 31, 24, 31, 31, 0, 192, 192},
    {15, 12, 15, 15, 0, 96, 96, 96},
    {6, 7, 7, 0, 48, 48, 176, 240},
    {3, 3, 0, 24, 152, 216, 248, 248},
    {1, 0, 140, 204, 236, 252, 124, 60},
    {0, 198, 230, 246, 254, 190, 158, 142}
  },
  { //N
    {227, 243, 251, 255, 223, 207, 199, 195},
    {0, 198, 230, 246, 254, 190, 158, 142},
    {1, 0, 140, 204, 236, 252, 124, 60},
    {3, 3, 0, 24, 152, 216, 248, 248},
    {6, 7, 7, 0, 48, 48, 176, 240},
    {12, 12, 15, 15, 0, 96, 96, 96},
    {25, 25, 24, 31, 31, 0, 192, 192},
    {48, 51, 51, 48, 63, 63, 0, 128},
    {127, 96, 103, 103, 97, 127, 127, 0}
  },
  { //G
    {255, 255, 192, 207, 207, 195, 255, 255},
    {0, 127, 127, 96, 103, 103, 97, 127},
    {128, 0, 63, 63, 48, 51, 51, 48},
    {192, 192, 0, 31, 31, 24, 25, 25},
    {96, 224, 224, 0, 15, 15, 12, 12},
    {240, 48, 240, 240, 0, 7, 7, 6},
    {248, 248, 24, 248, 248, 0, 3, 3},
    {0, 252, 252, 12, 252, 252, 0, 1},
    {254, 128, 254, 254, 6, 254, 254, 0}
  }
};

int lastName[][8][8] = {                          //Last name: YANG
  { //Y
    {195, 231, 126, 60, 24, 24, 24, 24},
    {206, 252, 120, 48, 48, 48, 48, 0},
    {252, 120, 48, 48, 48, 48, 0, 0},
    {240, 96, 96, 96, 96, 0, 0, 0},
    {96, 96, 96, 96, 0, 0, 0, 0},
    {192, 192, 192, 0, 0, 0, 0, 0},
    {192, 192, 0, 0, 0, 0, 0, 0},
    {128, 0, 0, 0, 0, 0, 0, 0}
  },
  { //A
    {24, 60, 126, 195, 255, 255, 195, 195},
    {30, 63, 97, 127, 127, 97, 97, 0},
    {31, 48, 63, 63, 48, 48, 0, 0},
    {24, 31, 31, 24, 24, 0, 0, 0},
    {15, 15, 12, 12, 0, 0, 0, 0},
    {7, 6, 6, 0, 0, 0, 0, 0},
    {3, 3, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0}
  },
  { //N
    {227, 243, 251, 255, 223, 207, 199, 195},
    {0, 198, 230, 246, 254, 190, 158, 142},
    {0, 0, 140, 204, 236, 252, 124, 60},
    {0, 0, 0, 24, 152, 216, 248, 248},
    {0, 0, 0, 0, 48, 48, 176, 240},
    {0, 0, 0, 0, 0, 96, 96, 96},
    {0, 0, 0, 0, 0, 0, 192, 192},
    {0, 0, 0, 0, 0, 0, 0, 128}
  },
  { //G
    {255, 255, 192, 207, 207, 195, 255, 255},
    {0, 127, 127, 96, 103, 103, 97, 127},
    {0, 0, 63, 63, 48, 51, 51, 48},
    {0, 0, 0, 31, 31, 24, 25, 25},
    {0, 0, 0, 0, 15, 15, 12, 12},
    {0, 0, 0, 0, 0, 7, 7, 6},
    {0, 0, 0, 0, 0, 0, 3, 3},
    {0, 0, 0, 0, 0, 0, 0, 1}
  }
};

int stickMan[][8] = {
  {56, 43, 18, 254, 144, 56, 108, 198},
  {56, 168, 144, 254, 18, 56, 108, 198},
  {56, 43, 18, 254, 144, 56, 108, 198},
  {56, 168, 144, 254, 18, 56, 108, 198},
  {25, 19, 214, 188, 214, 19, 113, 64},
  {99, 54, 28, 9, 127, 72, 212, 28},
  {2, 142, 200, 107, 61, 107, 200, 152}
};

int pacMan[][8] = {
  {120, 132, 172, 132, 132, 172, 212, 172},
  {60, 66, 86, 66, 66, 86, 106, 86},
  {30, 33, 43, 33, 33, 43, 53, 43},
  {60, 120, 240, 194, 199, 242, 120, 60},
  {30, 60, 120, 97, 99, 121, 60, 30},
  {15, 30, 60, 48, 49, 60, 30, 15}
};

void setup() {
  Serial.begin(9600);                          //Generic: Sets the data rate in bauds for serial data transmission
  randomSeed(analogRead(0));                          //Initializes the pseudo-random number generator with random analogue input

  for (int next = 0; next < 8; next++)                          // sets all I/O used as outputs
  {
    pinMode(column[next], OUTPUT);
    pinMode(row[next], OUTPUT);
  }

  pinMode(LEFT_BUTTON, INPUT);                          //Sets both button I/O as inputs
  pinMode(RIGHT_BUTTON, INPUT);

  framesDuration = 400;                          //Sets frames duration for frames of animation

  displayMan();
  displayPac();

  int determinant = random(1, 10);                         //Randomy determine ball y-position and initial ball direction

  ballPosition = random(3, 5);

  if (determinant % 2 == 0)
  {
    rdir = 1;
  }
  else
  {
    rdir = -1;
  }
}

void displayMan()                          //Displays stickman
{
  for (int i = 0; i < 7; i++)
  {
    scan(stickMan[i]);
  }
}

void displayPac()                          //Displays Pacman animation
{
  for (int i = 0; i < 6; i++)
  {
    scan(pacMan[i]);
  }
}

void displayFirstName()                          //Displays frames for first name: SIMENG
{
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      rightButton = digitalRead(RIGHT_BUTTON);                          //Get states of both buttons
      leftButton = digitalRead(LEFT_BUTTON);

      if ((leftButton == HIGH) || (rightButton == HIGH))                          //If either button is pressed, exits the display
      {
        showIt = false;
        break;
      }

      scan(firstName[i][j]);
    }
  }
}

void displayLastName()                          //Displays frames for last name: YANG
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      scan(lastName[i][j]);
    }
  }
}

void ClearArray(int toClear[])                          //Clears array; sets all array indexes to 0
{
  for (int i = 0; i < 8; i++)
  {
    toClear[i] = 0;
  }
}

void loop() {                          //Runs pong on the matrix
  if (showIt == true)                          //Displays first name
  {
    displayFirstName();
    //displayLastName();                          //Will use too much memory, so omitted during runtime ('stability problems may occur')
  }
  else
  {
    framesDuration = 500;                          //Make frames duration longer so pong is easier to play
    
    rightButton = digitalRead(RIGHT_BUTTON);                          //Get states of both buttons
    leftButton = digitalRead(LEFT_BUTTON);

    if (leftButton == HIGH)                          //For when the left button is pressed
    {
      firstLeftMillis = millis();                          //Sets time in milliseconds from start

      if ((leftPad[0] == 128) || (leftPad[7] == 128))                        //If the paddle reaches the ends of the screen, direction is flipped
      {
        left *= -1;
      }

      leftCoordinate += left;                          //Changes the reference vertical coordinate for the left paddle
      ClearArray(leftPad);                          //Clears the paddle
      leftPad[leftCoordinate] = 128;                          //'Turns on' on the display for the paddle with appropriate coordinates
      leftPad[leftCoordinate + 1] = 128;
      leftPad[leftCoordinate + 2] = 128;
    }
    else if ((leftPad[0] == 0) && (leftPad[7] == 0))                           //Applies once button is released and if the paddle does not touch the ends
    {
      lastLeftMillis = millis();                          //Sets time in milliseconds from start

      if (lastLeftMillis - firstLeftMillis < 1000)                           //If time pressed is within a sufficiently short timeframe, increments the counter
      {
        leftCounter++;
      }

      if (leftCounter % 2 == 0)                           //If counter is odd, flips the direction; in effect, every two 'rapid button clicks' changes the direction fo the paddle
      {
        left *= -1;
      }

      firstLeftMillis = lastLeftMillis;                          //Make sure you update the time
    }

    if (rightButton == HIGH)                          //For when the right button is pressed
    {
      firstRightMillis = millis();                          //Same shebang; update the time

      if ((rightPad[0] == 1) || (rightPad[7] == 1))                          //If the paddle reaches the ends of the screen vertically within the column, flip direction
      {
        right *= -1;
      }

      rightCoordinate += right;                          //Change the vertical reference coordinate of the right paddle
      ClearArray(rightPad);                          //Clears the array for the right paddle
      rightPad[rightCoordinate] = 1;                          //'Turns on' the paddle with appropriate coordinates
      rightPad[rightCoordinate + 1] = 1;
      rightPad[rightCoordinate + 2] = 1;
    }
    else if ((rightPad[0] == 0) && (rightPad[7] == 0))                           //Applies once button is released and if the paddle does not touch the ends
    {
      lastRightMillis = millis();                           //Update time

      if (lastRightMillis - firstRightMillis < 1000)                           //Increments counter if button is pressed and released in quick succession
      {
        rightCounter++;
      }

      if (rightCounter % 2 == 0)                           //If the right counter is even, flip the direction of the paddle
      {
        right *= -1;
      }

      firstRightMillis = lastRightMillis;                           //Updates the time
    }

    if ((ball[ballCoordinate] == 64) && ((leftPad[ballCoordinate] != 0)))                           //Applies when the ball occupies a column adjacent to the column for the left paddle
    {                                                                                               //If the paddle is 'on' in a cell horizontally adjacent to the ball, flip direction of ball
        rdir *= -1;

        if (ballCoordinate == leftCoordinate)                           //Determines vertical movement of ball if ball comes into contact with edge of paddle; initially no vertical movement
        {
          deltaCoordinate = 1;
        }
        else if (ballCoordinate == leftCoordinate - 2)
        {
          deltaCoordinate = -1;
        }
        else if (ballCoordinate == leftCoordinate + 2)
        {
          deltaCoordinate = 1;
        }
      }
      else if (ball[ballCoordinate] == 128)
      {
        deltaCoordinate = 0;                           //Otherwise, reset ball as it has been lost by the left paddle
        ballCoordinate = random(2, 4);
        ballPosition = 3;
      }
    else if ((ball[ballCoordinate] == 2) && (rightPad[ballCoordinate] != 0))                         //Applies when the ball occupies a column adjacent to the column for the right paddle
    {                                                                                                //If the paddle is 'on' in a cell horizontally adjacent to the ball, flip direction of ball
        rdir *= -1;

        if (ballCoordinate == rightCoordinate)                           //Determines vertical movement of ball if ball comes into contact with edge of paddle; initially no vertical movement
        {
          deltaCoordinate = -1;
        }
        else if (ballCoordinate == rightCoordinate - 2)
        {
          deltaCoordinate = 1;
        }
        else if (ballCoordinate == rightCoordinate + 2)
        {
          deltaCoordinate = -1;
        }
      }
      else if (ball[ballCoordinate] == 1)
      {
        deltaCoordinate = 0;                           //Otherwise, reset ball as it has been lost by the right paddle
        ballCoordinate = random(2, 4);
        ballPosition = 5;
      }

    if ((ballCoordinate == 0) || (ballCoordinate == 7))                       //If the ball touches the cieling or floor of the display, reverses the vertical movement of the ball
    {
      if (!((ball[ballCoordinate] == 2) && (rightPad[7] == 1)))                       //Fixes a nasty problem for when the ball is at the lower-right corner of the matrix
      {
        deltaCoordinate *= -1;
      }
    }

    ballPosition += rdir;                           //Changes ball horizontal position
    ballCoordinate += deltaCoordinate;                           //Changes ball vertical position; initially 0
    ClearArray(ball);                           //Clears ball array
    ball[ballCoordinate] = pow(2, ballPosition) + 0.5;                           //Determines ball horizontal coordinate in relation to vertical coordinate

    scanAll(leftPad, rightPad, ball);                           //Displays the left paddle, right paddle and ball
  }
}

void scanAll(int firstArray[], int secondArray[], int thirdArray[])
{
  turnOffAllcolumn();
  unsigned long startTime = millis();

  do {
    for (int currentRow = 0; currentRow < 8; currentRow++)
    {
      outputRow(firstArray[currentRow], currentRow);
      outputRow(secondArray[currentRow], currentRow);
      outputRow(thirdArray[currentRow], currentRow);
    }
  } while (millis() - startTime < framesDuration);                         //delay for how long character is shown.
}

void scan(int currentArray[])                                  //Turns on/off all columns in all rows sequentially to display letter
{
  turnOffAllcolumn();
  unsigned long startTime = millis();

  do {
    for (int currentRow = 0; currentRow < 8; currentRow++)
    {
      outputRow(currentArray[currentRow], currentRow);
    }
  } while (millis() - startTime < framesDuration);                         //delay for how long character is shown.
}

void outputRow(int cols, int curRow)                            //Manages the LEDs for a particular row
{
  digitalWrite(row[curRow], HIGH);                              //turns on current row

  for (int currentColumn = 0; currentColumn < 8; currentColumn++)
  {
    bool isAlive = bitRead(cols, currentColumn);

    if (isAlive)              //Reads bits individually at specified indexes
                              //At index/position currentColumn, if the bit/digit is 1, then output a high at the position for that row
    {
      digitalWrite(column[currentColumn], LOW);
      digitalWrite(column[currentColumn], HIGH);
    }
  }
  digitalWrite(row[curRow], LOW);                             //shuts off current row
}

void turnOffAllcolumn()
{
  for (int currentColumn = 0; currentColumn < 8; currentColumn++)
  {
    digitalWrite(column[currentColumn], HIGH);                // reset column to off
  }
}

