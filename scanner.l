%{
  #include <stdlib.h>
  #include <llvm-c/Core.h>
  #include "y.tab.h"
  #include "utils.h"

  void yyerror(LLVMModuleRef module, LLVMBuilderRef builder, const char* s);
  
%}

DIGIT    [0-9]
ID       [A-Za-z][A-Za-z0-9]*

%option noyywrap
%%

if                 { return IF;                                                        }
else               { return ELSE;                                                      }
while              { return WHILE;                                                     }
print              { return PRINT;                                                     }
int                { return INT_TYPE;                                                  }
bool               { return BOOL_TYPE;                                                 }
true               { return TRUE;                                                      }
false              { return FALSE;                                                     }
{DIGIT}+           { yylval.value = atoi(yytext); return VAL;                          }
{ID}               { yylval.id = string_int_get(&global_ids, yytext); return ID;       }
[ \t\r\n]+         /* discard whitespace */
[-*/+><=;\{\}\(\)] { return *yytext;                                                   }
\?                 { return QUESTION_MARK;                                             }
\:                 { return COLON;                                                     }
\>=                { return GE;                                                        }
\<=                { return LE;                                                        }
==                 { return EQ;                                                        }
!=                 { return NE;                                                        }
\>\>               { return RIGHTSHIFT;                                                }
\<\<               { return LEFTSHIFT;                                                 }
\&&                { return AND;                                                       }
\|\|               { return OR;                                                        }
\^                 { return XOR;                                                       }
%                  { return REMAINDER;                                                 }
\++                { return PLUSPLUS;                                                  }
\-\-               { return MINUSMINUS;                                                }
.                  { yyerror(NULL, NULL, "Unexpected character");                      } 

%%
