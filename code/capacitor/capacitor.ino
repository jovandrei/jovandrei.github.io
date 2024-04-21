// Project: Proximity-Based Musical Instrument Using Arduino and Aluminum Foil
	// Description: Uses capacitive sensing to detect proximity and translate it into musical notes.
	// Author: Jovan Tapia

	// Define constants for timer resolution and mains frequency based on geographic location.
	#define resolution 8
	#define mains 60 // 60 Hz for North America and Japan; 50 Hz for most other places.
	#define refresh 2 * 1000000 / mains // Calculate refresh rate based on mains frequency.

	void setup() {
		Serial.begin(115200); // Start serial communication at 115200 baud rate
	
		// Initialize all digital pins as outputs and set them to LOW to stabilize the field
		for(int i = 2; i < 14; i++) {
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
		}
	
		// Set pins 8, 9, and 10 as inputs for capacitive sensing
		for(int i = 8; i < 11; i++)
		pinMode(i, INPUT);
	
		startTimer(); // Initialize the timer for the capacitive sensing loop
	}

	// Declare variables to hold the capacitive readings
	long pin8, pin9, pin10;
	
	int note; // Variable to hold the musical note value
	int scale = 1; // Variable to adjust the musical scale dynamically
	int sharp = 1; // Variable to adjust the musical tone dynamically
	int fadedCounter = -1; // Counter for cycling through faded notes array
	
	void loop() {  
		// Array containing frequencies of faded notes (lower intensity)
		double fadedNotes[] = {196.00,196.00,196.00,246.94,329.62,329.62,329.62,293.66,246.94,246.94,246.94,246.94,174.61,174.61,174.61,164.81};
	
		// Array containing frequencies for normal notes
		long notes[7] = {130.81, 146.83, 164.81, 174.61, 196.00, 220.00, 246.94}; 
		// Array containing frequencies for sharp notes
		long sharpNotes[7] = {138.59, 155.56, 174.61, 185.00, 207.65, 233.08, 261.63};
	
		// Read the capacitive values from the pins
		pin8 = time(8, B00000001);
		pin9 = time(9, B00000010);
		pin10 = time(10, B00000100);
	
		// Determine which note to play based on the proximity measured on pin 9
		if (sharp == 1) {
		// If in sharp mode, select notes based on sharpNotes array
		determineNoteSharp(pin9, notes);
		} else {
		// If not in sharp mode, cycle through faded notes
		determineNoteFaded(pin9, fadedNotes);
		}
	
		// Check for control changes from pin8 or pin10 to toggle scale or sharp mode
		checkControlChanges(pin8, pin10);
	}
	
	// Function to handle determining sharp notes
	void determineNoteSharp(long pinValue, long notes[]) {
		if (pinValue < 23300) {
		Serial.println("C");
		note = notes[0];
		} else if (pinValue >= 23450 && pinValue <= 23550) {
		Serial.println("D");
		note = notes[1];
		}
		// More conditions can be added here for additional notes
	}
	
	// Function to handle determining faded notes
	void determineNoteFaded(long pinValue, double fadedNotes[]) {
		if (pinValue < 23400) {
		note = 0;
		} else if (pinValue >= 23450) {
		fadedCounter++;
		note = fadedNotes[fadedCounter % sizeof(fadedNotes) / sizeof(fadedNotes[0])]; // Cycle through the array
		Serial.println("Playing faded note");
		}
	}
	
	// Function to check and handle control changes from pins
	void checkControlChanges(long pin8Value, long pin10Value) {
		if (pin8Value >= 40100) {
		scale = (scale % 2) + 1; // Toggle scale between 1 and 2
		Serial.print("Scale: ");
		Serial.println(scale);
		delay(500);
		}
		if (pin10Value >= 40100) {
		sharp = (sharp % 2) + 1; // Toggle sharp mode
		Serial.print("Sharp: ");
		Serial.println(sharp);
		delay(500);
		}
	}

	// Custom function to measure the time a pin is high compared to the total cycle time,
	// which correlates to the capacitance of the sensor setup.
	long time(int pin, byte mask) {
		unsigned long count = 0, total = 0;
		startTimer(); // Reset the timer at the start of each measurement.
		while(checkTimer() < refresh) {
		pinMode(pin, INPUT);
		while((PINB & mask) == 0) count++; // Count how long the pin is LOW.
		total++;
		}
		return (count << resolution) / total; // Calculate and return the proportion of time HIGH.
	}

	// Function to start the timer for capacitive measurements.
	void startTimer() {
		timer0_overflow_count = 0;
		TCNT0 = 0;
	}

	// Function to check the elapsed time in the timer, used in the 'time' function.
	unsigned long checkTimer() {
		return ((timer0_overflow_count << 8) + TCNT0) << 2; // Calculate elapsed time.
	}