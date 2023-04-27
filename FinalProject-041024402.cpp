#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/Timer.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

using namespace std;

//Global declarations
int randomNum;
int index = 0;
int clickCount = 0;
int rounds = 0;
bool patternFinished = false;
std::vector<int> vec;
sf::RenderWindow window{ {400, 400},"Skylar Phanenhour - Final Project" };
tgui::ChildWindow::Ptr endGameWindow;
tgui::Button::Ptr btnGreen, btnRed, btnYellow, btnBlue, btnStart;
tgui::Timer::Ptr timer = tgui::Timer::create([]() {}, 500, false);
tgui::Timer::Ptr clickTimer = tgui::Timer::create([]() {}, 5000, false);

sf::SoundBuffer buffer;
sf::Sound sound;

//Forward Declarations
void makeWindow();
void start();
void generateNum();
void pattern();
void enableButtons();
void setButtonsEnabled(bool);
void checkButtonClicked(int);
void endGame();
void restart();

//Main method that starts and makes the GUI
int main(int argc, const char* argv[]) {
	makeWindow();
	return 0;
}

//Makes the GUI on the window
void makeWindow() {
	tgui::Gui gui{ window };
	gui.loadWidgetsFromFile("form.txt");

	//Gets the buttons that are on the window
	btnGreen = gui.get<tgui::Button>("greenButton");
	btnRed = gui.get<tgui::Button>("redButton");
	btnYellow = gui.get<tgui::Button>("yellowButton");
	btnBlue = gui.get<tgui::Button>("blueButton");
	btnStart = gui.get<tgui::Button>("startButton");

	//Action listeners for when the buttons are clicked
	btnGreen->onClick([] { checkButtonClicked(1); });
	btnRed->onClick([] { checkButtonClicked(2); });
	btnYellow->onClick([] { checkButtonClicked(3); });
	btnBlue->onClick([] { checkButtonClicked(4); });
	btnStart->onClick([] { start(); });

	gui.mainLoop();
	
	btnGreen = NULL;
	btnRed = NULL;
	btnYellow = NULL;
	btnBlue = NULL;
	btnStart = NULL;
}

/*
 * Method for when the start button is clicked.
 * Initializes the counters to 0 and generates a random num.
 */
void start() {
	rounds = 0;
	index = 0;
	clickCount = 0;
	btnStart->setEnabled(false);
	generateNum();
	timer->setEnabled(true);
	pattern();

	return;
}

/*
 * Method to generate a random num, 
 * srand is used to make a different set of random nums each game.
 * 
 * The random nums are pushed on to the vector.
 */
 void generateNum() {
	srand(time(NULL));
	randomNum = rand() % 4 + 1;
	vec.push_back(randomNum);
}

/*
 * Pattern method to display the corresponding sequence of the vector.
 * 
 * A different button will flash depending on the num in the vec (1-4).
 */
void pattern() {
	//Boolean so the user can't click a button while the pattern is blinking.
	patternFinished = false; 

	//If the last index of the vector
	if (index == vec.size()) {
		timer->setEnabled(false);
		index = 0;
		patternFinished = true;

		//If the user doesn't click a button in 5 seconds.
		clickTimer->setEnabled(true);
		clickTimer->setCallback([]() { endGame(); });
		return;
	}

	//Gets the value at the vector index
	int val = vec.at(index);

	//Switch statement to light up the button at the value and play a sound.
	switch (val) {
	case 1:
		btnGreen->setEnabled(false);
		buffer.loadFromFile("green_beep.wav");
		sound.setBuffer(buffer);
		sound.play();
		break;

	case 2:
		btnRed->setEnabled(false);
		buffer.loadFromFile("red_beep.wav");
		sound.setBuffer(buffer);
		sound.play();
		break;

	case 3:
		btnYellow->setEnabled(false);
		buffer.loadFromFile("yellow_beep.wav");
		sound.setBuffer(buffer);
		sound.play();
		break;

	case 4:
		btnBlue->setEnabled(false);
		buffer.loadFromFile("blue_beep.wav");
		sound.setBuffer(buffer);
		sound.play();
		break;
	}

	//After the button is disabled, enable it in 0.5s
	timer->setCallback([]() { enableButtons(); });
	index++; //Go to the next index
}

//Method that enables the buttons and goes back to the pattern
void enableButtons() {
	setButtonsEnabled(true);

	timer->setCallback([]() { pattern(); });
}

//Either enables or disables the buttons based on param.
void setButtonsEnabled(bool valid) {
	btnGreen->setEnabled(valid);
	btnRed->setEnabled(valid);
	btnYellow->setEnabled(valid);
	btnBlue->setEnabled(valid);
}

//Method for when a button is clicked.
void checkButtonClicked(int value) {
	//Checks if the pattern has finished blinking.
	if (!patternFinished) {
		return;
	}
	//If it has finished blinking.
	else {
		//Checks if the button clicked does not match the value in the vector index.
		if (value != vec.at(clickCount)) {
			endGame(); 
		}
		else {
			//Checks if the this is the last click of the vector sequence.
			if (clickCount == vec.size() - 1) {
				//Resets everything for the next round.
				timer->setEnabled(false);
				clickTimer->setEnabled(false);
				index = 0;
				clickCount = 0;
				rounds++;
				
				//Plays the "correct" sound effect
				buffer.loadFromFile("correct_beep.wav");
				sound.setBuffer(buffer);
				sound.play();

				//Generates a new number and goes to the next round.
				timer->setEnabled(true);
				generateNum();
				timer->setCallback([]() { enableButtons(); });
				return;
			}
			//If its not the end of the round, but correct button.
			if (value == vec.at(clickCount)) {
				//Plays the "correct" sound effect
				buffer.loadFromFile("correct_beep.wav");
				sound.setBuffer(buffer);
				sound.play();

				clickCount++;

				/* Resets the 5s timer and end the game if 
				the user takes to long to click the button */
				clickTimer->setEnabled(true);
				clickTimer->setCallback([]() { endGame(); });
			}
		}
	}
}

//Method for the end game state
void endGame() {
	//Disables the timer and all of the buttons
	clickTimer->setEnabled(false);
	setButtonsEnabled(false);

	//Plays the "game over" sound effect
	buffer.loadFromFile("game_over.wav");
	sound.setBuffer(buffer);
	sound.play();

	//Makes a child window to display the "Game Over" screen.
	tgui::Gui gui{ window };
	gui.loadWidgetsFromFile("form.txt");

	endGameWindow = gui.get<tgui::ChildWindow>("endGame");
	endGameWindow->setVisible(true);
	
	//Restart the game if the user closes the child window
	endGameWindow.get()->onClose([] { restart(); });

	tgui::Button::Ptr btnPlayAgain = gui.get<tgui::Button>("playAgainButton");
	tgui::Button::Ptr btnQuit = gui.get<tgui::Button>("quitButton");

	//Displays the number of rounds played
	tgui::Label::Ptr roundsLabel = gui.get<tgui::Label>("roundsLabel");
	string message = "Sorry, wrong color.\nYou made it to round " + to_string(rounds) + " of the game";
	roundsLabel->setText(message);

	btnPlayAgain->onClick([] { restart(); }); //Play again - restarts the game
	btnQuit->onClick([] { exit(0); }); //Quit game - exits the program

	gui.mainLoop();
}

//Method to restart the game.
void restart() {
	btnStart->setEnabled(true); //Enables the start button
	endGameWindow->setVisible(false);
	vec.clear(); //Empties the values in the vector
	patternFinished = false;
	makeWindow(); //Goes back and initializes the buttons in the game window
}