/* See LICENSE file for copyright and license details. */

/* Default terminal emulator */
#define TERMINAL "alacritty"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"JetBrainsMonoMedium:pixelsize=16:antialias=true:autohint=true",
	"JoyPixels:pixelsize=13:antialias=true:autohint=true",
	/* "FontAwesome5Free:pixelsize=12:antialias=true:autohint=true", */
	/* "FontAwesome5Brands:pixelsize=14:antialias=true:autohint=true", */
	"FontAwesome5Free:style=Solid:pixelsize=14:antialias=true:autohint=true",
	"fontawesome-regular:style=Solid:pixelsize=16:antialias=true:autohint=true",

};
static char dmenufont[]             = "JetBrainsMono:size=12:style=Medium";
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#4C566A";
static char normfgcolor[]           = "#eeeeee";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#da1657";
static char selbgcolor[]            = "#da1657";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm","-g", "80x20", NULL };
const char *spcmd2[] = {"st", "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",    spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ "discord",  NULL,       NULL,          1 << 6,       0,           0,         0,        -1 },
	{ "Gpick",    NULL,       NULL,       	    0,       	  1,           0,         0,        -1 },
	{ "Pavucontrol",    NULL,       NULL,       	    0,       	  1,           0,         0,        -1 },
	{ "st-256color",    NULL,     NULL,           	    0,            0,           1,         0,        -1 },
	{ "St",       NULL,     NULL,           	    0,            0,           1,         0,        -1 },
	{ "Alacritty",NULL,     NULL,           	    0,            0,           1,         0,        -1 },
	{ "kitty",    NULL,     NULL,           	    0,            0,           1,         0,        -1 },
	{ NULL,      NULL,     "Event Tester", 	    0,            0,           0,         1,        -1 }, /* xev */
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
    { "Steam",   NULL,        NULL,              1 << 7,       1,           0,         0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
 	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "[B]",	dwindle },		/* Decreasing in size right and leftward */
	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ "|F|",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,			XK_grave,	spawn,	SHCMD("dmenuunicode") },
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,	XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,	XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,	XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_BackSpace,	spawn,		SHCMD("sysact") },
	{ MODKEY|ShiftMask,	XK_BackSpace,	spawn,		SHCMD("sysact") },

	{ MODKEY,			XK_Tab,		view,		{0} },
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY,			XK_w,		spawn,		SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,	XK_w,		spawn,		SHCMD(TERMINAL " -e nmtui") },
	{ MODKEY,			XK_e,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ MODKEY|ShiftMask,	XK_e,		spawn,		SHCMD("vpn --toggle") },
	{ MODKEY,			XK_r,		spawn,		SHCMD(TERMINAL " -e lf") },
	{ MODKEY|ShiftMask,	XK_r,		spawn,		SHCMD("st -e ranger") },
	/* { MODKEY,			XK_t,		spawn,		SHCMD(TERMINAL " -e cointop") }, */
	{ MODKEY, 			XK_y,		cyclelayout,   {.i = +1 } },
	{ MODKEY,			XK_u,		togglescratch,	{.ui = 0}},
	{ MODKEY,			XK_i,		spawn,		SHCMD("toggcomp") },
    { MODKEY|ShiftMask,	XK_i,		spawn,		SHCMD("bl") },
	{ MODKEY,			XK_o,		incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,	XK_o,		incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_p,		spawn,		SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,	XK_p,		spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			XK_bracketleft,		spawn,	SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,	XK_bracketleft,		spawn,	SHCMD("mpc seek -60") },
	{ MODKEY,			XK_bracketright,	spawn,	SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,	XK_bracketright,	spawn,	SHCMD("mpc seek +60") },
	{ MODKEY,			XK_backslash,		view,	{0} },

	{ MODKEY,			XK_a,		spawn,		SHCMD(TERMINAL " -e pulsemixer") },
	{ MODKEY|ShiftMask,	XK_a,		spawn,		SHCMD("pavucontrol") },
	{ MODKEY,			XK_s,		togglefloating,	{0}  },
	{ MODKEY|ShiftMask,	XK_s,		togglesticky,	{0} },
	{ MODKEY,			XK_d,		spawn,      {.v = dmenucmd } },
	{ MODKEY|ShiftMask,	XK_d,		togglegaps,	{0}  },
	{ MODKEY,			XK_f,		togglefullscr,	{0} },
	{ MODKEY,			XK_g,		shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,	XK_g,		shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_h,		setmfact,	{.f = -0.05} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			XK_l,		setmfact,   {.f = +0.05} },
	/* { MODKEY|ShiftMask,	XK_l,		spawn,      SHCMD("keyboard -t") }, */
	{ MODKEY,			XK_semicolon,	shiftview,	{ .i = 1 } },
	{ MODKEY|ShiftMask,	XK_semicolon,	shifttag,	{ .i = 1 } },
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 1} },
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	{ MODKEY|ShiftMask,	XK_Return,	togglescratch,	{.ui = 0} },

	{ MODKEY,			XK_z,		incrgaps,	{.i = +3 } },
	{ MODKEY|ShiftMask,	XK_z,		incrgaps,	{.i = -3 } },
	{ MODKEY,			XK_x,		defaultgaps,{0} },
	{ MODKEY|ShiftMask,	XK_x,		togglegaps,	{0} },
	{ MODKEY,			XK_c,		spawn,		SHCMD("dmenuconfig") },
	{ MODKEY|ShiftMask,	XK_c,		spawn,		SHCMD(TERMINAL " -e calcurse -D ~/.config/calcurse") },
	{ MODKEY,	XK_v,		spawn,		SHCMD("copyq toggle") },
	{ MODKEY|ShiftMask, XK_v,		zoom,		{0}},
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			XK_b,		togglebar,	{0} },
	{ MODKEY|ShiftMask,	XK_b,		spawn,		SHCMD("dmenubooks") },
	{ MODKEY,			XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat") },
	{ MODKEY|ShiftMask,	XK_n,		spawn,		SHCMD(TERMINAL " -e nvim -c VimwikiIndex") },
	{ MODKEY,			XK_m,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ MODKEY|ShiftMask,	XK_m,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_comma,	spawn,		SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,	XK_comma,	spawn,		SHCMD("mpc seek 0%") },
	{ MODKEY,			XK_period,	spawn,		SHCMD("mpc next") },
	{ MODKEY|ShiftMask,	XK_period,	spawn,		SHCMD("mpc repeat") },
	{ MODKEY,          	XK_slash,  	spawn,		SHCMD("showmusic")  },

	{ MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,	XK_Left,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,	XK_Right,	tagmon,		{.i = +1 } },

	{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,	XK_Page_Up,	shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,	XK_Page_Down,shifttag,	{ .i = +1 } },
	{ MODKEY,			XK_Insert,	spawn,		SHCMD("dmenuhistory") },
	{ MODKEY,			XK_F1,		spawn,		SHCMD("groff -mom ~/.local/src/dwm/dwm.mom -Tpdf | zathura -") },
	{ MODKEY,			XK_F2,		spawn,		SHCMD("reloaddwm") },
	{ MODKEY,			XK_F3,		spawn,		SHCMD("dmenudisplay") },
	{ MODKEY,			XK_F4,		spawn,		SHCMD("dmenuscripts") },
	{ MODKEY,			XK_F5,		spawn,		SHCMD("timercontrol -r") },
    /* { MODKEY,			XK_F5,		spawn,		SHCMD("keyboard -t") }, */
	{ MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") },
	{ MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") },
	{ MODKEY,			XK_F8,		spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F9,		spawn,		SHCMD("dmenuumount") },
	{ MODKEY,			XK_F10,		spawn,		SHCMD("bl") },
	/* { MODKEY,			XK_F11,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_F12,		spawn,		SHCMD("playerctl --player spotifyd next") }, */

	{ MODKEY,			XK_space,	zoom,		{0} },
    /* { MODKEY,			XK_space,	spawn,		SHCMD("keyboard -t") }, */
	{ MODKEY|ShiftMask,			XK_space,	spawn,      SHCMD("keyboard -t") },
	/* { MODKEY|ShiftMask,	XK_space,	togglefloating,	{0} }, */

	{ 0,				XK_Print,	spawn,		SHCMD("flameshot gui -p ~") },
	{ ShiftMask,		XK_Print,	spawn,		SHCMD("flameshot full -p ~") },
	/* { MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") }, */
	/* { MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill") }, */
	/* { MODKEY,			XK_Delete,	spawn,		SHCMD("dmenurecord kill") }, */
	/* { MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") }, */

	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,	SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,	SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	{ 0, XF86XK_Sleep,		    spawn,		SHCMD("sudo -A zzz") },
	{ 0, XF86XK_WWW,		    spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,		    spawn,		SHCMD(TERMINAL) },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_Mail,	    	spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL " -e lf /") },
	{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,	SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,	SHCMD("xbacklight -dec 15") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		    MODKEY,		    Button4,	    incrgaps,	    {.i = +1} },
	{ ClkClientWin,		    MODKEY,		    Button5,	    incrgaps,	    {.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		    0,		        Button4,	    shiftview,	    {.i = -1} },
	{ ClkTagBar,		    0,		        Button5,	    shiftview,	    {.i = 1} },
	{ ClkRootWin,		    0,		        Button2,	    togglebar,	    {0} },
};
