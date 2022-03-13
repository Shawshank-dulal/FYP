from stockfish import Stockfish


class StockfishEngine:
    stockfish=Stockfish();
    def __init__(self):
        self.stockfish = Stockfish(parameters={
            "Write Debug Log": "false",
            "Contempt": 0,
            "Min Split Depth": 0,
            "Threads": 1,
            "Ponder": "false",
            "Hash": 16,
            "MultiPV": 1,
            "Skill Level": 20,
            "Move Overhead": 30,
            "Minimum Thinking Time": 20,
            "Slow Mover": 80,
            "UCI_Chess960": "false"
        })
    def get_current_status(self):
            return self.stockfish.get_fen_position()

    def get_board_visualization(self):
            return self.stockfish.get_board_visual()

    def move_piece(self,move):
            if(self.stockfish.is_move_correct(move)):
                self.stockfish.make_moves_from_current_position(move)
                return "\nmove Success"
            else:
                return "invalid Move"

    def get_move(self):
            return self.stockfish.get_best_move()

    def move_is_valid(self,move):
            if (self.stockfish.is_move_correct(move)):
                return True
            else:
                return False

    def initialize_board(self,position):
        self.stockfish.set_fen_position(position)
