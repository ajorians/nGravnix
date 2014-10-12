#include <nspireio.h>
#include "GravnixLib/GravnixLib.h"
#include "Tests.h"

int StartsWith(const char* pstr, const char* pstrFind)
{
   if( strlen(pstrFind) > strlen(pstr) )
      return 0;

   int i;
   for(i=0; i<strlen(pstrFind); i++)
      if( tolower(pstr[i]) != tolower(pstrFind[i]) )
         return 0;

   return 1;
}

int main(int argc, char *argv[])
{
	wait_no_key_pressed();
	
	// Initialize console 1.
	nio_console c1;
	
	nio_init(&c1,NIO_MAX_COLS,NIO_MAX_ROWS,0,0,NIO_COLOR_BLACK,NIO_COLOR_WHITE,TRUE);
	nio_set_default(&c1);
	nio_fflush(&c1);

	nio_printf("%s built at %s, %s\n",__FILE__,__DATE__,__TIME__);
	nio_printf("For help with this console, type \"help\"!\n\n");
	while(1)
	{
		char text[100];
		nio_printf("> ");
		// If no text was entered, exit
		if(!nio_gets(text))
			continue;
		
		// Check for interesting text
		if (!strcmp(text, "help")) {
			nio_printf("GravnixLib Test Help\n");
			nio_printf("  runtests\n");
			nio_printf("    Runs the unit tests\n");
			nio_printf("  setcursor cursor\n");
			nio_printf("    Set the cursor type.\n");
			nio_printf("    cursor can be from 0-3, where:\n");
			nio_printf("      0 is a block cursor\n");
			nio_printf("      1 is an underscore cursor\n");
			nio_printf("      2 is a verical bar cursor\n");
			nio_printf("      3 is a custom cursor\n");
			nio_printf("  setcursorwidth\n");
			nio_printf("    Set the cursor width.\n");
			nio_printf("  clear\n");
			nio_printf("    Clear the console.\n");
			nio_printf("  exit\n");
			nio_printf("    Exit this console.\n");
		}

                if (!strcmp(text, "setcursor 0")) {
                        nio_cursor_type(&c1, NIO_CURSOR_BLOCK);
                        nio_printf(" Cursor type is now set to block cursor.\n");
                }
                if (!strcmp(text, "setcursor 1")) {
                        nio_cursor_type(&c1, NIO_CURSOR_UNDERSCORE);
                        nio_printf(" Cursor type is now set to underscore cursor.\n");
                }
                if (!strcmp(text, "setcursor 2")) {
                        nio_cursor_type(&c1, NIO_CURSOR_VERTICAL);
                        nio_printf(" Cursor type is now set to vertical bar cursor.\n");
                }

		if (!strcmp(text, "setcursorwidth")) {
                        nio_printf("Specify cursor width: ");
                        char num[10];
                        nio_gets(num);
                        nio_cursor_width(&c1, atoi(num));
                }

		if( StartsWith(text, "RunTests") ) {
			RunTests();
		}
		
		if ( !strcmp(text, "clear")) {
			nio_clear(&c1);
		}
		
		if ( StartsWith(text, "exit")) break;
	}
	
	nio_free(&c1);
	
	//if(has_colors)
	//	lcd_incolor();
		
	return 0;
}
