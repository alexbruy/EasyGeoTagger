#!/bin/sh

#collapes any double spaces to start with
rpl -R -x .cpp -x .h "  (" " (" $1
rpl -R -x .cpp -x .h "(  " "( " $1
rpl -R -x .cpp -x .h "  )" " )" $1
rpl -R -x .cpp -x .h ")  " ") " $1

#set spaces
rpl -R -x .cpp -x .h "(" "( " $1
rpl -R -x .cpp -x .h ")" " )" $1

#collapes any double spaces again
rpl -R -x .cpp -x .h "  (" "(" $1
rpl -R -x .cpp -x .h "(  " "( " $1
rpl -R -x .cpp -x .h "  )" " )" $1
rpl -R -x .cpp -x .h ")  " ") " $1

#set spaces around =
rpl -R -x .cpp -x .h "  =" " =" $1
rpl -R -x .cpp -x .h "=  " "= " $1
rpl -R -x .cpp -x .h "=" " = " $1
rpl -R -x .cpp -x .h "  =" " =" $1
rpl -R -x .cpp -x .h "=  " "= " $1

#clean up some special cases
rpl -R -x .cpp -x .h "= =" "==" $1
rpl -R -x .cpp -x .h "< =" "<=" $1
rpl -R -x .cpp -x .h "> =" ">=" $1
rpl -R -x .cpp -x .h "! =" "!=" $1
rpl -R -x .cpp -x .h "+ =" "+=" $1
rpl -R -x .cpp -x .h "- =" "-=" $1
rpl -R -x .cpp -x .h "/ =" "/=" $1
rpl -R -x .cpp -x .h "* =" "*=" $1
rpl -R -x .cpp -x .h ") ." ")." $1
rpl -R -x .cpp -x .h ") ->" ")->" $1
rpl -R -x .cpp -x .h "))" ") )" $1
rpl -R -x .cpp -x .h "((" "( (" $1
rpl -R -x .cpp -x .h "){" ") {" $1
rpl -R -x .cpp -x .h "MS_DLL_SPEC = " "MS_DLL_SPEC=" $1
rpl -R -x .cpp -x .h "EXPORT = " "EXPORT=" $1
