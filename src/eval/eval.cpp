#include "eval.hpp"

#include <utility>

#include "../board/piece.hpp"

namespace eval {

constexpr std::array game_phase_increments = {0, 1, 1, 2, 4, 0};
constexpr int        max_game_phase        = 24;

constexpr packed_score S(i16 mg, i16 eg) { return {mg, eg}; }

/// @brief Evaluation terms
namespace terms {

template <typename T>
using piece_table = std::array<T, constants::num_piece_types>;

using piece_square_table = piece_table<std::array<packed_score, constants::num_squares>>;

constexpr piece_table<packed_score> piece_values = {S(61, 93),   S(259, 298), S(237, 279),
                                                    S(297, 454), S(573, 833), S(0, 0)};

constexpr piece_square_table all_psqt = {
    {
     {S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),
     S(0, 0),     S(122, 278), S(155, 270), S(123, 271), S(159, 207), S(141, 202), S(113, 220),
     S(36, 274),  S(-2, 293),  S(21, 196),  S(38, 206),  S(82, 162),  S(88, 136),  S(92, 125),
     S(121, 106), S(94, 167),  S(42, 163),  S(1, 108),   S(33, 93),   S(36, 68),   S(39, 57),
     S(66, 46),   S(55, 51),   S(61, 74),   S(30, 75),   S(-12, 76),  S(21, 72),   S(20, 50),
     S(40, 46),   S(40, 44),   S(31, 46),   S(43, 61),   S(15, 51),   S(-15, 68),  S(17, 71),
     S(16, 48),   S(17, 63),   S(36, 54),   S(22, 51),   S(61, 58),   S(22, 46),   S(-17, 73),
     S(16, 75),   S(9, 57),    S(-3, 64),   S(22, 72),   S(42, 57),   S(71, 56),   S(11, 47),
     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),
     S(0, 0)},
     {S(-86, 43),  S(-42, 123), S(37, 141),  S(78, 130),  S(119, 131), S(41, 102),  S(-24, 128),
     S(-14, 14),  S(87, 120),  S(114, 143), S(145, 151), S(165, 147), S(137, 136), S(222, 122),
     S(101, 141), S(133, 97),  S(104, 140), S(146, 156), S(169, 177), S(182, 178), S(234, 156),
     S(226, 152), S(176, 142), S(130, 124), S(96, 153),  S(112, 177), S(143, 192), S(168, 195),
     S(143, 197), S(177, 189), S(123, 176), S(140, 141), S(77, 152),  S(98, 165),  S(114, 194),
     S(116, 194), S(128, 197), S(121, 185), S(120, 167), S(90, 140),  S(52, 130),  S(81, 154),
     S(95, 167),  S(103, 185), S(117, 183), S(100, 161), S(108, 147), S(72, 132),  S(36, 119),
     S(52, 140),  S(72, 152),  S(85, 155),  S(86, 153),  S(92, 148),  S(75, 127),  S(70, 131),
     S(-15, 114), S(45, 92),   S(35, 132),  S(53, 137),  S(58, 137),  S(72, 122),  S(48, 100),
     S(24, 101)},
     {S(127, 186), S(107, 197), S(118, 194), S(66, 210),  S(79, 202),  S(96, 191),  S(137, 183),
     S(98, 180),  S(159, 168), S(192, 191), S(180, 195), S(153, 199), S(196, 187), S(187, 188),
     S(183, 196), S(155, 169), S(163, 204), S(193, 196), S(197, 209), S(226, 193), S(204, 201),
     S(248, 203), S(214, 196), S(200, 193), S(153, 197), S(169, 216), S(200, 209), S(212, 226),
     S(209, 217), S(202, 214), S(169, 213), S(154, 198), S(144, 190), S(161, 212), S(164, 223),
     S(194, 218), S(190, 218), S(167, 215), S(161, 208), S(153, 177), S(155, 189), S(164, 203),
     S(164, 213), S(165, 212), S(167, 217), S(162, 213), S(167, 190), S(172, 176), S(157, 182),
     S(158, 182), S(173, 180), S(145, 200), S(153, 201), S(172, 187), S(177, 190), S(164, 156),
     S(131, 165), S(155, 188), S(132, 158), S(127, 187), S(133, 182), S(127, 182), S(162, 166),
     S(148, 147)},
     {S(243, 383), S(232, 391), S(242, 400), S(248, 394), S(269, 382), S(286, 375), S(263, 377),
     S(289, 370), S(224, 385), S(224, 400), S(247, 404), S(272, 391), S(251, 393), S(290, 376),
     S(268, 374), S(308, 356), S(196, 388), S(223, 390), S(226, 391), S(230, 387), S(266, 373),
     S(266, 366), S(310, 357), S(282, 350), S(176, 391), S(191, 388), S(195, 399), S(208, 393),
     S(214, 375), S(212, 369), S(220, 366), S(224, 359), S(150, 380), S(155, 385), S(168, 387),
     S(184, 384), S(183, 379), S(161, 377), S(190, 361), S(179, 357), S(140, 373), S(154, 372),
     S(163, 371), S(163, 376), S(170, 369), S(164, 361), S(208, 334), S(180, 337), S(137, 365),
     S(152, 369), S(171, 370), S(167, 372), S(172, 361), S(174, 356), S(193, 345), S(159, 351),
     S(155, 355), S(161, 369), S(172, 379), S(178, 376), S(184, 366), S(168, 359), S(189, 354),
     S(156, 343)},
     {S(422, 703), S(434, 716), S(474, 735), S(519, 713), S(520, 712), S(525, 708), S(542, 653),
     S(470, 698), S(476, 668), S(451, 719), S(459, 757), S(452, 776), S(457, 799), S(502, 754),
     S(481, 732), S(528, 703), S(471, 685), S(472, 705), S(471, 753), S(490, 754), S(490, 773),
     S(551, 745), S(545, 704), S(542, 684), S(451, 695), S(455, 723), S(463, 740), S(462, 768),
     S(465, 782), S(479, 763), S(476, 746), S(483, 720), S(449, 686), S(450, 722), S(448, 732),
     S(458, 760), S(459, 753), S(454, 744), S(468, 716), S(472, 700), S(448, 669), S(455, 688),
     S(450, 719), S(446, 718), S(452, 721), S(458, 713), S(476, 681), S(469, 666), S(446, 658),
     S(452, 664), S(462, 662), S(461, 677), S(459, 680), S(472, 647), S(479, 611), S(496, 569),
     S(442, 653), S(432, 661), S(438, 667), S(452, 656), S(446, 662), S(429, 661), S(461, 620),
     S(450, 621)},
     {S(13, -94),  S(10, -35),   S(39, -24),  S(-133, 37),  S(-68, 14),   S(-16, 19),
     S(38, 7),    S(136, -119), S(-124, 22), S(-85, 62),   S(-132, 74),  S(5, 48),
     S(-63, 77),  S(-51, 91),   S(-26, 83),  S(-59, 42),   S(-165, 44),  S(-36, 66),
     S(-111, 88), S(-135, 101), S(-82, 103), S(15, 93),    S(-31, 94),   S(-67, 53),
     S(-121, 28), S(-129, 71),  S(-149, 91), S(-201, 107), S(-180, 106), S(-140, 101),
     S(-141, 91), S(-180, 59),  S(-109, 11), S(-117, 49),  S(-154, 79),  S(-184, 96),
     S(-181, 95), S(-138, 79),  S(-147, 65), S(-179, 44),  S(-53, -1),   S(-30, 25),
     S(-101, 51), S(-115, 66),  S(-107, 65), S(-106, 54),  S(-48, 29),   S(-70, 13),
     S(60, -33),  S(7, 3),      S(-9, 18),   S(-52, 32),   S(-55, 36),   S(-31, 22),
     S(28, -2),   S(41, -27),   S(57, -76),  S(84, -53),   S(46, -28),   S(-76, -3),
     S(1, -38),   S(-43, -8),   S(58, -44),  S(63, -80)},
     }
};

constexpr packed_score tempo = S(25, 28);

} // namespace terms

int get_game_phase(const board::position& pos) {
    const int game_phase = game_phase_increments[std::to_underlying(piece_type::knight)]
                             * pos.piece_type_bb(piece_type::knight).bit_count()
                         + game_phase_increments[std::to_underlying(piece_type::bishop)]
                               * pos.piece_type_bb(piece_type::bishop).bit_count()
                         + game_phase_increments[std::to_underlying(piece_type::rook)]
                               * pos.piece_type_bb(piece_type::rook).bit_count()
                         + game_phase_increments[std::to_underlying(piece_type::queen)]
                               * pos.piece_type_bb(piece_type::queen).bit_count();

    return std::min(game_phase, max_game_phase);
}

template <color SideToMove>
packed_score evaluate_material(const board::position& pos) {
    constexpr color us   = SideToMove;
    constexpr color them = ~us;

    const int pawn_count =
        pos.piece_count<us>(piece_type::pawn) - pos.piece_count<them>(piece_type::pawn);
    const int knight_count =
        pos.piece_count<us>(piece_type::knight) - pos.piece_count<them>(piece_type::knight);
    const int bishop_count =
        pos.piece_count<us>(piece_type::bishop) - pos.piece_count<them>(piece_type::bishop);
    const int rook_count =
        pos.piece_count<us>(piece_type::rook) - pos.piece_count<them>(piece_type::rook);
    const int queen_count =
        pos.piece_count<us>(piece_type::queen) - pos.piece_count<them>(piece_type::queen);

    packed_score material_score;

    material_score += terms::piece_values[std::to_underlying(piece_type::pawn)] * pawn_count;
    material_score += terms::piece_values[std::to_underlying(piece_type::knight)] * knight_count;
    material_score += terms::piece_values[std::to_underlying(piece_type::bishop)] * bishop_count;
    material_score += terms::piece_values[std::to_underlying(piece_type::rook)] * rook_count;
    material_score += terms::piece_values[std::to_underlying(piece_type::queen)] * queen_count;

    return material_score;
}

template <color SideToMove>
packed_score evaluate_psqt(const board::position& pos) {
    constexpr color us   = SideToMove;
    constexpr color them = ~us;

    packed_score psqt_score;

    auto our_pieces   = pos.occupancies(us);
    auto their_pieces = pos.occupancies(them);

    while (!our_pieces.empty()) {
        const auto       sq = static_cast<square>(our_pieces.pop_lsb());
        const piece_type pt = board::pieces::piece_to_piece_type(pos.piece_on(sq));
        psqt_score +=
            terms::all_psqt[std::to_underlying(pt)][std::to_underlying(relative_square<us>(sq))];
    }

    while (!their_pieces.empty()) {
        const auto       sq = static_cast<square>(their_pieces.pop_lsb());
        const piece_type pt = board::pieces::piece_to_piece_type(pos.piece_on(sq));
        psqt_score -=
            terms::all_psqt[std::to_underlying(pt)][std::to_underlying(relative_square<them>(sq))];
    }

    return psqt_score;
}

template <color SideToMove>
score evaluate(const board::position& pos) {
    constexpr color    us = SideToMove;
    const packed_score packed_eval =
        evaluate_material<us>(pos) + evaluate_psqt<us>(pos) + terms::tempo;
    const int game_phase = get_game_phase(pos);

    const score eval =
        (packed_eval.midgame() * game_phase + packed_eval.endgame() * (max_game_phase - game_phase))
        / max_game_phase;

    return eval;
}

score evaluate(const board::position& pos) {
    return pos.side_to_move() == color::white ? evaluate<color::white>(pos)
                                              : evaluate<color::black>(pos);
}

} // namespace eval