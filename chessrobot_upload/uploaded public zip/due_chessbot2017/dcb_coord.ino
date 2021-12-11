/*
   Coordinate system as set out in ppt and xls diagrams
   Arm trig calculations in void solveDeg use separate coordinate system with base of arm as origin but these are internal to that function only (see excel file)

  File and Rank go from 1 to 8: a1 is (1,1) ; a8 is (1,8) ; h1 is (8,1) ; h8 is (8,8)
*/

int b2File(int bNum) { //see excel calcs conversions.xlsx for conversion logic
  int intFile = 0;
  int intRank = 0;
  intRank = 8 - bNum / 16;
  intFile = bNum + 16 * intRank - 127;
  return intFile;
}

int b2Rank(int bNum) { //see excel calcs conversions.xlsx for conversion logic
  int intRank = 0;
  intRank = 8 - bNum / 16;
  return intRank;
}

int fr2bNum(int intFile, int intRank) { // gets the b[] array index used by micromax for a given file and rank
  return intFile + 96 - 16 * (intRank + 48) + 799;
}

String fr2Square(int intFile, int intRank) {

  String square = "";
  square.concat((char)(intFile + 96));
  square.concat((char)(intRank + 48));
  return square;
}

int square2X(String square) { //a1 etc // origin is one square off to bottom left of a1

  int intXCalibration[8][8] = {{ -2, -2, -1, -1, -1, -1, -2, -2},
    { -1, -2, -2, -1, -1, -1, -2, -2},
    { -1, -1, -1, -1, 0, 0, 0, 0},
    { -1, -2, -2, -1, 0, 0, 1, 0},
    { -1, 0, 1, 1, 2, 4, 5, 5},
    {0, 1, 3, 5, 5, 8, 9, 10},
    {0, 2, 4, 6, 5, 8, 9, 10},
    {2, 4, 5, 8, 8, 9, 9, 10}
  };

  int intAdjustment = 0;
  if (booCalibration) intAdjustment = intXCalibration[square[0] - 96 - 1][square[1] - 48 - 1];
  Serial.print("x adjustment: "); Serial.println(intAdjustment);
  return (intSquareSide / 2 + intSquareSide * (square[0] - 96) + intAdjustment);
}

int square2Y(String square) {

  int intYCalibration[8][8] = {{ -6, -8, -10, -8, -8, -7, -8, -6},
    { -6, -8, -10, -8, -8, -6, -7, -4},
    { -6, -8, -10, -8, -7, -5, -5, -3},
    { -6, -8, -10, -8, -7, -5, -5, -2},
    { -6, -8, -10, -8, -7, -5, -5, -2},
    { -6, -8, -10, -8, -7, -5, -5, -2},
    { -6, -7, -9, -8, -7, -5, -5, -2},
    { -6, -7, -8, -7, -7, -5, -4, -1}
  };

  int intAdjustment = 0;
  if (booCalibration) intAdjustment = intYCalibration[square[0] - 96 - 1][square[1] - 48 - 1];
  Serial.print("y adjustment: "); Serial.println(intAdjustment);
  return (intSquareSide / 2 + intSquareSide * (square[1] - 48) + intAdjustment);
}

