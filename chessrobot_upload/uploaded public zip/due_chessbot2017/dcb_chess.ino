

void playChess(int intMode, int intN, bool booPlayForcedMoves) {
  int k = 8; //sets whose move it is (white = 8, black = 16)
  int intMoveCounter = 1;
  int intFromFile = 0; //my tracker of origin move square using ints
  int intFromRank = 0;
  int intToFile = 0; //my tracker of target move square using ints
  int intToRank = 0;
  int intMicroMaxResult;
  bool booPrint = true;
  bool booGameNotEnded = true;
  bool booHumanPlaysWhite;
  bool booHumanPlaysBlack;
  bool booHumanMove;
  int intOBIndex = 0; //stores the array position of where we are in the opening book (given initial value zero)
  bool booStillInOpeningBook = true; //true until a move has been made out of opening book
  char prev_b[129]; //copy of b[] to check what pieces were caputured etc. doesn't need to be global like b[] because micromax() doesn't use it.
  String strExecutedMove = "";
  double time = 0;

  // set note limit (for depth of play)
  intNodeLimit = intN;

  if (intMode == 0) {
    booHumanPlaysWhite = true;
    booHumanPlaysBlack = false;
  }
  if (intMode == 1) {
    booHumanPlaysWhite = false;
    booHumanPlaysBlack = true;
  }
  if (intMode == 2) {
    booHumanPlaysWhite = false;
    booHumanPlaysBlack = false;
  }
  if (intMode == 3) {
    booHumanPlaysWhite = true;
    booHumanPlaysBlack = true;
  }

  //initial board setup
  K = 8; while (K--)
  { b[K] = (b[K + 112] = o[K + 24] + 8) + 8; b[K + 16] = 18; b[K + 96] = 9;
    L = 8; while (L--)b[16 * L + K + 8] = (K - 4) * (K - 4) + (L - 3.5) * (L - 3.5);
  }



  while (booGameNotEnded)
  {


    if (booPrint)
    {
      if (time > 0) { //at least one move has been made
        Serial.println();
        Serial.print("Time: "); Serial.println(time / 1000000);
        Serial.print("Nodes: "); Serial.println(N);
      }
      for (int intJ = 0; intJ < 120; intJ++) //changed this to stop using N which is the node variable
      {
        if (intJ & 8 && (intJ += 7))
        {
          Serial.println();
        }
        else {
          Serial.print(n[b[intJ] & 15]);
        }
      }
      Serial.println();

      if (time > 0) { //at least one move has been made
        Serial.print("From:"); Serial.println(fr2Square(intFromFile, intFromRank));
        Serial.print("To:"); Serial.println(fr2Square(intToFile, intToRank));
        Serial.print("Moved piece:"); Serial.println(n[b[L] & 15]);  //hacked from Micromax (works even though L is a char)
      }
      booPrint = false;
    }

    //before we proceed to next move, make sure that the pieces are in the right positions. Also covers initial setup position
    // regardless of whether a human move was made (either physically, or by serial input), or a cpu move made (executed by arm), we need to now make sure that the board scan is updated and matches the position in b[]
    while (!checkBoardvsArray()) {
      // hope the human fixes the situation
      Serial.print("Game doesn't match board..........");
      //maybe print a full debug
      delay(1000); //wait 1 second before checking again
      Serial.println("checking again");
    }

    memcpy (prev_b, b, sizeof(b)); // copy contents of b to prev_b before we start with next move

    K = I;
    N = 0; //cq added

    booHumanMove = ((k == 8 && booHumanPlaysWhite) || (k == 16 && booHumanPlaysBlack));

    // get an input from serial or board if it's booHumanMove
    if (booPlayForcedMoves && getForcedMove(intMoveCounter)) { //getForcedMove returns true if there is one available (and loads c[])
      Serial.println("Forced move used");
    }
    else if (booHumanMove) {
      Serial.println("Waiting for move");
      getHumanMove();
    }

    if (booStillInOpeningBook) {
      if (booHumanMove) {
        intOBIndex = checkOpeningBook(intOBIndex);
      }
      else {
        intOBIndex = getFromOpeningBook(intOBIndex);
      }
      booStillInOpeningBook = !(intOBIndex == 0);
      if (!booStillInOpeningBook) {
        Serial.println("Moved out of opening book!");
      }
    }

    //need to move the char string into c[] once retrieved because that's where micromax gets the human move from
    if (*c - 10) //valued pointed to by c i.e. c[0]. So if c[0] is different from '\n' line feed. i.e. ALWAYS called unless a blank newline is sent
    {
      K = *c - 16 * c[1] + C;
      L = c[2] - 16 * c[3] + C;
    }

    // now clear c[0] so that it's blank for next time
    c[0] = 10;

    if (K == I) //if K is still equal to I then computer will be move (i.e. should only be happening if booHumanMove is false)
    {
      Serial.println("cpu wil generate move");
    }

    time = micros();

    intMicroMaxResult = microMax(k, -I, I, Q, O, 8, 2);
    Serial.print("MM output:"); Serial.println(intMicroMaxResult);

    //potentially submit a nonsense move like a1a1 to get what micromax thinks result is. Seems to give -7999 for either king in check - not clear how to work out whether game has ended

    if (intMicroMaxResult == I) //micromax magic
    {
      //Serial.println("ok");
      // a move was made
      time = micros() - time;
      booPrint = true;
      Serial.print("Move:"); Serial.println(intMoveCounter);
      intMoveCounter++;
      if (intMoveCounter > 30) {
        booMMPrint = true;
      }
      //now K and L will hold ints for Origin and Target squares, whether it was human or cpu move
      intFromFile = b2File(K);
      intFromRank = b2Rank(K);
      intToFile = b2File(L);
      intToRank = b2Rank(L);

      strExecutedMove = "";
      strExecutedMove.concat(fr2Square(intFromFile, intFromRank));
      strExecutedMove.concat(fr2Square(intToFile, intToRank));

      if (!booHumanMove && !booDisableArm) { //if cpu move we need it to be executed. format: from,to,movepiece,capturedpiece
        Serial.println("Executing arm move.");
        armPowerOn();
        delay(500);
        armExecuteMove(strExecutedMove, n[prev_b[K] & 15], n[prev_b[L] & 15]);
        delay(500);
        armPowerOff();
      };



      k ^= 24;  //bitwise xor assignment. k holds value of whose move it is (8 for white, 16 for black. XOR with 24 flips from one to the other)
    }
    else {
      //no move made. Might use this for a catch if an invalid move is attempted
      Serial.println("Move error!");
    }
  }
}

char getPieceAtFileRank(int intFile, int intRank) {
  char chrPiece;
  int bNum; // array index used by micromax for b[] array
  bNum = fr2bNum(intFile, intRank);
  chrPiece = n[b[bNum] & 15];
  return chrPiece;
}


bool getForcedMove(int intMoveCounter) { // used to get a forced move for debugging (populates c[] directly)

  int intForcedMoves = 67;// = 67; //= 30; //67 // set to 0 to disable
  char forcedMoves[277];//[277]= "d2d4c7c5b1c3e7e6e2e4b8c6c1e3d8b6c3a4b6b4c2c3b4a5a4c5d7d6c5b3a5a4d1d2c8d7e3g5f7f6g5f4e8c8g1f3f8e7f1d3c8c7b3c5a4a5d3c4g7g5b2b4a5a3f4g3g5g4f3h4e6e5c5d7c7d7b4b5c6a5c4e2h7h5d4e5d8c8h4g6h8h7e5d6e7d6a1d1c8c3g3d6c3c5d6c5d7e8c5a3e8f7g6f8h7g7d2a5g8e7d1d8g7g8a3e7g8h8a5c7a7a6e7d6f7g8e2c4";

  if (intMoveCounter <= intForcedMoves) {
    for (int i = 0; i < 4; i++) {
      c[i] = forcedMoves[(intMoveCounter - 1) * 4 + i];
    }
    Serial.print(F("Forced move: ")); Serial.print(c[0]); Serial.print(c[1]); Serial.print(c[2]); Serial.print(c[3]); Serial.println(" loaded"); //F() to save memory
    return true;
  }
  return false;
}


void getHumanMove() { //gets move from either serial or the chessboard detection. populates move into c[]

  //loop, continuously checking for either a null terminated string or a change in the board which is a valid move
  String strText = "";
  while (true)
  {

    // read shiftregister array.

    strText = checkForBoardMove(); //justlike checkForCommand, this will either return a move in form of a 4 char string, or return "" if no move detected
    if (strText != "") break;
    // if no physical move detected, check serial
    //checkForCommand();
    strText = checkForCommand();
    if (strText != "") break;
    delay(10); //delay(1) was causing not to read properly
  }
  Serial.print("strText is "); Serial.println(strText);
  //populate c[] array with the move so that micromax can use it
  for (int i = 0; i < 4; i++) {
    c[i] = strText[i];
  }
}


int checkOpeningBook(int intOBIndex) { //checks whether a move made by human player is still in opening book, returns appropriate value for intOBIndex

  int intMovesAvailable = intOBMoveOptions[intOBIndex];
  bool boolMoveMatched;
  for (int i = 0; i < intMovesAvailable; i++) {
    boolMoveMatched = true;
    for (int j = 0; j < 4; j++) {
      boolMoveMatched = boolMoveMatched && (c[j] == charOB[intOBIndex + i][j]); //only true by the end if all chars matched
    }
    if (boolMoveMatched) {
      return intOBNext[intOBIndex + i];
    }
  }
  return 0; //returns 0 if no further moves in opening book
}

int getFromOpeningBook(int intOBIndex) { // gets a CPU move (in 'e2e4' format) from opening book by updating the global char array c[]. Should only be used if we already know we are still in opening book

  int intMovesAvailable = intOBMoveOptions[intOBIndex]; //currently unused
  int intMoveSelection = intOBIndex + random(0, intMovesAvailable); // needs to be randbetween 0 and intMovesAvailable-1. formula is exclusive on upper bound

  Serial.print("OB selected move "); Serial.print(intMoveSelection); Serial.print(" based on random number of "); Serial.println(intMoveSelection - intOBIndex);

  //populate c[] array with the move so that micromax can use it
  for (int i = 0; i < 4; i++) {
    c[i] = charOB[intMoveSelection][i];
  }
  return intOBNext[intOBIndex + intMoveSelection]; //returns 0 if no further moves in opening book
}



/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/***************************************************************************/
/* version 1.6 (1433 non-blank characters) features:                       */
/* - recursive negamax search                                              */
/* - quiescence search with recaptures                                     */
/* - recapture extensions                                                  */
/* - (internal) iterative deepening                                        */
/* - best-move-first 'sorting'                                             */
/* - full FIDE rules and move-legality checking                            */

/* accepts under-promotions: type 1,2,3 (=R,B,N) after input move          */
/* (input buffer c[] & *P made global, K and N encoding swapped for this)  */


// global variable definitions moved to top

//microMax(k,q,l,e,E,z,n)
//int k,q,l,e,E,z,n;
int microMax(int k, int q, int l, int e, int E, int z, int n) //rewritten as above two lines wont compile
{

  //Serial.print("microMax called, n = "); Serial.println(n);

  int j, r, m, v, d, h, i, F, G, s;
  char t, p, u, x, y, X, Y, H, B;

  q--;
  d = X = Y = 0;

  while (d++ < n ||                          //trying just the depth test
         z == 8 & K == I && (N < intNodeLimit & d < 98 ||
                             (K = X, L = Y & ~M, d = 2)))         // && is AND, || is OR
    // while(d++<n|z==8&N<1e4&d<98)                //try code from 3.2
  { x = B = X;
    h = Y & S;
    m = d > 1 ? -I : e;
    N++;
    /*if (booMMPrint) {
      if (fmod(N, 1.00) == 0) {
        Serial.println(N); // crashing on 2457
        }
      if (N == 2457) {
        Serial.println("About to crash");
      }
      }*/
    do {
      u = b[x];
      if (u & k)
      { r = p = u & 7;
        j = o[p + 16];
        while (r = p > 2 & r < 0 ? -r : -o[++j])
        { A:
          y = x; F = G = S;
          do {
            H = y = h ? Y ^ h : y + r;
            if (y & M)break;
            m = E - S && b[E] && y - E < 2 & E - y < 2 ? I : m; /* castling-on-Pawn-check bug fixed */
            if (p < 3 & y == E)H ^= 16;
            t = b[H]; if (t & k | p < 3 & !(y - x & 7) - !t)break;
            i = 99 * w[t & 7];
            m = i < 0 ? I : m;               /* castling-on-Pawn-check bug fixed */
            if (m >= l)goto C;

            if (s = d - (y != z))
            { v = p < 6 ? b[x + 8] - b[y + 8] : 0;
              b[G] = b[H] = b[x] = 0; b[y] = u | 32;
              if (!(G & M))b[F] = k + 6, v += 30;
              if (p < 3)
              { v -= 9 * ((x - 2 & M || b[x - 2] - u) +
                          (x + 2 & M || b[x + 2] - u) - 1);
                if (y + r + 1 & S)b[y] |= 7, i += C;
              }
              v = -microMax(24 - k, -l, m > q ? -m : -q, -e - v - i, F, y, s);
              if (K - I)
              { if (v + I && x == K & y == L & z == 8)
                { Q = -e - i; O = F;
                  if (b[y] - u & 7 && P - c > 5)b[y] -= c[4] & 3; /* under-promotions */
                  return l;
                } v = m;
              }
              b[G] = k + 6; b[F] = b[y] = 0; b[x] = u; b[H] = t;
              if (v > m)
                m = v, X = x, Y = y | S & F;
              if (h) {
                h = 0;
                goto A;
              }
            }
            if (x + r - y | u & 32 |
                p > 2 & (p - 3 | j - 7 ||
                         b[G = x + 3 ^ r >> 1 & 7] - k - 6
                         || b[G ^ 1] | b[G ^ 2])
               )t += p < 5;
            else F = y;
          } while (!t);
        }
      }
    } while ((x = x + 9 & ~M) - B);
C: if (m > I - M | m < M - I)d = 98;
    m = m + I ? m : -microMax(24 - k, -I, I, 0, S, S, 1);
  }
  return m += m < e;
}


