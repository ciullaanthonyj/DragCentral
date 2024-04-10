BigDelay = 1000
SmallDelay = 60
SetKeyDelay 25
;code
Send, 5{TAB}{SPACE}
Sleep, BigDelay

;name
Send, {TAB}{TAB}[#1]{TAB}
Sleep, SmallDelay
Send, {ENTER}
Sleep, BigDelay

;last name
Send, {TAB}[#2]{TAB}
Sleep, SmallDelay
Send, {ENTER}

Sleep, BigDelay

;second name
Send, {TAB}[#3]{TAB}
Sleep, SmallDelay
Send, {ENTER}
Sleep, BigDelay

SetKeyDelay, 25
;birth date
Send, {TAB}{ALTDOWN}{DOWN}{ALTUP}{HOME}[#4R]{ENTER}
Sleep, SmallDelay


;month, year
Send, {TAB}{ALTDOWN}{DOWN}{ALTUP}{HOME}[#5R]{ENTER}{TAB}[#6]{TAB}
Sleep, SmallDelay

;gender
Send, {ALTDOWN}{DOWN}{ALTUP}{HOME}[GENDER]{ENTER}
Sleep, SmallDelay

;TIN
Send, {TAB}[#8]{TAB}

;DOC
Send, [#9]{TAB}{TAB}{TAB}{TAB}
Send, {ALTDOWN}{DOWN}{ALTUP}{HOME}[DOCTYPE]{ENTER}{TAB}{TAB}[#11]{TAB}[#12]{TAB}

Sleep, SmallDelay

Doc:
Send, {ALTDOWN}{DOWN}{ALTUP}{HOME}[#13R]{ENTER}{TAB}{ALTDOWN}{DOWN}{ALTUP}{HOME}[#14R]{ENTER}{TAB}[#15]{TAB}[#16]{TAB} 
Sleep, SmallDelay

Addr:
Send, {TAB}{TAB}[#17]{TAB}{TAB}{TAB}{TAB}[#18]{TAB}{TAB}{TAB}{TAB}{TAB}[#19]{TAB}{TAB}{TAB}{TAB}{TAB}[#20]{TAB}[#21]{TAB}[#22]{TAB}[#23]{TAB}
Sleep, SmallDelay

Phone:
Send, [#24]{TAB}[#25]{TAB}
Sleep, SmallDelay

SetKeyDelay, 25
Numbers:
Send, [#26]{TAB}{TAB}{TAB}
Sleep, SmallDelay
Send, [#27]{TAB}
Sleep, SmallDelay
Send, [#28]{TAB}
Sleep, SmallDelay