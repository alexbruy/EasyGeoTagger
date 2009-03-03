#!/bin/sh

#set spaces
rpl -R -x .cpp -x .h "(" "( " $1
rpl -R -x .cpp -x .h ")" " )" $1

#preserve three or more space, they are probably intended, add teo extra because the next step is going to remove them
rpl -R -x .cpp -x .h "   (" "     (" $1

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
rpl -R -x .cpp -x .h "))" ") )" $1
rpl -R -x .cpp -x .h "((" "( (" $1
rpl -R -x .cpp -x .h "((" "( (" $1
rpl -R -x .cpp -x .h "){" ") {" $1
rpl -R -x .cpp -x .h "MS_DLL_SPEC = " "MS_DLL_SPEC=" $1
rpl -R -x .cpp -x .h "EXPORT = " "EXPORT=" $1
