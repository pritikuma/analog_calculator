#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <IRremote.h>

#define led 3
#define MAX_SIZE 150

//Define pins 
const int pinS0 = 7;
const int pinS1 = 8;
const int pinS2 = 9;
const int pinS3 = 10;
const int analogpin = A0;

int button = 0; // Variable to store IR remote button pressed

Adafruit_MCP4725 dac_1; // Create MCP4725 DAC object 1
Adafruit_MCP4725 dac_2; // Create MCP4725 DAC object 2

// Function to map IR remote code to button
int mapCodeToButton(unsigned long code) {
  if ((code & 0x0000FFFF) == 0x0000BF00) {
    code >>= 16;
    if (((code >> 8) ^ (code & 0x00FF)) == 0x00FF) {
      return code & 0xFF;
    }
  }
  return -1;
}
// Function to read IR remote and map to button
int readInfrared() {
  int result = -1;
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    result = mapCodeToButton(code);
    IrReceiver.resume();
  }
  Serial.println(result);
  return result;
}

// Functions to perform addition
float addition(float num1, float num2) {
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  dac_1.setVoltage(num1 * 4098 / 5, false);
  dac_2.setVoltage(num2 * 4098 / 5, false);
  delay(10);  //to be added;
  int rawValue = analogRead(analogpin);
  float voltage = rawValue * (5.0 / 1023.0);
  return voltage;
}
// Functions to perform subtraction
float subtraction(float num1, float num2) {
  digitalWrite(pinS0, LOW);
  digitalWrite(pinS1, HIGH);
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  dac_1.setVoltage(num1 * 4098 / 5, false);
  dac_2.setVoltage(num2 * 4098 / 5, false);
  delay(10);  //to be added;
  int rawValue = analogRead(analogpin);
  float voltage = rawValue * (5.0 / 1023.0);
  return voltage;
}
// Functions to perform multiplication
float multiplication(float num1, float num2) {
  digitalWrite(pinS0, LOW);
  digitalWrite(pinS1, LOW);
  digitalWrite(pinS2, HIGH);
  digitalWrite(pinS3, LOW);
  dac_1.setVoltage(num1 * 4098 / 5, false);
  dac_2.setVoltage(num2 * 4098 / 5, false);
  delay(10);  //to be added;
  int rawValue = analogRead(analogpin);
  float voltage = rawValue * (5.0 / 1023.0);
  return voltage;
}

// Functions to perform radication
float radication(float num1, float num2) {
  digitalWrite(pinS0, LOW);
  digitalWrite(pinS1, LOW);
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  dac_1.setVoltage(num1 * 4098 / 5, false);
  dac_2.setVoltage(num2 * 4098 / 5, false);
  delay(10);  //to be added;
  int rawValue = analogRead(analogpin);
  float voltage = rawValue * (5.0 / 1023.0);
  return voltage;
}

// Function to replace manupulated expression to its value
void replacing(float *rectified_expression, float partial_ans, int start, int end, int &size) {
  float duplicate[size]; //for taking the expression in a dummy array
  int brac_size = end - start - 1;
  
  for (int i = 0; i < size; i++) {
    duplicate[i] = rectified_expression[i];
  }
  int rectified_track = 0;
  for (int i = 0; i < size; i++) {
    if (i == start) {
      rectified_expression[rectified_track] = partial_ans;
      rectified_track++;
    } else if (i >= start + 1 && i <= end) {
      continue;
    } else {
      rectified_expression[rectified_track] = duplicate[i];
      rectified_track++;
    }
  }
  size = size - brac_size - 1;
}

// Function to substitute number in expression
void substituting_num(float *rectified_expression, int operator_tracker, float value, int &size) {
  float duplicate_string[size];
  for (int m = 0; m < size; m++) {
    duplicate_string[m] = rectified_expression[m];
  }
  int z = 0;
  for (int j = 0; j < size; j++) {
    if (j == operator_tracker - 1 || j == operator_tracker + 1) {
      continue;
    } else if (j == operator_tracker) {
      rectified_expression[z] = value;
      z++;

    } else {
      rectified_expression[z] = duplicate_string[j];

      z++;
    }
  }
  size = size - 2;
}

// Function to apply BODMAS rule (Order of operations) in expression
float bodmasrule(float *dump_subarray, int brac_size) {
  int operator_searcher = 1;
  float value = 0;
  while (operator_searcher + 2 <= brac_size) {
    if (dump_subarray[operator_searcher] == 16) {
      value = radication(dump_subarray[operator_searcher - 1], dump_subarray[operator_searcher + 1]);
      substituting_num(dump_subarray, operator_searcher, value, brac_size);
    } else {
      operator_searcher += 2;
    }
  }
  operator_searcher = 1;
  while (operator_searcher + 2 <= brac_size) {
    if (dump_subarray[operator_searcher] == 15) {
      value = multiplication(dump_subarray[operator_searcher - 1], dump_subarray[operator_searcher + 1]);
      substituting_num(dump_subarray, operator_searcher, value, brac_size);
    } else {
      operator_searcher += 2;
    }
  }
  operator_searcher = 1;
  while (operator_searcher + 2 <= brac_size) {
    if (dump_subarray[operator_searcher] == 13) {
      value = addition(dump_subarray[operator_searcher - 1], dump_subarray[operator_searcher + 1]);
      substituting_num(dump_subarray, operator_searcher, value, brac_size);
    } else if (dump_subarray[operator_searcher] == 14) {
      value = subtraction(dump_subarray[operator_searcher - 1], dump_subarray[operator_searcher + 1]);
      substituting_num(dump_subarray, operator_searcher, value, brac_size);
    } else {
      operator_searcher += 2;
    }
  }
  return dump_subarray[0];
}
// Function to calculate expression within brackets
void calculation_arr(float *rectified_expression, int start, int end, int &size) {
  float dump_subarray[end - start - 1];
  float partial_ans = 0;
  for (int i = 0; i < end - start - 1; i++) {
    dump_subarray[i] = rectified_expression[start + 1 + i];
  }
  int brac_size = end - start - 1;

  partial_ans = bodmasrule(dump_subarray, brac_size);
  replacing(rectified_expression, partial_ans, start, end, size);
}

// Function to erase elements in array
void eraseArray(float *arr, int &size, int startIndex, int endIndex, float ans) {
  if (startIndex >= 0 && endIndex < size && startIndex <= endIndex) {
    int elementsToRemove = endIndex - startIndex;
    // Shift elements to fill the gap
    arr[startIndex] = ans;

    for (int i = startIndex + 1; i <= size - elementsToRemove; i++) {
      arr[i] = arr[i + elementsToRemove];
    }

    // Update the size of the array
    size -= elementsToRemove;
  }
}
// Function to remove small brackets from expression 
//that are of no use to be in the expression e.g. ((3)+4) 
//in this small bracket around 3 is of no use.
void small_brac_remover(float *arr, int &size) {
  float *finder = arr;
  float *start = 0;
  float *end = 0;
  int bracket_count = 0;
  float small_brac_value = 0;

  for (int i = 0; finder != (arr + size); finder++) {

    if (*finder == 18) {

      start = finder;
      bracket_count = 1;
    }
    if (*finder == 19) {
      end = finder;
      bracket_count--;
        if (end - start == 2) {
        small_brac_value = *(start + 1);
        eraseArray(arr, size, start - arr, end - arr, small_brac_value);
        end = 0;
        start = 0;
        finder = finder - 2;
      }
    }
  }
}
// Function to check if there are any open brackets in expression
bool isopenthere(float *arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i] == 18) {
      return true;
    }
  }
  return false;
}

// Function to search for innermost brackets in expression so that they can be 
// manupulated first in accordance to bodmas
void Dwarf_Bracket_Searcher(float *arr, int &size) {
  float *mobile_pointer = arr;
  float *start = 0;
  float *end = 0;
  int bracket_count = 0;
  for (int i = 0; mobile_pointer != (arr + size - 1); mobile_pointer++) {
    if (!isopenthere(arr, size)) {
      break;
    }
    if (*mobile_pointer == 18) {
      start = mobile_pointer;
      bracket_count++;
    }

    if (*mobile_pointer == 19) {
      end = mobile_pointer;
      bracket_count--;
      calculation_arr(arr, start - arr, end - arr, size);
      mobile_pointer = arr;
    }
  }
}

void setup() {
   // Initialize IR receiver and DACs
  IrReceiver.begin(2); // Initialize IR receiver on pin 2
  dac_1.begin(0x60); // Initialize DAC 2 with address 0x60
  dac_2.begin(0x62); // Initialize DAC 2 with address 0x62
 analogReference(DEFAULT); // Set analog reference to default (usually 5V)
  pinMode(pinS0, OUTPUT);   // Set pinS0 as output
  pinMode(pinS1, OUTPUT);   // Set pinS1 as output
  pinMode(analogpin, INPUT); // Set analogpin as input
  pinMode(led, OUTPUT);      // Set led pin as output
  Serial.begin(9600);        // Start serial communication at 9600 baud rate
}

void loop() {
 // Initialize variables and arrays
  float expression[MAX_SIZE]; // Array to store the expression
  int num = 0;                // Temporary variable to store numbers
  int track_decimal = 1;      // Tracks decimal places for floating point numbers
  int expression_track = 0;   // Tracks position in the expression array
  bool dot_encountered = false; // Flag to indicate if decimal point has been encountered
  int modulo = 0;             // Modulo flag for operations
  int varnum;                 // Temporary variable to store numbers from IR
  int size = 0;               // Tracks the size of the expression

  while (true) {
     digitalWrite(led, HIGH); // Turn on LED
    button = readInfrared(); // Read input from IR sensor

    // Check different IR button inputs
    if (button == 0) {
      size++;
      break; // Break the loop if button is 0
    }

    if (button == 1) {
      // Code for button 1
      // Check if modulo operation is pending, if so, it's an incorrect input
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
      // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
        // Add the operation identifier (for example, 18 represents some operation)
      expression[expression_track] = 18;
      expression_track++;
      size++;
    }
    if (button == 3) {
      // Code for button 3
      // Check if modulo operation is pending, if so, it's an incorrect input
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
        // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
      expression[expression_track] = 14;
      expression_track++;
      size++;
    }
    if (button == 4) {
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
        // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
      expression[expression_track] = 15;
      expression_track++;
      size++;
    }
    if (button == 5) {
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
        // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
      expression[expression_track] = 13;
      expression_track++;
      size++;
    }
    if (button == 6) {
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
        // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
      expression[expression_track] = 16;
      expression_track++;
      size++;
    }
    if (button == 7) {
      if (modulo == 1) {
        Serial.print("Incorrect input");
        abort();
      }
        // If there's a number being constructed, add it to the expression array
      if (num > 0) {
        expression[expression_track] = num;
        expression_track++;
        num = 0;
        track_decimal = 1;
        dot_encountered = false;
      }
      expression[expression_track] = 19;
      expression_track++;
      size++;
    }
    if (button == 8) {
      modulo = 1;
      size++;
    }
      // If there's a number being constructed, add it to the expression array

    if (button >= 9) {
      if (modulo == 1) {
        expression[expression_track] = varnum;
        modulo = 0;

      } else {
        if (!dot_encountered) {
          num = num * 10 + varnum;

        } else {
          num = num + varnum / pow(10, track_decimal);
          track_decimal++;
        }
      }
      size++;
    }
    if (button >= 12 && button <= 20) {
      if (modulo == 1) {
        expression[expression_track] = varnum;
        modulo = 0;

      } else {
        if (!dot_encountered) {
          num = num * 10 + varnum;

        } else {
          num = num + varnum / pow(10, track_decimal);
          track_decimal++;
        }
      }
      size++;
    }
    if (button == 10) {
      dot_encountered = true;
      size++;
    }
    delay(10);
    digitalWrite(led, LOW);
  }
  // calling the function small_brac_remover
  small_brac_remover(expression, size);
  for (int i = 0; i < size; i++) {
    if (expression[i] == 18 || expression[i] == 19 || expression[i] == 15 || expression[i] == 13 || expression[i] == 14 || expression[i] == 16) {
      expression[i] = expression[i];
    } else {
      expression[i] = expression[i] / 10;
    }
  }
  while (isopenthere(expression, size)) {
    Dwarf_Bracket_Searcher(expression, size);
  }
  if (size == 1) {
    Serial.println(expression[0]);
  } else {
    float ans;
    ans = bodmasrule(expression, size);
    Serial.println(expression[0]);
  }
}
