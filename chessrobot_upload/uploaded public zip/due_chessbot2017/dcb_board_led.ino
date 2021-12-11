/*
   COMMENTS
*/

String checkForBoardMove() { //doesn't yet deal enpassant. Castles should work (indrectly) as long as the king is moved first. King treated as moving piece and rook is basically ignored
  String detectedMove = "";
  int intFromFile = 0;
  int intFromRank = 0;
  int intToFile = 0;
  int intToRank = 0;
  int intTempFile = 0;
  int intTempRank = 0;
  int intCounter = 0;
  bool booRecheck = false;
  bool booCaptureDetected = false;
  int intScanDelay = 100;

  bool booOriginalBoardReadings[8][8]; // temporary backup of booBoardReadings in case we need to revert
  bool booLastBoardReadings[8][8]; // temporary internal copy of booBoardReadings
  memcpy (booOriginalBoardReadings, booBoardReadings, sizeof(booBoardReadings));
  memcpy (booLastBoardReadings, booBoardReadings, sizeof(booBoardReadings));

  do {
    refreshBoard(false);
    intCounter = 0;
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (booBoardReadings[i][j] != booLastBoardReadings[i][j]) {
          intCounter = intCounter + 1;
          intTempFile = i + 1;
          intTempRank = j + 1;
        }
      }
    }
    if (intCounter == 1) {
      if (booBoardReadings[intTempFile - 1][intTempRank - 1] > booLastBoardReadings[intTempFile - 1][intTempRank - 1]) {
        intCounter = 99; // piece was added which shouldn't happen at this stage according to moving protocol (remove piece first), so set counter to 99 to guarantee a rescan
        Serial.println("*** Piece was added in first scan. Should only be removed! ***");
      }
      if (booRecheck) break;
      booRecheck = true;
      delay(intScanDelay); //wait briefly before rechecking
    }

  } while (intCounter > 0); // if the counter is > 1, go again until we have a sensible reading. If it is 1, we are looping for the second check. if its 0, nothing changed so we will do nothing

  if (intCounter == 0) return ""; // if no changes found, give up now

  // if we get to here, we have exactly one square that changed and it was a removed piece.
  // intTempFile and intTempRank is the changed square. FOR NOW ASSUME it's the from square
  Serial.println("Detected one removed piece");
  intFromFile = intTempFile;
  intFromRank = intTempRank;
  booRecheck = false; //reset this for next scan
  memcpy (booLastBoardReadings, booBoardReadings, sizeof(booBoardReadings));

  do {
    refreshBoard(false);
    intCounter = 0;
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (booBoardReadings[i][j] != booLastBoardReadings[i][j]) {
          intCounter = intCounter + 1;
          intTempFile = i + 1;
          intTempRank = j + 1;
        }
      }
    }
    if (intCounter == 1) { // only bother doing stuff if there was exactly one change, otherwise we'll be rescanning

      if (booRecheck) { // we've validated and now confident to treat as a changed square
        if (booBoardReadings[intTempFile - 1][intTempRank - 1] > booLastBoardReadings[intTempFile - 1][intTempRank - 1]) {
          // piece was added which should mean we have a simple move of a piece from / to

          intToFile = intTempFile;
          intToRank = intTempRank;
          break;
        }
        if (booBoardReadings[intTempFile - 1][intTempRank - 1] < booLastBoardReadings[intTempFile - 1][intTempRank - 1]) {
          // another piece has disappeared!
          booCaptureDetected = true;
          //first detected square was actually the capture square!
          intToFile = intFromFile; intToRank = intFromRank;
          intFromFile = intTempFile; intFromRank = intTempRank;
          break;
        }
      }
      booRecheck = true;
      delay(intScanDelay); //wait briefly before rechecking
    }
  } while (true); // since we've got to the second stage, we can't quit even if no changes detected, so keep rescanning until we find the other changed square

  if (intFromFile == intToFile && intFromRank == intToRank) {
    // piece must have been picked up and put back - treat this as if no move was detected in the first place
    return "";
  }

  if (booCaptureDetected) {
    //we've had two disappearing pieces, firstly on the To square, then on the From square. Now wait until piece reappears on To square

    while (true) {
      refreshBoard(false);
      if ( booBoardReadings[intToFile - 1][intToRank - 1] == 1) {
        break;
      }
      delay(intScanDelay);
    }
  }

  // if we get to here, we know From and To and all the pieces should be in the right place
  // perform a full re-validation

  booRecheck = true; // if this makes it to the end still true, we're ok.
  refreshBoard(false);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (i == intToFile - 1 && j == intToRank - 1) {
        booRecheck = booRecheck && (booBoardReadings[i][j] == 1); //must have a piece on it
      }
      else if (i == intFromFile - 1 && j == intFromRank - 1) {
        booRecheck = booRecheck && (booBoardReadings[i][j] == 0); //must NOT have a piece on it
      }
      else {
        booRecheck = booRecheck && (booBoardReadings[i][j] == booOriginalBoardReadings[i][j]); // if not from or to square, must be unchaged
      }
    }
  }
  if (booRecheck == false) {
    // something isn't where it should be. Let's forget the whole thing ever happened
    Serial.println("Failed re-check");
    memcpy (booBoardReadings, booOriginalBoardReadings, sizeof(booBoardReadings)); // write original readings back to the global permanent variable, so that we have the right starting point for comparison whenever this function is called again
    return "";
  }
  // report move
  detectedMove = fr2Square(intFromFile, intFromRank);
  detectedMove.concat(fr2Square(intToFile, intToRank));
  Serial.print("Reporting move: "); Serial.println(detectedMove);
  return detectedMove;
}

bool checkBoardvsArray() {

  if (booDisableBoardChecking) return true; //if checking disabled, immediately return true

  bool booCheck = true; // if this makes it to the end still true, we're ok.
  char chrPiece;
  int intExpected = 0;
  refreshBoard(false);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      intExpected = 0;
      chrPiece = getPieceAtFileRank(i + 1, j + 1);
      if (chrPiece == '+' || chrPiece == 'k' || chrPiece == 'n' || chrPiece == 'b' || chrPiece == 'r' || chrPiece == 'q' || chrPiece == '*' || chrPiece == 'K' || chrPiece == 'N' || chrPiece == 'B' || chrPiece == 'R' || chrPiece == 'Q') {
        intExpected = 1;
      }
      booCheck = booCheck && (booBoardReadings[i][j] == intExpected);
    }
  }
  if (!booCheck) printArray();
  return booCheck;
}

void boardTester()
{
  testLED();
  unsigned long timer = millis();
  do {
    refreshBoard(true);
    delay(200);
    if (millis() - timer > 10000) {
      timer = millis();
      testLED();
    }
  } while (true);
}



void refreshBoard(bool booPrint)
{
  readShiftRegisters();
  if (booPrint) printArray();
  updateLED();
}

void readShiftRegisters()
{
  default_SR_Settings(); //reset to correct starting positions
  digitalWrite(pin_shift_PL, LOW); delay(serialScanDelay); digitalWrite(pin_shift_PL, HIGH); //refresh readings ..

  digitalWrite(pin_shift_CE, LOW); //enable clock

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++) //order of readings given is A1..A8..H1..H8
    {
      digitalWrite(pin_shift_CP, LOW);
      booBoardReadings[i][j] = digitalRead(pin_shift_Read); //populate array
      digitalWrite(pin_shift_CP, HIGH);
      delay(serialScanDelay); //was delay 10
    }
  }
}

void printArray()
{
  Serial.println("Array values: ");
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      Serial.print(booBoardReadings[i][j]);
    }
    Serial.println();
  }
}

void default_SR_Settings()
{
  digitalWrite(pin_shift_CP, LOW);
  digitalWrite(pin_shift_PL, HIGH);
  digitalWrite(pin_shift_CE, HIGH);
}
void updateLED() // LED will always show live readings of shift registers, not where we think pieces are
{
  //write along each file then rank, so A1, a8....ending with h1,H8
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      lc.setLed(0, i, j, booBoardReadings[i][j]);
    }
  }
}

void testLED() {

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      lc.setLed(0, i, j, 1);
    }
  }
  delay(1000);
  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, 255);
    delay(50);
  }
  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++)
  {
    lc.setColumn(0, i, 255);
    delay(50);
  }
  delay(1000);

  lc.clearDisplay(0);

}



