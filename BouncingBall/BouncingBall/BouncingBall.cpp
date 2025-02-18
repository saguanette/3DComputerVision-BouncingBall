#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#define WIDTH 800
#define HEIGHT 600
cv::Mat image;
int xCoord, yCoord;  //the coordinates of the ball
int xSpeed = 5, ySpeed = 5;  //setting the speed of the ball
int xRacket; //this will be changing, as we will hover the mouse over and move the racket
const int yRacket = HEIGHT - 30;  //racket is not moving in the vertical dir, so better to make it a const 
const int racketWidth = 150;
const int racketHeight = 20;

void redraw() {
    cv::rectangle(image, cv::Point(0, 0), cv::Point(WIDTH, HEIGHT), cv::Scalar(0, 0, 0), cv::FILLED); //drawing our black window 
    cv::circle(image, cv::Point(xCoord, yCoord), 30, cv::Scalar(128, 0, 128), cv::FILLED);//drawing the ball, I like purple, so it's purple)
    cv::rectangle(image, cv::Point(xRacket, yRacket), cv::Point(xRacket + racketWidth, yRacket + racketHeight), cv::Scalar(0, 255, 255), cv::FILLED); //the racket will be yellow
    cv::imshow("Display window", image);
}

bool updateBallPosition() {
    //updating the ball's position so that it is constantly moving, this will be called in the main
    xCoord += xSpeed; //basically moving the ball just by adding 5 points to its X and Y axis coordinates
    yCoord += ySpeed;

    //adding the bouncing from the walls on right and left sides 
    if (xCoord >= WIDTH - 30 || xCoord <= 30) {
        xSpeed = -xSpeed; //reversing the direction of the ball if it goes beyond right or left wall
    }
    if (yCoord <= 30) {
        ySpeed = -ySpeed; //same for the top wall
    }

    //for bouncing off the racket 
    if (yCoord + 30 >= yRacket && xCoord >= xRacket && xCoord <= xRacket + racketWidth) {
        ySpeed = -ySpeed;
    }

    //checking if the ball hits the bottom 
    if (yCoord > HEIGHT) {
        return false;  //if yes, we will return false, this will be used in the main func to trigger the exit  
    }

    return true;  //otherwise continue game
}

void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    //moving the racket with mouse
    if (event == cv::EVENT_MOUSEMOVE) {
        xRacket = x - racketWidth / 2;  //centering the racket on the mouse 
        if (xRacket < 0) xRacket = 0;
        if (xRacket + racketWidth > WIDTH) xRacket = WIDTH - racketWidth;
    }
}

int main() {
    image = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    xCoord = WIDTH / 2; //these are starting positions of a ball
    yCoord = HEIGHT / 2;
    xRacket = WIDTH / 2 - racketWidth / 2; //and the racket

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    redraw();

    cv::setMouseCallback("Display window", MouseCallBackFunc, NULL); 

    int key;
    while (true) {
        key = cv::waitKey(30);
        if (key == 27) break;  //exit on esc key 

        if (!updateBallPosition()) {
            std::cout << "Game Over! The ball fell.\n";
            break;  //break off the loop if the ball falls 
        }

        redraw();
    }

    return 0;
}
