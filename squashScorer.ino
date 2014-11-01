// Squash Scorer, by @MrWigster
// Squash scoring is fairly easy for the most part, however at 10-10 the first player to have reached 10 must decide whether to play until 13 or 15, as the winner must always have a 2 point lead.
// For this reason we need to include logic to let the players choose what score to play until, always in either incremenets of 2 || 4. EG: 13 || 15, 17 || 19 etc.

//To Do :
// * Set/Games Counter
// Function for score incrementing to help respect DRY coding
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Setting up Switches, 1 for each player to press.
const int player1switch = 7;
const int player2switch = 6;
//Setting up LEDs, 1 representing each player.
const int led1 = 9;
const int led2 = 10;
//Players names
String player1 = "Hraday";
String player2 = "Alex";
int switchStatePlayer1 = 0;
int switchStatePlayer2 = 0;
int targetChosen = 1;
int decideTarget = 1;
int p1score = 0;
int p2score = 0;
int winner;
// Initial Target score to get to
int target = 11;

void setup() {
	Serial.begin(9600);
	lcd.begin(16,2);
	pinMode(player1switch,INPUT);
	pinMode(player2switch,INPUT);
	pinMode(led1,OUTPUT);
	pinMode(led2,OUTPUT);
	lcd.print(player1);
	lcd.print(" Vs ");
	lcd.print(player2);
	lcd.setCursor(0,1);
	lcd.print("SquashScorer 0.1");
}

void loop() {



	// If LED is still on, turn it off.
	if((digitalRead(led1) == HIGH) || (digitalRead(led2) == HIGH)){
		delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
	}




	// If players scores are the same, and it's at the target score to change
	if (targetChosen == 0){
		lcd.clear();
		lcd.setCursor(0,0);
		// Give the players the info about which score they can choose
		lcd.print(target + 2);
		lcd.print("    OR    ");
		lcd.print(target + 4);
		targetChosen = 1;
		decideTarget = 0;
	}
	// While new target score hasn't been decided, keep checking.
	while(decideTarget == 0) {
			newTarget();
	}
	if((targetChosen == 1) && (decideTarget == 1)) {
		switchStatePlayer1 = digitalRead(player1switch);
		if(switchStatePlayer1 == HIGH){
			digitalWrite(led1, HIGH);
			p1score = p1score++;
			if ((p1score == target - 1) && (p2score == target - 1)) { targetChosen = 0; }
			//Deciding if this was winning point
			if ((p1score == target) || (p2score == target) && (p1score != p2score)) {
				winner = whoWins(player1);
			}
			Serial.println("New Score__________");
			Serial.println(p1score);
			newScore();
		}
		switchStatePlayer2 = digitalRead(player2switch);
		if(switchStatePlayer2 == HIGH){
			digitalWrite(led2, HIGH);
			p2score = p2score++;
			if ((p1score == target - 1) && (p2score == target - 1)) { targetChosen = 0; }
			//Deciding if this was winning point
			if ((p1score == target) || (p2score == target) && (p1score != p2score)) {
				winner = whoWins(player2);
			}
			Serial.println("New Score__________");
			Serial.println(p2score);
			newScore();
		}
	}
}
int newTarget(){
	Serial.println(decideTarget);
	Serial.println("Waiting for input...");
	if(digitalRead(player1switch) == HIGH){
		target = target + 2;
		newScore();
		decideTarget = 1;
		delay(250);
	}
	if(digitalRead(player2switch) == HIGH){
		target = target + 4;
		newScore();
		decideTarget = 1;
		delay(250);
	}
}
void newScore(){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(player1);
	lcd.print("      ");
	lcd.print(player2);
	lcd.setCursor(0,1);
	lcd.print(p1score);
	lcd.print(" Target:");
	lcd.print(target);
	lcd.print("  ");
	lcd.print(p2score);
	Serial.println("NewScore() Called");
}
void ledflash(){
	for (int i=0;i<4;i++){
		digitalWrite(led1, LOW);
		digitalWrite(led2, HIGH);
		delay(500);
		digitalWrite(led1, HIGH);
		digitalWrite(led2, LOW);
		delay(500);
	}
	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
}
int whoWins(String player){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(player);
	lcd.setCursor(0,1);
	lcd.print("is the Winner!");
	ledflash();
	delay(3000);
	p1score = 0;
	p2score = 0;
	target = 11;
}
