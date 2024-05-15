#include "position.hpp"

#include <print>

#include "../utils.hpp"

namespace Board {

Position::Position(const std::string &fen) {
    this->pieces.fill(Piece::NO_PIECE);
    const auto tokens = Utils::splitString(fen, ' ');

    if (tokens.size() > 6)
        throw std::invalid_argument("Invalid FEN string: too many fields.\n");
    if (tokens.size() < 6)
        throw std::invalid_argument("Invalid FEN string: missing fields.\n");

    const auto ranks = Utils::splitString(tokens[0], '/');

    int square = std::to_underlying(Square::A8);
    for (const auto &rank : ranks)
    {
        for (const auto c : rank)
        {
            if (std::isdigit(c))
                square += c - '0';
            else
            {
                const auto  sq         = static_cast<Square>(square);
                const Color pieceColor = std::islower(c) ? Color::BLACK : Color::WHITE;
                const Piece piece      = Pieces::charToPiece.at(c);
                this->setPiece(piece, sq, pieceColor);
                square++;
            }
        }
        square = square - 16 + square % std::to_underlying(File::FILE_NB);
    }

    this->stm      = tokens[1] == "w" ? Color::WHITE : Color::BLACK;
    this->castling = CastlingRights(tokens[2]);

    const std::string &enpassant = tokens[3];

    this->epSq = enpassant == "-"
                   ? Square::NO_SQ
                   : Bitboards::Util::squareOf(enpassant[0] - 'a', 8 - (enpassant[1] - '0'));

    this->halfMoveClock  = std::stoi(tokens[4]);
    this->fullMoveNumver = std::stoi(tokens[5]);
}

void printBoard(const Position &pos) {
    std::println("\n +---+---+---+---+---+---+---+---+");

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            const auto  sq           = Bitboards::Util::squareOf(file, rank);
            const Piece currentPiece = pos.pieceOn(sq);
            std::print(" | {}", currentPiece == Piece::NO_PIECE
                                    ? ' '
                                    : Pieces::pieceToChar[std::to_underlying(currentPiece)]);
        }
        std::println(" | {}\n +---+---+---+---+---+---+---+---+", 8 - rank);
    }

    std::println("   A   B   C   D   E   F   G   H\n");
    std::println(" Side to move    : {}", pos.sideToMove() == Color::WHITE ? "white" : "black");

    const auto enpassant = pos.epSquare();

    std::println(" En passant      : {}",
                 enpassant != Square::NO_SQ
                     ? Util::squareToCoordinates[std::to_underlying(enpassant)]
                     : "-");
    std::println(" Castling rights : {}", pos.castlingRights().toString());
    std::println(" Halfmove clock  : {}", pos.fiftyMoveRule());
    std::println(" Fullmove number : {}", pos.fullMoves());
}


} // namespace Board