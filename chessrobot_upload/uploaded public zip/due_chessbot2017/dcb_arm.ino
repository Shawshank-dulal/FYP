// ARM functions

void armExecuteMove(String strMove, char chrMovingPiece, char chrTakenPiece) { // needs to know what peices are being moved/taken to get height right.
  bool booCastling = false;
  int intFromXYZ[3] = {0, 0, 0};
  int intToXYZ[3] = {0, 0, 0};
  String strFromSquare = "a1";
  String strToSquare = "a1";
  strFromSquare[0] = strMove[0]; strFromSquare[1] = strMove[1];
  strToSquare[0] = strMove[2]; strToSquare[1] = strMove[3];
  int intStageDelay = 700;

  if ( (chrMovingPiece == 'K' && (strMove == "e1g1" || strMove == "e1c1")) || (chrMovingPiece == 'k' && (strMove == "e8g8" || strMove == "e8c8")) ) {
    booCastling = true;
  }

  Serial.print("Arm making move: "); Serial.print(strMove); if (booCastling) Serial.print(" (castling)"); Serial.println();

  if (chrTakenPiece != '.') {
    //first we have to remove the taken piece

    // position over piece
    intToXYZ[0] = square2X(strToSquare); intToXYZ[1] = square2Y(strToSquare); intToXYZ[2] = intArmHoverHeight;
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    armGripperOpen();
    delay(intStageDelay);
    // lower to grabbing height
    intToXYZ[2] =  armGrabHeight(chrTakenPiece, strToSquare);
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    armGripperClose();
    delay(intStageDelay);
    //raise to hover height
    intToXYZ[2] = intArmHoverHeight;
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    delay(intStageDelay);
    //take to dump spot
    armArcMove(intToXYZ, intCapturedPieceDumpXYZ);
    delay(intStageDelay);
    // lower to release height
    intToXYZ[0] = intCapturedPieceDumpXYZ[0]; intToXYZ[1] = intCapturedPieceDumpXYZ[1];
    intToXYZ[2] =  armGrabHeight(chrTakenPiece, strToSquare);
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    armGripperOpen();
    delay(intStageDelay);
    //raise to hover height
    intToXYZ[2] = intArmHoverHeight;
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    delay(intStageDelay);

    captureDumpIncrement(); // so that next taken piece is place elsewhere

  }

  //now move to the From square
  intFromXYZ[0] = square2X(strFromSquare); intFromXYZ[1] = square2Y(strFromSquare); intFromXYZ[2] = intArmHoverHeight;
  //coming from dump spot if it was a capture, otherwise coming from home
  if (chrTakenPiece != '.') {
    armArcMove(intToXYZ, intFromXYZ);
  }
  else {
    solveDeg(intFromXYZ); smoothMove(fltSolvedDeg);
  }
  armGripperOpen();
  delay(intStageDelay);
  // lower to grabbing height
  intFromXYZ[2] =  armGrabHeight(chrMovingPiece, strFromSquare);
  solveDeg(intFromXYZ); smoothMove(fltSolvedDeg);
  armGripperClose();
  delay(intStageDelay);
  //raise to hover height
  intFromXYZ[2] = intArmHoverHeight;
  solveDeg(intFromXYZ); smoothMove(fltSolvedDeg);
  delay(intStageDelay);
  // move to To Square
  intToXYZ[0] = square2X(strToSquare); intToXYZ[1] = square2Y(strToSquare); intToXYZ[2] = intArmHoverHeight;
  armArcMove(intFromXYZ, intToXYZ);
  delay(intStageDelay);
  // lower to release height
  intToXYZ[2] =  armGrabHeight(chrMovingPiece, strToSquare);
  solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
  armGripperOpen();
  delay(intStageDelay);
  //raise to hover height
  intToXYZ[2] = intArmHoverHeight;
  solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
  delay(intStageDelay);

  if (booCastling) { //if castling, we'd have just put king in its new position, so now we need to do the rook
    //overwrite strFromSquare and strToSquare with the Rook move
    if (strMove == "e1g1") {
      strFromSquare = "h1";
      strToSquare = "f1";
    }
    if (strMove == "e1c1") {
      strFromSquare = "a1";
      strToSquare = "d1";
    }
    if (strMove == "e8g8") {
      strFromSquare = "h8";
      strToSquare = "f8";
    }
    if (strMove == "e8c8") {
      strFromSquare = "a8";
      strToSquare = "d8";
    }

    //now move to the From square
    intFromXYZ[0] = square2X(strFromSquare); intFromXYZ[1] = square2Y(strFromSquare); intFromXYZ[2] = intArmHoverHeight;
    armArcMove(intToXYZ, intFromXYZ); //we're coming from the toXYZ where we placed the king
    armGripperOpen();

    delay(intStageDelay);
    // lower to grabbing height
    intFromXYZ[2] =  armGrabHeight('r', strFromSquare);
    solveDeg(intFromXYZ); smoothMove(fltSolvedDeg);
    armGripperClose();
    delay(intStageDelay);
    //raise to hover height
    intFromXYZ[2] = intArmHoverHeight;
    solveDeg(intFromXYZ); smoothMove(fltSolvedDeg);
    delay(intStageDelay);
    // move to To Square. First, replace intToXYZ with the Rook destination
    intToXYZ[0] = square2X(strToSquare); intToXYZ[1] = square2Y(strToSquare); intToXYZ[2] = intArmHoverHeight;
    armArcMove(intFromXYZ, intToXYZ);
    delay(intStageDelay);
    // lower to release height
    intToXYZ[2] =  armGrabHeight('r', strToSquare);
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    armGripperOpen();
    delay(intStageDelay);
    //raise to hover height
    intToXYZ[2] = intArmHoverHeight;
    solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
    delay(intStageDelay);
  }

  armHome();
}

void armArcMove(int intFromXYZ[], int intToXYZ[]) { // swings from one spot to another by stringing together smooveMoves
  int intMidXYZ[3] = {0, 0, 0};
  int intRememberStepDelay =intStepDelay;
  intMidXYZ[0] = (intFromXYZ[0] + intToXYZ[0]) / 2;
  intMidXYZ[1] = (intFromXYZ[1] + intToXYZ[1]) / 2;
  intMidXYZ[2] = intToXYZ[2] + intArmArcHeight;
  intStepDelay = intStepDelay /1.5; // faster for arc move
  solveDeg(intMidXYZ);
  //printArmDiagnosis();  if (requestConfirmation()) smoothMove(fltSolvedDeg);
  smoothMove(fltSolvedDeg);

  solveDeg(intToXYZ);
  //printArmDiagnosis();  if (requestConfirmation()) smoothMove(fltSolvedDeg);
  smoothMove(fltSolvedDeg);

  intStepDelay = intRememberStepDelay;

}

void armStartUp() {
  int intFromXYZ[3] = {0, 0, 0};
  int intToXYZ[3] = {0, 0, 0};
  armHome();

  armPowerOn();
  delay(2000);

  armGripperOpen();
  delay(500);
  armGripperClose();
  delay(500);
  armGripperOpen();
  delay(500);

  // head to a1
  Serial.println("Heading to a1");
  intToXYZ[0] = square2X("a1"); intToXYZ[1] = square2Y("a1"); intToXYZ[2] = intArmHoverHeight;
  solveDeg(intToXYZ);
  smoothMove(fltSolvedDeg);

  // head to h1
  Serial.println("Heading to h1");
  memcpy (intFromXYZ, intToXYZ, sizeof(intToXYZ)); //populates 'From' with Current 'To'
  intToXYZ[0] = square2X("h1"); intToXYZ[1] = square2Y("h1"); intToXYZ[2] = intArmHoverHeight;
  armArcMove(intFromXYZ, intToXYZ);

  // head to h8
  Serial.println("Heading to h8");
  memcpy (intFromXYZ, intToXYZ, sizeof(intToXYZ)); //populates 'From' with Current 'To'
  intToXYZ[0] = square2X("h8"); intToXYZ[1] = square2Y("h8"); intToXYZ[2] = intArmHoverHeight;
  armArcMove(intFromXYZ, intToXYZ);

  // head to a8
  Serial.println("Heading to a8");
  memcpy (intFromXYZ, intToXYZ, sizeof(intToXYZ)); //populates 'From' with Current 'To'
  intToXYZ[0] = square2X("a8"); intToXYZ[1] = square2Y("a8"); intToXYZ[2] = intArmHoverHeight;
  armArcMove(intFromXYZ, intToXYZ);

  armHome();
  armPowerOff();
}

void armHandShake() {
  int intFromXYZ[3] = {0, 0, 0};
  int intToXYZ[3] = {0, 0, 0};
  armHome();

  armPowerOn();
  delay(2000);

  intToXYZ[0] = 200; intToXYZ[1] = 200; intToXYZ[2] = 200;
  solveDeg(intToXYZ);
  smoothMove(fltSolvedDeg);
  fltSolvedDeg[3] = 180;
  smoothMove(fltSolvedDeg);
  armGripperOpen();
  delay(1500);
  armGripperClose();
  delay(500);

  fltSolvedDeg[2] = 100;
  smoothMove(fltSolvedDeg);
  delay(500);

  fltSolvedDeg[2] = 80;
  smoothMove(fltSolvedDeg);
  delay(500);

  fltSolvedDeg[2] = 90;
  smoothMove(fltSolvedDeg);
  delay(500);
  armGripperOpen();
  delay(500);

  armHome();
  armPowerOff();
}

void armHome() {
  solveDeg(intHomeXYZ);
  smoothMove(fltSolvedDeg);
  fltSolvedDeg[0] = 30;
  fltSolvedDeg[2] = 35;
  fltSolvedDeg[4] = 85;
  smoothMove(fltSolvedDeg);
  armGripperClose();
  writeServoDeg(3, (float)intDegMin[3]);
  delay(1000);
}

int armGrabHeight(char chrPiece, String square) {
  int intHeightAdjust = 0;
  int intRank = (square[1] - 48);
  if (intRank == 1) intHeightAdjust = intHeightAdjust + 23;
  if (intRank == 2) intHeightAdjust = intHeightAdjust + 18;
  if (intRank == 3) intHeightAdjust = intHeightAdjust + 7;
  if (intRank == 4) intHeightAdjust = intHeightAdjust + 5;
  if (intRank == 6) intHeightAdjust = intHeightAdjust + 0;
  if (intRank == 7) intHeightAdjust = intHeightAdjust + 0;
  if (intRank == 8) intHeightAdjust = intHeightAdjust + 0;

  if (chrPiece == '+' || chrPiece == '*' || chrPiece == 'p' || chrPiece == 'P') return intArmGrabHeights[0] + intHeightAdjust; //just so i can use p for arm tester
  if (chrPiece == 'n' || chrPiece == 'N') return intArmGrabHeights[1] + intHeightAdjust;
  if (chrPiece == 'b' || chrPiece == 'B') return intArmGrabHeights[2] + intHeightAdjust;
  if (chrPiece == 'r' || chrPiece == 'R') return intArmGrabHeights[3] + intHeightAdjust;
  if (chrPiece == 'q' || chrPiece == 'Q') return intArmGrabHeights[4] + intHeightAdjust;
  if (chrPiece == 'k' || chrPiece == 'K') return intArmGrabHeights[5] + intHeightAdjust;
  Serial.println("Error - no grab height for target piece!");
  return intArmHoverHeight;
}

void captureDumpIncrement() {

  if (intCapturedPieceDumpXYZ[1] < 320) {
    intCapturedPieceDumpXYZ[1] = intCapturedPieceDumpXYZ[1] + 40;

  }
  else {
    intCapturedPieceDumpXYZ[1] = 60;
    intCapturedPieceDumpXYZ[0] = 400;
  }

}

void armTapSquare(String strSquare, int intTapHeight) {
  int intToXYZ[3] = {0, 0, 0};
  String strFromSquare = "a1";
  String strToSquare = "a1";
  int intStageDelay = 1000;

  // position over piece
  intToXYZ[0] = square2X(strSquare); intToXYZ[1] = square2Y(strSquare); intToXYZ[2] = intArmHoverHeight;
  solveDeg(intToXYZ); smoothMove(fltSolvedDeg);

  delay(intStageDelay);
  intToXYZ[0] = square2X(strSquare); intToXYZ[1] = square2Y(strSquare); intToXYZ[2] = armGrabHeight('p',strSquare)+intTapHeight;
  solveDeg(intToXYZ); smoothMove(fltSolvedDeg);
  delay(intStageDelay * 3);
  intToXYZ[0] = square2X(strSquare); intToXYZ[1] = square2Y(strSquare); intToXYZ[2] = intArmHoverHeight;
  solveDeg(intToXYZ); smoothMove(fltSolvedDeg);

}

void armTester() {

  armHome();
  int intTapHeight = 30;
  while (true) { //just keeps looping. To return to main prog requires reset.
    char chrAction = 0;
    char chrTemp[5];
    int intTemp1 = 0;
    int intTemp2 = 0;
    int intTemp3 = 0;
    int intXYZ[3] = {0, 0, 0};
    int intTapInts[64];
    int intTapOrder[64];

    int intRand = 0;
    float fltTemp = 0;
    String strCommand = "";
    Serial.println("O to power arm, o to power down");
    Serial.println("Format a@ ###(.#) change servo @ in degrees (updates angles and MS)");
    Serial.println("Format m@ #### change servo @ ms (doesn't update angles)");
    Serial.println("Format x ### ### ### move to xyz");
    Serial.println("Format d ###  for step delay");
    Serial.println("Format M@ e2e4 move @ (piece) from square to square");
    Serial.println("Format g e4 go to square");
    Serial.println("Format t e2 tap middle of square");
    Serial.println("Format T tap middle of all squares (a1-h8)");
    Serial.println("Format R tap middle of all squares (random order)");
    Serial.println("Format h ### set tap height");
    Serial.println("c to toggle square calibration adjustments on/off");

    while (chrAction == 0) {
      delay(100);
      strCommand = checkForCommand(); //returns "" and wipes the serial stream if no carriage return was detected.
      chrAction = strCommand[0];  //returns 0 if can't find an int.
    }
    Serial.println("...parsing...");

    switch (chrAction)
    {
      case 'M':
        chrTemp[0] = strCommand[3]; chrTemp[1] = strCommand[4]; chrTemp[2] = strCommand[5]; chrTemp[3] = strCommand[6]; chrTemp[4] = '\0';
        armExecuteMove(chrTemp, strCommand[1], '.');
        break;

      case 'a':
        chrTemp[0] = strCommand[1]; chrTemp[1] = 0; chrTemp[2] = 0; chrTemp[3] = 0; chrTemp[4] = 0;      //null
        intTemp1 = atoi(chrTemp);
        chrTemp[0] = strCommand[3];
        chrTemp[1] = strCommand[4];
        chrTemp[2] = strCommand[5];
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intTemp2 = atoi(chrTemp);
        chrTemp[0] = strCommand[7];
        chrTemp[1] = 0;      //null
        chrTemp[2] = 0;      //null
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intTemp3 = atoi(chrTemp);
        fltTemp = (float)intTemp2 + (float)(intTemp3 / 10.0);
        memcpy(fltSolvedDeg, fltCurrentDeg, sizeof(fltCurrentDeg));
        fltSolvedDeg[intTemp1] = fltTemp;
        smoothMove(fltSolvedDeg);
        break;

      case 'g':
        chrTemp[0] = strCommand[2];
        chrTemp[1] = strCommand[3];
        chrTemp[2] = 0;      //null
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intXYZ[0] = square2X(chrTemp); intXYZ[1] = square2Y(chrTemp); intXYZ[2] = armGrabHeight('p', chrTemp);
        solveDeg(intXYZ); //fltSovledDeg gets populated
        smoothMove(fltSolvedDeg);
        break;

      case 'm':
        chrTemp[0] = strCommand[1];
        chrTemp[1] = 0;      //null
        chrTemp[2] = 0;      //null
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intTemp1 = atoi(chrTemp);
        chrTemp[0] = strCommand[3];
        chrTemp[1] = strCommand[4];
        chrTemp[2] = strCommand[5];
        chrTemp[3] = strCommand[6];
        chrTemp[4] = 0;      //null
        intTemp2 = atoi(chrTemp);
        Serial.print("Writing servo @ with MS ####: "); Serial.print(intTemp1); Serial.print(" "); Serial.println(intTemp2);
        writeServoMS(intTemp1, intTemp2);
        break;

      case 'x': //Format x ### ### ###
        chrTemp[0] = strCommand[2]; chrTemp[1] = strCommand[3]; chrTemp[2] = strCommand[4]; chrTemp[3] = 0; chrTemp[4] = 0;
        intXYZ[0] = atoi(chrTemp);
        chrTemp[0] = strCommand[6]; chrTemp[1] = strCommand[7]; chrTemp[2] = strCommand[8]; chrTemp[3] = 0; chrTemp[4] = 0;
        intXYZ[1] = atoi(chrTemp);
        chrTemp[0] = strCommand[10]; chrTemp[1] = strCommand[11]; chrTemp[2] = strCommand[12]; chrTemp[3] = 0; chrTemp[4] = 0;
        intXYZ[2] = atoi(chrTemp);

        solveDeg(intXYZ); //fltSovledDeg gets populated
        printArmDiagnosis();
        if (requestConfirmation()) smoothMove(fltSolvedDeg);

        break;

      case 'd':
        chrTemp[0] = strCommand[2];
        chrTemp[1] = strCommand[3];
        chrTemp[2] = strCommand[4];
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intTemp1 = atoi(chrTemp);
        intStepDelay = intTemp1;
        Serial.print("intStepDelay set to : "); Serial.println(intStepDelay);
        break;

      case 'h':
        chrTemp[0] = strCommand[2];
        chrTemp[1] = strCommand[3];
        chrTemp[2] = strCommand[4];
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        intTemp1 = atoi(chrTemp);
        intTapHeight = intTemp1;
        Serial.print("intTapHeight set to : "); Serial.println(intTapHeight);
        break;

      case 't':
        chrTemp[0] = strCommand[2];
        chrTemp[1] = strCommand[3];
        chrTemp[2] = 0;      //null
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null
        armTapSquare(chrTemp, intTapHeight);
        break;

      case 'T':
        chrTemp[0] = 0;      //null
        chrTemp[1] = 0;      //null
        chrTemp[2] = 0;      //null
        chrTemp[3] = 0;      //null
        chrTemp[4] = 0;      //null

        for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
            chrTemp[0] = 97 + i;
            chrTemp[1] = 49 + j;
            armTapSquare(chrTemp, intTapHeight);
          }
        }
        break;

      case 'R':
        chrTemp[0] = 0; chrTemp[1] = 0; chrTemp[2] = 0; chrTemp[3] = 0; chrTemp[4] = 0; //null

        for (int i = 0; i < 64; i++) {
          intTapInts[i] = i + 1;
        }

        for (int i = 0; i < 64; i++) { // creates array with ints 1-64 in random order
          intTemp1 = 0;
          intRand = random(0, 63 - i);
          for (int j = 0; j < 64; j++) {
            if (intTapInts[j] > 0) {
              if (intTemp1 == intRand) {
                intTapOrder[i] = intTapInts[j];
                intTapInts[j] = 0;
                break;
              }
              intTemp1++;
            }
          }
        }
        for (int i = 0; i < 64; i++) {
          chrTemp[0] = 97 + ((intTapOrder[i] - 1) % 8);
          chrTemp[1] = 49 + ((intTapOrder[i] - 1) / 8);
          armTapSquare(chrTemp, intTapHeight);
        }


        break;

      case 'c':
        booCalibration = !booCalibration;
        Serial.print("booCalibration =");Serial.println(booCalibration);
        break;

      case 'O':
        digitalWrite(RELAYPIN, HIGH);
        break;

      case 'o':
        digitalWrite(RELAYPIN, LOW);
        break;
    }
  }
}

void writeServoMS(int intServoNum, int intMS) { //the only function that actually writes to servos. will block writing MS that isn't inside min max range
  bool booSafe = false;
  if (intMSMin[intServoNum] <= intMSMax[intServoNum]) { //need this because some cases the MS min and max are reversed
    booSafe = ((intMS >= intMSMin[intServoNum]) && (intMS <= intMSMax[intServoNum]));
  }
  else booSafe = ((intMS <= intMSMin[intServoNum]) && (intMS >= intMSMax[intServoNum]));

  if (booSafe) {
    intCurrentMS[intServoNum] = intMS;
    armServos[intServoNum].writeMicroseconds(intMS);
  }
  else {
    if (booPrintServoLimits) {
      Serial.print("Blocked writing ");
      Serial.print(intMS);
      Serial.print(" MS to servo ");
      Serial.println(intServoNum);
    }
  }
}

void writeServoDeg(int intServoNum, float fltAngle)
{
  int intMS = 0;
  intMS = round(((float)intMSMax[intServoNum] - (float)intMSMin[intServoNum]) * (fltAngle - (float)intDegMin[intServoNum]) / ((float)intDegMax[intServoNum] - (float)intDegMin[intServoNum]) + (float)intMSMin[intServoNum]);
  fltCurrentDeg[intServoNum] = fltAngle;
  writeServoMS(intServoNum, intMS);
}


void smoothMove(float fltTargetDeg[]) { //moves in increments of 0.1 degree
  float fltMaxDegChange = 0; //max across all the servos
  float fltRequiredDegChanges[5];
  float fltDegIncrements[5]; // whichever is moving by most should move 0.1 deg, others will be less
  float fltStartingDeg[5]; // need to refer back to Starting angles to avoid rounding issues
  memcpy(fltStartingDeg, fltCurrentDeg, sizeof(fltCurrentDeg)); //need to store the current angles as they will get overwritten as servos move

  for (int j = 0; j < 5; j++)
  {
    fltRequiredDegChanges[j] = fltTargetDeg[j] - fltCurrentDeg[j];
    fltMaxDegChange = max(fltMaxDegChange, abs(fltRequiredDegChanges[j]));
  }

  if (fltMaxDegChange > 0) {
    for (int j = 0; j < 5; j++)
    {
      fltDegIncrements[j] = (float)(fltRequiredDegChanges[j] / fltMaxDegChange) / 10.0; //this will be 0.1 for the servo moving the furthest
    }
  }

  if (false)
  {
    Serial.print("Max angle change " ); Serial.print(fltMaxDegChange); Serial.println(".");
    Serial.println("Format: (Base, Shoulder, Elbow, Wrist, Gripper)");

    Serial.print("fltStartingDeg[] contains: (");
    for (int j = 0; j < 5; j++)
    {
      Serial.print(fltStartingDeg[j]);
      if (j + 1 < 5) Serial.print(",");
    }
    Serial.println(")");

    Serial.print("fltCurrentDeg[] contains: (");
    for (int j = 0; j < 5; j++)
    {
      Serial.print(fltCurrentDeg[j]);
      if (j + 1 < 5) Serial.print(",");
    }
    Serial.println(")");

    Serial.print("fltRequiredDegChanges[] contains: (");
    for (int j = 0; j < 5; j++)
    {
      Serial.print(fltRequiredDegChanges[j]);
      if (j + 1 < 5) Serial.print(",");
    }
    Serial.println(")");

    Serial.print("fltDegIncrements[] contains: (");
    for (int j = 0; j < 5; j++)
    {
      Serial.print(fltDegIncrements[j]);
      if (j + 1 < 5) Serial.print(",");
    }
    Serial.println(")");
  }

  if (true)
  {
    for (int i = 0; i < (int)(fltMaxDegChange * 10 + 1); i++) //increments of 0.1 degree
    {
      for (int j = 0; j < 4; j++) //avoid affecting gripper
      {
        if (false)
        {
          Serial.print("writing servo ");
          Serial.print(j);
          Serial.print(" ");
          Serial.println((fltStartingDeg[j] + i * fltDegIncrements[j]));
        }
        writeServoDeg(j, (fltStartingDeg[j] + i * fltDegIncrements[j]));
        delayMicroseconds(intStepDelay);
      }
    }
    //won't nec be at exact position because of overincrements, so finalise now

    for (int j = 0; j < 4; j++) // avoid affecting gripper
    {
      writeServoDeg(j, fltTargetDeg[j]);
    }
  }
}

void solveDeg(int intXYZ[]) //reverse kinematics, populates intSolvedDeg using provided XYZ.
{
  //convert to separate variables to avoid changing original code
  // NOTE X AND Y SWITCHED !!
  int intX = intXYZ[1]; // Y value
  int intY = intXYZ[0]; // X value
  int intZ = intXYZ[2];

  //convert from board xyz to xyz relative to base of arm (also requires switching x and y)
  intX = intArmOriginXYZ[1] - intX;
  intY = intY - intArmOriginXYZ[0];

  bool booReverseAngles[5] = {false, false, false, false, false};
  // rad = deg*PI/180.0
  //acos returns answers in radians hence use of PI/180.0

  float pV = 0;  //vertical position of wrist in plane of the arm
  float pH = 0;  //horizonal position of wrist in plane of the arm
  int intH = intArmOriginXYZ[2];
  int intA = intArmLengths[0];
  int intB = intArmLengths[1];
  int intC = intArmLengths[2];

  for (int j = 0; j < 4; j++) //dont include gripper
  {
    fltSolvedDeg[j] = 90;
  }

  if (intY == 0) fltSolvedDeg[0] = 90;
  else if (intY < 0) fltSolvedDeg[0] = 180.0 - (180.0 / PI) * atan((float)intX / -intY);
  else fltSolvedDeg[0] = (180.0 / PI) * atan((float)intX / intY);

  pV = intZ - intH + intC;

  if (fltSolvedDeg[0] == 0) pH = intX;
  else pH = intX / sin((PI / 180.0) * fltSolvedDeg[0]);

  fltSolvedDeg[2] = (180.0 / PI) * acos( (float)(pow(intA, 2) + pow(intB, 2) - pow(pV, 2) - pow(pH, 2)) / ((float)2 * intA * intB) );

  fltSolvedDeg[1] = 180.0 - (180.0 / PI) * acos( (pow(intA, 2) + pow(pV, 2) + pow(pH, 2) - pow(intB, 2)) / (2 * intA * pow(pow(pH, 2) + pow(pV, 2), 0.5) ))
                    + ((pV < 0) ? 1 : -1) * (180.0 / PI) * acos(pH / pow(pow(pH, 2) + pow(pV, 2), 0.5));

  fltSolvedDeg[3] = 90 + fltSolvedDeg[1] - fltSolvedDeg[2];

  if (false)
  {
    Serial.print("fltSolvedDeg[] before any reversing contains: [");
    for (int j = 0; j < 5; j++)
    {
      Serial.print(fltSolvedDeg[j]);
      if (j + 1 < 5) Serial.print(",");
    }
    Serial.println("]");
  }

  for (int j = 0; j < 4; j++) // don't write anything for gripper
  {
    if (booReverseAngles[j]) fltSolvedDeg[j] = 180.0 - fltSolvedDeg[j];
  }
}

void printArmDiagnosis()
{
  Serial.print("\n");

  Serial.println("Format: (Base, Shoulder, Elbow, Wrist, Gripper)");

  Serial.print("fltCurrentDeg[] contains: (");
  for (int j = 0; j < 5; j++)
  {
    Serial.print(fltCurrentDeg[j]);
    if (j + 1 < 5) Serial.print(",");
  }
  Serial.println(")");

  Serial.print("fltSolvedDeg[] contains: [");
  for (int j = 0; j < 5; j++)
  {
    Serial.print(fltSolvedDeg[j]);
    if (j + 1 < 5) Serial.print(",");
  }
  Serial.println("]");

  Serial.print("\n");
}
void printAll()
{
  Serial.print("\n");
  Serial.println("Attached servos (pin, MSMin, MSMax): ");
  for (int j = 0; j < 5; j++)
  {
    Serial.print(strServoNames[j]); Serial.print(": "); Serial.print(intServoPins[j]); Serial.print(", "); Serial.print(intMSMin[j]); Serial.print(", ");  Serial.print(intMSMax[j]);
    Serial.println("");
  }
  Serial.println("Arm lengths in mm are (h, a, b, c): ");
  for (int j = 0; j < 4; j++)
  {
    Serial.print(intArmLengths[j]);
    if (j < 3) Serial.print(", ");
  }
  Serial.println("");

  Serial.println("Positions (intCurrentMS and fltCurrentDeg): ");
  for (int j = 0; j < 5; j++)
  {
    Serial.print(strServoNames[j]); Serial.print(": "); Serial.print(intCurrentMS[j]); Serial.print(", "); Serial.print(fltCurrentDeg[j]);
  }
  Serial.println("");


}

boolean requestConfirmation() {
  boolean booResult = false;
  String strCommand = "";
  Serial.println("Type y to confirm, anything else to cancel.");
  while (strCommand[0] == 0) {
    delay(100);
    strCommand = checkForCommand(); //returns "" and wipes the serial stream if no carriage return was detected.
    strCommand[0];  //returns 0 if can't find an int.
  }
  if (strCommand == "y\0") booResult = true;
  return booResult;
}



void armGripperClose() {
  writeServoMS(4, intMSMax[4]);
}
void armGripperOpen() {
  writeServoMS(4, intMSMin[4]);
}

void armPowerOn() {
  digitalWrite(RELAYPIN, HIGH);
}
void armPowerOff() {
  digitalWrite(RELAYPIN, LOW);
}



