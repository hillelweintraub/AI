#ifndef VT100_MACRO_H
#define VT100_MACRO_H

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define VT_BLACK    0
#define VT_RED      1
#define VT_GREEN    2
#define VT_YELLOW   3
#define VT_BLUE     4
#define VT_MAGENTA  5
#define VT_CYAN     6
#define VT_WHITE    7
#define VT_DEFAULT  9

#define reset "\33c"

#define clear_screen "\33[2J"
#define clear_to_bos "\33[1J"
#define clear_to_eos "\33[J"

#define clear_line   "\33[2K"
#define clear_to_bol "\33[1K"
#define clear_to_eol "\33[K"

#define visual_bell  "\33g"
#define audible_bell "\a"

#define show_cursor( v ) ((v) ? "\33\67p" : "\33\66p")

#define goto_xy( x, y ) "\33[" << y << ";" << x << "H"
#define move_to( y, x ) "\33[" << y << ";" << x << "f"

#define cursor_up(    count ) "\33[" << count << "A"
#define cursor_down(  count ) "\33[" << count << "B"
#define cursor_right( count ) "\33[" << count << "C"
#define cursor_left(  count ) "\33[" << count << "D"

#define set_scrolling_region( top, bottom ) "\33[" << top << ";" << bottom << "r"

#define scroll_up(   count ) "\33[" << count << "S"
#define scroll_down( count ) "\33[" << count << "T"

#define insert_line( count ) "\33[" << count << "L"
#define delete_line( count ) "\33[" << count << "M"

#define insert_char( count ) "\33[" << count << "@"
#define delete_char( count ) "\33[" << count << "P"

static bool is_b = FALSE;
static bool is_i = FALSE;
static bool is_u = FALSE;
static bool is_r = FALSE;
static int  fg_c = VT_DEFAULT;
static int  bg_c = VT_DEFAULT;

#define default_attributes "\33[0m"

// these four following are not meant to be used externally
#define set_b( b ) ((is_b = b) ? "\33[1m" : "")
#define set_i( i ) ((is_i = i) ? "\33[3m" : "")
#define set_u( u ) ((is_u = u) ? "\33[4m" : "")
#define set_r( r ) ((is_r = r) ? "\33[7m" : "")

#define set_colors( fg, bg ) "\33[3" << (fg_c = fg) << ";4" << (bg_c = bg) << "m"

#define set_attributes( b, i, u, r ) set_b( b ) << set_i( i ) << set_u( u ) << set_r( r ) << set_colors( fg_c, bg_c )

#define set_bold(      b ) default_attributes << set_attributes( b, is_i, is_u, is_r )
#define set_italic(    i ) default_attributes << set_attributes( is_b, i, is_u, is_r )
#define set_underline( u ) default_attributes << set_attributes( is_b, is_i, u, is_r )
#define set_reverse(   r ) default_attributes << set_attributes( is_b, is_i, is_u, r )

#define finalize default_attributes

#endif
