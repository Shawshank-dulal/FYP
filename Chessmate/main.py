from Chess.stockfish_engine import StockfishEngine


def run_program():
    print('PyCharm')
    jf = StockfishEngine()
    print("welcome to stockfish application:\n 1. start new Game")
    uInput = input()
    if uInput == "1":
        jf.initialize_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 2")
        print("Board Initialized")
        print(jf.get_board_visualization())
    else:
        print("please input correct menu item")
    print("Generating move for d2d4",jf.move_piece("d2e4"))
    print("the best move for d2d4 is:",jf.get_move()  )
    print(jf.get_board_visualization())
# See PyCharm help at https://www.jetbrains.com/help/pycharm/


run_program()