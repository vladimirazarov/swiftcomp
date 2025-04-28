G = (N, T, P, S)

T = {[let], [var], [:], [=], [if], [else], [{], [}], [,], [func], [−>], [return], [while], [(], [)], [->]​, [id], [Int], [Double], [String]}
N = {<program>, <section> ,<section-tail> ,<statements>, <statement>, <var-declaration>, 
     <optional-type>, <assignment>, <conditional>, <loop>, 
     <function-call>, <return-statement>, <function-definitions>, <function-definition>, 
     <optional-return-type>, <parameters>, <parameter>, <name>, <more-parameters>, 
     <optional-var-declaration>, <arguments>, <argument>, <more-arguments>, 
     <expression-or-id>, <else-part>, <block>, <type>}
S = <program> 

P = {

<program> → <section>

<section> → <statements> <function-definitions> <section-tail>
<section-tail> → <section>
<section-tail> → ε

SECTIONS
-----------------------------------------------------------------------------------------------------------------------------------------------------
<statements> → <statement> <statements>
<statements> → ε

<statement> → <declaration> | <declaration-initialization> | <assignment> | <function-call> | <return-statement> | <while-loop> | <if-cond> | <block>

<declaration> → var id : <type>
<declaration> → let id : <type>

<declaration-initialization> → let id <optional-type> = <expression-or-func-call>
<declaration-initialization> → var id <optional-type> = <expression-or-func-call>
<optional-type> → : <type>
<optional-type> → ε

<assignment> → id = <expression-or-func-call>
<expression-or-func-call> -> <expression>
<expression-or-func-call> -> <function-call>


<function-call> → <optional-var-declaration> id = id ( <arguments> )
<function-call> → id ( <arguments> )
<optional-var-declaration> → let
<optional-var-declaration> → var
<optional-var-declaration> → ε
<arguments> → <argument> <more-arguments>
<arguments> → ε
<argument> → <name> : <litreal-or-id>
<argument> → <literal-or-id>
<argument> → string-literal | int-literal | double-literal | id
<more-arguments> → , <argument> <more-arguments>
<more-arguments> → ε

<return-statement> → return <expression>
<return-statement> → return

<while-loop> → while <expression> <block>

<if-cond> → if <expression-or-id> <block> <else-part>
<expression-or-id> → <expression>
<expression-or-id> → let id
<else-part> → else <block>
<else-part> → ε 

<block> → { <statements> }
--------------------------------------------------------------------------------------------------------------------------------------------------


FUNCTIONS
-----------------------------------------------------------------------------------------------------------------------------------------------------
<function-definitions> → <function-definition> <function-definitions>
<function-definitions> → ε
<function-definition> → func id ( <parameters> ) <optional-return-type> <block>
<optional-return-type> → -> <type>
<optional-return-type> → ε
<parameters> → <parameter> <more-parameters>
<parameters> → ε
<parameter> → <name> id : <type>
<name> → id
<name> → _
<more-parameters> → , <parameter> <more-parameters>
<more-parameters> → ε

-----------------------------------------------------------------------------------------------------------------------------------------------------

TYPES
-----------------------------------------------------------------------------------------------------------------------------------------------------
<type> → Int
<type> → Int_nullable
<type> → Double
<type> → Double-nullable
<type> → String
<type> → String-nullable
-----------------------------------------------------------------------------------------------------------------------------------------------------

<expression> → PREC_ANALYSIS
    }
