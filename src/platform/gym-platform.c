#include <limits.h>
#include <unistd.h>
#include "platform.h"

static enum displayGlyph glyphs[ROWS][COLS];

static char glyphToAscii(enum displayGlyph glyph) {
    unsigned int ch;

    switch (glyph) {
        case G_UP_ARROW: return '^';
        case G_DOWN_ARROW: return 'v';
        case G_FLOOR: return '.';
        case G_CHASM: return ':';
        case G_TRAP: return '%';
        case G_FIRE: return '^';
        case G_FOLIAGE: return '&';
        case G_AMULET: return ',';
        case G_SCROLL: return '?';
        case G_RING: return '=';
        case G_WEAPON: return '(';
        case G_GEM: return '+';
        case G_TOTEM: return '0'; // zero
        case G_GOOD_MAGIC: return '$';
        case G_BAD_MAGIC: return '+';
        case G_DOORWAY: return '<';
        case G_CHARM: return '7';
        case G_GUARDIAN: return '5';
        case G_WINGED_GUARDIAN: return '5';
        case G_EGG: return 'o';
        case G_BLOODWORT_STALK: return '&';
        case G_FLOOR_ALT: return '.';
        case G_UNICORN: return 'U';
        case G_TURRET: return '*';
        case G_CARPET: return '.';
        case G_STATUE: return '5';
        case G_CRACKED_STATUE: return '5';
        case G_MAGIC_GLYPH: return ':';
        case G_ELECTRIC_CRYSTAL: return '$';

        default:
            ch = glyphToUnicode(glyph);
            brogueAssert(ch < 0x80); // assert ascii
            return ch;
    }
}

void printDisplayBufferToConsole() {
    printf("<state>\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", glyphToAscii(glyphs[i][j]));
        }
        printf("\n");
    }
    printf("</state>\n");
    fflush(stdout);
}

static void _gameLoop() {
    int statusCode = rogueMain();
    exit(statusCode);
}

static boolean _pauseForMilliseconds(short ms, PauseBehavior behavior) {
    sleep(ms / 1000);
    return false;
}

static void _nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance) {
    printDisplayBufferToConsole();

    returnEvent->eventType = EVENT_ERROR;
    returnEvent->controlKey = 0;
    returnEvent->shiftKey = 0;
    returnEvent->param1 = 0;
    returnEvent->param2 = 0;

    char command[100];
    if (fgets(command, sizeof(command), stdin) == NULL) {
        rogue.gameHasEnded = true;
        rogue.nextGame = NG_QUIT;
        returnEvent->eventType = KEYSTROKE;
        returnEvent->param1 = QUIT_KEY;
        return;
    }
    command[strcspn(command, "\n")] = 0;

    returnEvent->eventType = KEYSTROKE;

    if (strcmp(command, "quit") == 0) {
        rogue.gameHasEnded = true;
        rogue.nextGame = NG_QUIT;
        returnEvent->param1 = QUIT_KEY;
    } else if (strcmp(command, "up") == 0 || strcmp(command, "8") == 0) {
        returnEvent->param1 = UP_KEY;
    } else if (strcmp(command, "down") == 0 || strcmp(command, "2") == 0) {
        returnEvent->param1 = DOWN_KEY;
    } else if (strcmp(command, "left") == 0 || strcmp(command, "4") == 0) {
        returnEvent->param1 = LEFT_KEY;
    } else if (strcmp(command, "right") == 0 || strcmp(command, "6") == 0) {
        returnEvent->param1 = RIGHT_KEY;
    } else if (strcmp(command, "upright") == 0 || strcmp(command, "9") == 0) {
        returnEvent->param1 = UPRIGHT_KEY;
    } else if (strcmp(command, "upleft") == 0 || strcmp(command, "7") == 0) {
        returnEvent->param1 = UPLEFT_KEY;
    } else if (strcmp(command, "downright") == 0 || strcmp(command, "3") == 0) {
        returnEvent->param1 = DOWNRIGHT_KEY;
    } else if (strcmp(command, "downleft") == 0 || strcmp(command, "1") == 0) {
        returnEvent->param1 = DOWNLEFT_KEY;
    } else if (strcmp(command, "rest") == 0 || strcmp(command, "5") == 0) {
        returnEvent->param1 = REST_KEY;
    } else if (strcmp(command, "search") == 0 || strcmp(command, "s") == 0) {
        returnEvent->param1 = SEARCH_KEY;
    } else if (strcmp(command, "inventory") == 0 || strcmp(command, "i") == 0) {
        returnEvent->param1 = INVENTORY_KEY;
    } else if (strcmp(command, "equip") == 0 || strcmp(command, "e") == 0) {
        returnEvent->param1 = EQUIP_KEY;
    } else if (strcmp(command, "drop") == 0 || strcmp(command, "d") == 0) {
        returnEvent->param1 = DROP_KEY;
    } else if (strcmp(command, "apply") == 0 || strcmp(command, "a") == 0) {
        returnEvent->param1 = APPLY_KEY;
    } else if (strcmp(command, "throw") == 0 || strcmp(command, "t") == 0) {
        returnEvent->param1 = THROW_KEY;
    } else if (strcmp(command, "descend") == 0 || strcmp(command, ">") == 0) {
        returnEvent->param1 = DESCEND_KEY;
    } else if (strcmp(command, "info") == 0 || strcmp(command, "D") == 0) {
        returnEvent->param1 = DISCOVERIES_KEY;
    } else if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0) {
        returnEvent->param1 = BROGUE_HELP_KEY;
    } else if (strcmp(command, "escape") == 0 || strcmp(command, "esc") == 0) {
        returnEvent->param1 = ESCAPE_KEY;
    } else if (strcmp(command, "return") == 0 || strcmp(command, "enter") == 0) {
        returnEvent->param1 = RETURN_KEY;
    } else if (strcmp(command, "acknowledge") == 0) {
        returnEvent->param1 = ACKNOWLEDGE_KEY;
    } else {
        if (strlen(command) > 0) {
            returnEvent->param1 = command[0];
        } else {
            returnEvent->param1 = ACKNOWLEDGE_KEY;
        }
    }
}

static void _plotChar(enum displayGlyph inputChar, short x, short y, short foreRed, short foreGreen, short foreBlue, short backRed, short backGreen, short backBlue) {
    glyphs[y][x] = inputChar;
}

static void _remap(const char *from, const char *to) {
}

static boolean _modifierHeld(int mod) {
    return false;
}

static boolean _takeScreenshot() {
    return false;
}

static enum graphicsModes _setGraphicsMode(enum graphicsModes mode) {
    return TEXT_GRAPHICS;
}

struct brogueConsole gymConsole = {
    _gameLoop,
    _pauseForMilliseconds,
    _nextKeyOrMouseEvent,
    _plotChar,
    _remap,
    _modifierHeld,
    NULL,
    _takeScreenshot,
    _setGraphicsMode
};