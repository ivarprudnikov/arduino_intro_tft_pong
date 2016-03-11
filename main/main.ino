#include <SPI.h>
#include <TFT.h>

#define cs   7
#define dc   0
#define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int paddleWidth = 20;
int paddleHeight = 5;
int ballDirectionX = 1;
int ballDirectionY = 1;
int ballRadius = 3;

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

void setup() {

  Serial.begin(9600);
  
  // initialize the display
  TFTscreen.begin();
  // black background
  TFTscreen.background(0, 0, 0);
}


void loop() {

  Serial.println(analogRead(A0));

  // save the width and height of the screen
  int myWidth = TFTscreen.width();
  int myHeight = TFTscreen.height();

  // map the paddle's location to the position of the potentiometers
  paddleX = map(analogRead(A0), 0, 1000, 0, myWidth - paddleWidth);
  paddleY = map(analogRead(A1), 0, 1000, myHeight - paddleHeight, 0);

  // set the fill color to black and erase the previous
  // position of the paddle if different from present
  TFTscreen.fill(0, 0, 0);

  if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    TFTscreen.rect(oldPaddleX, oldPaddleY, paddleWidth, paddleHeight);
  }

  // draw the paddle on screen, save the current position
  // as the previous.
  TFTscreen.fill(255, 255, 255);

  TFTscreen.rect(paddleX, paddleY, paddleWidth, paddleHeight);
  oldPaddleX = paddleX;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2) {
    moveBall();
  }
}

// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > TFTscreen.width() || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > TFTscreen.height() || ballY < 0) {
    ballDirectionY = -ballDirectionY;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  TFTscreen.fill(0, 0, 0);

  if (oldBallX != ballX || oldBallY != ballY) {
    TFTscreen.circle(oldBallX, oldBallY, ballRadius);
  }


  // draw the ball's current position
  TFTscreen.fill(255, 50, 50);
  TFTscreen.circle(ballX, ballY, ballRadius);

  oldBallX = ballX;
  oldBallY = ballY;

}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY) {
  boolean result = false;

  if ((x + ballRadius >= rectX && x <= (rectX + paddleWidth + ballRadius)) &&
      (y + ballRadius >= rectY && y <= (rectY + paddleHeight + ballRadius))) {
    result = true;
  }

  return result;
}


