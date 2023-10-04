G = (N, T, P, S)

T = {[let], [var], [:], [=], [if], [else], [{], [}], [,], [func], [−>], [return], [while], [(], [)], [->]​, [id], [Int], [Double], [String]}
N = {<program>, <section> ,<section-tail> ,<statements>, <statement>, <var-declaration>, 
     <optional-type>, <optional-expression>, <assignment>, <conditional>, <loop>, 
     <function-call>, <return-statement>, <function-definitions>, <function-definition>, 
     <optional-return-type>, <parameters>, <parameter>, <name>, <more-parameters>, 
     <optional-var-declaration>, <arguments>, <argument>, <more-arguments>, 
     <expression-or-id>, <else-part>, <block>, <type>}
S = <program> 

P = {

1. <program> → <section>

2. <section> → <statements> <function-definitions> <section-tail>
3. <section-tail> → <section>
4. <section-tail> → ε

5. <statements> → <statement> <statements>
6. <statements> → ε
7. <statement> → <var-declaration>
8. <statement> → <assignment>
9. <statement> → <conditional>
10. <statement> → <loop>
11. <statement> → <function-call>
12. <statement> → <return-statement>

13. <var-declaration> → let id <optional-type> <optional-expression>
14. <var-declaration> → var id <optional-type> <optional-expression>
15. <optional-type> → : <type>
16. <optional-type> → ε
17. <optional-expression> → = <expression>
18. <optional-expression> → ε
19. <assignment> → id = <expression>

20. <function-definitions> → <function-definition> <function-definitions>
21. <function-definitions> → ε
22. <function-definition> → func id ( <parameters> ) <optional-return-type> <block>
23. <optional-return-type> → -> <type>
24. <optional-return-type> → ε
25. <parameters> → <parameter> <more-parameters>
26. <parameters> → ε
27. <parameter> → <name> id : <type>
28. <name> → id
29. <name> → _
30. <more-parameters> → , <parameter> <more-parameters>
31. <more-parameters> → ε

32. <function-call> → <optional-var-declaration> id = id ( <arguments> )
33. <function-call> → id ( <arguments> )
34. <optional-var-declaration> → let
35. <optional-var-declaration> → var
36. <optional-var-declaration> → ε
37. <arguments> → <argument> <more-arguments>
38. <arguments> → ε
39. <argument> → <name> : <expression>
40. <argument> → <expression>
41. <more-arguments> → , <argument> <more-arguments>
42. <more-arguments> → ε
43. <return-statement> → return <expression>
44. <return-statement> → return

45. <conditional> → if <expression-or-id> <block> <else-part>
46. <expression-or-id> → <expression>
47. <expression-or-id> → let id
48. <else-part> → else <block>
49. <else-part> → ε 
50. <loop> → while <expression> <block>
51. <block> → { <statements> }

52. <type> → Int
53. <type> → Double
54. <type> → String

55. <expression> → PREC_ANALYSIS
    }
