#include "eval.hpp"

#include <utility>

#include "../board/piece.hpp"

namespace eval {

constexpr std::array game_phase_increments = {0, 1, 1, 2, 4, 0};
constexpr int        max_game_phase        = 24;

constexpr packed_score S(i16 mg, i16 eg) { return {mg, eg}; }

/// @brief Evaluation terms
namespace terms {

constexpr std::array piece_values = {S(67, 79),   S(281, 250), S(256, 233),
                                     S(330, 378), S(622, 693), S(0, 0)};

constexpr std::array<std::array<packed_score, 64>, 6> all_psqt = {
    {
     {S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),
         S(0, 0),    S(143, 214), S(158, 215), S(138, 211), S(161, 163), S(152, 162), S(129, 175),
         S(48, 226), S(21, 229),  S(18, 159),  S(43, 160),  S(80, 131),  S(83, 109),  S(90, 99),
         S(117, 83), S(96, 129),  S(44, 130),  S(1, 88),    S(33, 77),   S(33, 57),   S(40, 46),
         S(63, 37),  S(53, 40),   S(61, 59),   S(30, 59),   S(-13, 65),  S(21, 60),   S(19, 41),
         S(39, 36),  S(39, 35),   S(30, 37),   S(45, 48),   S(11, 43),   S(-15, 59),  S(17, 58),
         S(15, 40),  S(15, 53),   S(34, 45),   S(23, 42),   S(64, 45),   S(23, 38),   S(-18, 65),
         S(17, 63),  S(7, 51),    S(-3, 58),   S(20, 58),   S(43, 47),   S(74, 44),   S(13, 39),
         S(0, 0),    S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),     S(0, 0),
         S(0, 0)},
     {S(-99, 52),  S(-15, 100), S(46, 119),  S(69, 111),  S(133, 101), S(5, 100),   S(2, 101),
         S(-15, 10),  S(77, 97),   S(111, 117), S(166, 114), S(162, 121), S(157, 107), S(224, 94),
         S(107, 109), S(126, 77),  S(107, 109), S(149, 122), S(173, 140), S(188, 142), S(237, 120),
         S(233, 121), S(182, 111), S(130, 99),  S(105, 120), S(120, 142), S(147, 155), S(175, 156),
         S(146, 161), S(179, 152), S(124, 144), S(146, 109), S(86, 119),  S(107, 133), S(123, 157),
         S(122, 159), S(134, 160), S(128, 149), S(128, 132), S(93, 112),  S(63, 100),  S(91, 122),
         S(104, 132), S(114, 148), S(126, 146), S(107, 129), S(117, 114), S(77, 101),  S(47, 91),
         S(61, 111),  S(82, 119),  S(93, 123),  S(94, 122),  S(102, 115), S(85, 102),  S(78, 99),
         S(6, 78),    S(57, 68),   S(43, 104),  S(62, 109),  S(65, 107),  S(80, 94),   S(58, 74),
         S(31, 75)},
     {S(136, 149), S(112, 159), S(123, 156), S(85, 170),  S(82, 169),  S(97, 159),  S(150, 150),
         S(114, 150), S(160, 139), S(200, 154), S(183, 161), S(163, 163), S(200, 154), S(204, 152),
         S(191, 157), S(169, 134), S(168, 164), S(200, 160), S(210, 167), S(228, 158), S(217, 164),
         S(247, 167), S(222, 159), S(207, 154), S(160, 161), S(173, 179), S(207, 169), S(220, 181),
         S(214, 178), S(206, 174), S(176, 176), S(163, 160), S(153, 155), S(173, 171), S(174, 181),
         S(203, 176), S(201, 176), S(174, 178), S(172, 169), S(161, 147), S(165, 151), S(174, 164),
         S(173, 173), S(174, 172), S(177, 178), S(173, 172), S(175, 153), S(179, 143), S(166, 147),
         S(168, 146), S(179, 148), S(155, 163), S(162, 165), S(182, 150), S(188, 150), S(171, 126),
         S(139, 135), S(162, 148), S(143, 128), S(135, 155), S(140, 149), S(133, 149), S(169, 130),
         S(150, 126)},
     {S(259, 316), S(248, 325), S(260, 329), S(267, 323), S(285, 315), S(292, 311), S(272, 315),
         S(295, 307), S(234, 326), S(233, 336), S(260, 335), S(285, 324), S(264, 326), S(304, 311),
         S(282, 312), S(310, 296), S(208, 329), S(230, 328), S(237, 328), S(244, 323), S(279, 310),
         S(276, 307), S(313, 298), S(279, 298), S(183, 330), S(196, 329), S(207, 334), S(224, 327),
         S(227, 316), S(223, 314), S(230, 309), S(232, 302), S(162, 321), S(171, 323), S(177, 327),
         S(196, 323), S(197, 317), S(176, 317), S(200, 303), S(187, 298), S(153, 312), S(167, 312),
         S(174, 311), S(179, 313), S(184, 308), S(177, 303), S(219, 279), S(190, 283), S(150, 308),
         S(168, 308), S(182, 310), S(183, 309), S(187, 301), S(188, 297), S(205, 288), S(165, 296),
         S(167, 300), S(175, 309), S(186, 317), S(192, 315), S(197, 305), S(181, 301), S(197, 298),
         S(168, 286)},
     {S(449, 600), S(468, 609), S(501, 621), S(538, 605), S(539, 607), S(558, 596), S(560, 557),
         S(499, 599), S(488, 576), S(471, 615), S(481, 646), S(474, 665), S(480, 680), S(536, 641),
         S(511, 621), S(563, 573), S(487, 580), S(489, 604), S(500, 632), S(514, 635), S(521, 654),
         S(575, 624), S(573, 590), S(556, 579), S(473, 589), S(476, 618), S(486, 629), S(488, 651),
         S(493, 663), S(500, 653), S(496, 636), S(508, 607), S(469, 585), S(475, 611), S(474, 619),
         S(480, 649), S(487, 634), S(481, 629), S(494, 605), S(495, 590), S(469, 565), S(481, 576),
         S(476, 607), S(474, 603), S(481, 607), S(485, 599), S(500, 573), S(493, 556), S(467, 560),
         S(479, 562), S(487, 558), S(487, 569), S(485, 571), S(499, 538), S(504, 510), S(510, 484),
         S(468, 551), S(459, 555), S(465, 560), S(480, 540), S(471, 555), S(455, 548), S(470, 529),
         S(468, 524)},
     {S(67, -90),  S(29, -30),  S(54, -26),  S(-31, 5),   S(20, -6),   S(25, 9),   S(42, 5),
         S(123, -75), S(-54, 6),   S(-7, 36),   S(-36, 39),  S(20, 27),   S(5, 41),   S(14, 59),
         S(-9, 61),   S(-36, 26),  S(-83, 23),  S(20, 42),   S(-55, 57),  S(-60, 63), S(-33, 65),
         S(35, 66),   S(1, 71),    S(-47, 41),  S(-71, 12),  S(-74, 48),  S(-88, 63), S(-122, 74),
         S(-118, 77), S(-84, 73),  S(-89, 67),  S(-127, 41), S(-75, 1),   S(-76, 33), S(-105, 57),
         S(-137, 72), S(-133, 72), S(-112, 62), S(-111, 48), S(-133, 27), S(-50, -2), S(-28, 19),
         S(-88, 41),  S(-100, 52), S(-93, 52),  S(-90, 43),  S(-44, 23),  S(-63, 8),  S(46, -27),
         S(-1, 3),    S(-19, 16),  S(-62, 28),  S(-60, 29),  S(-36, 19),  S(22, -4),  S(32, -25),
         S(36, -57),  S(73, -49),  S(36, -26),  S(-84, -1),  S(-4, -33),  S(-50, -7), S(48, -40),
         S(50, -72)},
     }
};

constexpr packed_score tempo = S(30, 21);

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